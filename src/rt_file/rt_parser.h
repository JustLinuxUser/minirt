/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_parser.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 19:21:38 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/01 19:21:38 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_PARSER_H
# define RT_PARSER_H

# include <stdbool.h>
# include "rt_tokenizer.h"
# include "../minirt.h"

enum e_RT_NODE_TYPE
{
	RT_ND_NONE,
	//
	RT_ND_LIST,
	RT_ND_DICT,
	RT_ND_STRING,
	RT_ND_IDENT,
	// float tuples
	RT_ND_TUPLE_F1,
	RT_ND_TUPLE_F2,
	RT_ND_TUPLE_F3,
	// int tuples
	RT_ND_TUPLE_I1,
	RT_ND_TUPLE_I2,
	RT_ND_TUPLE_I3,
	//
	RT_ND_BOOL,
};

typedef struct s_vec_rt_nd
{
	size_t				cap;
	size_t				len;
	struct s_rt_node	*buff;
}	t_vec_rt_nd;

typedef struct s_vec_rt_kv
{
	size_t			cap;
	size_t			len;
	struct s_rt_kv	*buff;
}	t_vec_rt_kv;

// This is a manual union of all types,
// because real unions are annoying in C
typedef struct s_rt_node
{
	enum e_RT_NODE_TYPE	t;
	bool				used;
	t_rt_token			token;
	//
	t_vec_rt_nd			list;
	//
	t_vec_rt_kv			dict;
}	t_rt_node;

int			vec_rt_nd_init(t_vec_rt_nd *ret);
int			vec_rt_nd_double(t_vec_rt_nd *v);
int			vec_rt_nd_push(t_vec_rt_nd *v, t_rt_node el);
t_rt_node	vec_rt_nd_pop(t_vec_rt_nd *v);
t_rt_node	vec_rt_nd_idx(t_vec_rt_nd *v, size_t idx);

typedef struct s_rt_kv
{
	// an identifier only
	t_rt_token	k;
	t_rt_node	v;
	bool		used;
}	t_rt_kv;

int			vec_rt_kv_init(t_vec_rt_kv *ret);
int			vec_rt_kv_double(t_vec_rt_kv *v);
int			vec_rt_kv_push(t_vec_rt_kv *v, t_rt_kv el);
t_rt_kv		vec_rt_kv_pop(t_vec_rt_kv *v);
t_rt_kv		vec_rt_kv_idx(t_vec_rt_kv *v, size_t idx);
t_rt_kv		*vec_rt_kv_get(t_vec_rt_kv *v, char *buff, t_rt_token t);
t_rt_kv		*vec_rt_kv_get_str(t_vec_rt_kv *v, char *buff, char *str);

typedef struct s_rt_parser
{
	t_rt_tokenizer		tokenizer;
	t_rt_token			err_ref_tok;
	t_rt_token			err_ref_tok2;
	enum e_RT_ERR_TYPE	err;
	t_vec_rt_kv			statements;
}	t_rt_parser;

bool		consume_parsed_nodes(t_rt_parser parser, t_state *state);
void		print_parser_err(t_rt_parser *parser);

void		free_node(t_rt_node nd);
bool		dyn_str_read_file(char *file, t_dyn_str *ret);
bool		dyn_str_write_file(t_dyn_str buff, char *fname);
void		pprint_node(t_rt_parser parser, t_rt_node nd, int indent);
bool		parse_statement(t_rt_parser *parser, t_rt_kv *stmt);
bool		parse_file(t_rt_parser *parser);

bool		parse_dict(t_rt_parser *parser, t_rt_node *ret);
bool		parse_list(t_rt_parser *parser, t_rt_node *ret);
#endif
