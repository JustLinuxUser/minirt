#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include "../libft/dsa/dyn_str.h"
#include "../libft/utils/utils.h"
#include "rt_parser.h"
#include "rt_tokenizer.h"

bool unexpected_err(t_rt_parser* parser) {
    parser->err = RT_ERR_UNEXPECTED_TOKEN;
    return (false);
}

bool duplicate_key_err(t_rt_parser* parser,
                       t_rt_token prev_key,
                       t_rt_token curr) {
    parser->err = RT_ERR_DUPLICATE_KEY;
    parser->err_ref_tok = curr;
    parser->err_ref_tok2 = prev_key;
    return (false);
}

bool try_push_checked_dup(t_rt_parser* parser,
                          t_vec_rt_kv* v,
                          t_rt_kv kv,
                          bool upper_only) {
    t_rt_kv* duplicate;
	t_rt_token dup_tk;
    char first_char = parser->tokenizer.file.contents.buff[kv.k.start_idx];

    duplicate = vec_rt_kv_get(v, parser->tokenizer.file.contents.buff, kv.k);
	if (duplicate && (!upper_only || (first_char >= 'A' && first_char <= 'Z')))
	{
		dup_tk = duplicate->k;
		vec_rt_kv_push(v, kv);
        return (duplicate_key_err(parser, dup_tk, kv.k));
	}
	vec_rt_kv_push(v, kv);
    return (true);
}

bool is_rvalue(enum RT_TT tt) {
    switch (tt) {
        case RT_TUPLE:
        case RT_BOOL:
        case RT_STRING:
        case RT_LBRACE:
        case RT_LBRACKET:
            return true;

        default:
        case RT_RBRACE:
        case RT_IDENT:
        case RT_RBRACKET:
        case RT_COLON:
        case RT_NONE:
        case RT_EOF:
            return false;
    }
}

bool is_leaf(enum RT_TT tt) {
    switch (tt) {
        case RT_TUPLE:
        case RT_BOOL:
        case RT_STRING:
            return true;

        default:
        case RT_LBRACE:
        case RT_LBRACKET:
        case RT_RBRACE:
        case RT_IDENT:
        case RT_RBRACKET:
        case RT_COLON:
        case RT_NONE:
        case RT_EOF:
            return false;
    }
}

enum RT_NODE_TYPE tuple_token_type(t_rt_token t) {
    ft_assert(t.t == RT_TUPLE);
    if (t.is_int)
        return RT_ND_TUPLE_I1 + t.tuple_len - 1;
    return RT_ND_TUPLE_F1 + t.tuple_len - 1;
}

bool parse_leaf(t_rt_parser* parser, t_rt_node* ret) {
    ft_assert(is_leaf(peek_token_type(&parser->tokenizer)));
    *ret = (t_rt_node){.token = consume_token_panic(&parser->tokenizer)};
    if (ret->token.t == RT_STRING)
        ret->t = RT_ND_STRING;
    if (ret->token.t == RT_TUPLE)
        ret->t = tuple_token_type(ret->token);
    if (ret->token.t == RT_BOOL)
        ret->t = RT_ND_BOOL;
    return (true);
}

bool parse_dict(t_rt_parser* parser, t_rt_node* ret);
bool parse_list(t_rt_parser* parser, t_rt_node* ret);

bool parse_node(t_rt_parser* parser, t_rt_node* ret) {
    if (!is_rvalue(peek_token_type(&parser->tokenizer)))
        return (unexpected_err(parser));
    if (is_leaf(peek_token_type(&parser->tokenizer)))
        return parse_leaf(parser, ret);
    if (peek_token_type(&parser->tokenizer) == RT_LBRACE)
        return parse_dict(parser, ret);
    if (peek_token_type(&parser->tokenizer) == RT_LBRACKET)
        return parse_list(parser, ret);
    return (unexpected_err(parser));
}

bool parse_dict(t_rt_parser* parser, t_rt_node* ret) {
    t_rt_kv kv = {0};

    *ret = (t_rt_node){.t = RT_ND_DICT};
    ft_assert(peek_token_type(&parser->tokenizer) == RT_LBRACE);
    ret->token = consume_token_panic(&parser->tokenizer);

    while (peek_token_type(&parser->tokenizer) == RT_IDENT) {
        kv.k = consume_token_panic(&parser->tokenizer);
        if (peek_token_type(&parser->tokenizer) != RT_COLON)
            return (unexpected_err(parser));
        consume_token_panic(&parser->tokenizer);
        if (!is_rvalue(peek_token_type(&parser->tokenizer)))
            return (unexpected_err(parser));
        if (!parse_node(parser, &kv.v)) {
            // to keep track of the possibly allocated things
            vec_rt_kv_push(&ret->dict, kv);
            return (false);
        }
        if (!try_push_checked_dup(parser, &ret->dict, kv, false))
            return false;
    }
    if (peek_token_type(&parser->tokenizer) != RT_RBRACE)
        return unexpected_err(parser);
    consume_token_panic(&parser->tokenizer);
    return (true);
}

bool parse_list(t_rt_parser* parser, t_rt_node* ret) {
    t_rt_node nd;

    *ret = (t_rt_node){.t = RT_ND_LIST};
    ft_assert(peek_token_type(&parser->tokenizer) == RT_LBRACKET);
    ret->token = consume_token_panic(&parser->tokenizer);
    while (is_rvalue(peek_token_type(&parser->tokenizer))) {
        if (!parse_node(parser, &nd)) {
            vec_rt_nd_push(&ret->list, nd);
            return (false);
        }
        vec_rt_nd_push(&ret->list, nd);
    }
    if (peek_token_type(&parser->tokenizer) != RT_RBRACKET)
        return unexpected_err(parser);
    consume_token_panic(&parser->tokenizer);
    return (true);
}

// until the next top level ident
bool parse_old_array(t_rt_parser* parser, t_rt_node* ret) {
    *ret = (t_rt_node){.t = RT_ND_LIST};
    t_rt_token t;
    while (is_rvalue(peek_token_type(&parser->tokenizer))) {
        if (peek_token_type(&parser->tokenizer) == RT_NONE)
            return (false);
        if (peek_token_type(&parser->tokenizer) != RT_TUPLE) {
            parser->err = RT_ERR_NEW_SYNTAX_IN_OLD_CONTEXT;
            return (false);
        }
        t = consume_token_panic(&parser->tokenizer);
        vec_rt_nd_push(&ret->list,
                       (t_rt_node){.t = tuple_token_type(t), .token = t});
    }
    return (true);
}

bool parse_statement(t_rt_parser* parser, t_rt_kv* stmt) {
    t_rt_token tk;

    *stmt = (t_rt_kv){0};
    if (peek_token_type(&parser->tokenizer) != RT_IDENT)
        return unexpected_err(parser);
    consume_token(&parser->tokenizer, &stmt->k);
    if (peek_token_type(&parser->tokenizer) == RT_COLON) {
        consume_token(&parser->tokenizer, &tk);
        if (peek_token_type(&parser->tokenizer) != RT_LBRACE)
            return (unexpected_err(parser));
        return parse_dict(parser, &stmt->v);
    } else {
        return parse_old_array(parser, &stmt->v);
    }
    return (true);
}

bool parse_file(t_rt_parser* parser) {
    bool ret = true;
    while (1) {
        if (peek_token_type(&parser->tokenizer) == RT_EOF)
            break;
        t_rt_kv stmt;
        if (!parse_statement(parser, &stmt)) {
            ret = false;
            if (parser->tokenizer.err) {
                print_tokenizer_err(&parser->tokenizer);
            } else {
                print_parser_err(parser);
            }
            free_node(stmt.v);
            break;
        }
        pprint_node(*parser, stmt.v, 0);
		if (!try_push_checked_dup(parser, &parser->statements, stmt, true))
		{
			print_parser_err(parser);
			ret = false;
			break;
		}
    }
    return ret;
}
