#include "rt_consumer.h"

bool process_file(char* fpath, t_state *state)
{
    t_dyn_str file;
	bool ret = true;

    dyn_str_init(&file);
    if (!dyn_str_read_file(fpath, &file))
		return (false);
	t_rt_parser parser = {.tokenizer = {.file = file}};
	while (1)
	{
		t_rt_kv stmt;
		if (!parse_statement(&parser, &stmt)) {
			ret = false;
			if (parser.tokenizer.err) {
				print_tokenizer_err(&parser.tokenizer);
			} else {
				print_parser_err(&parser);
			}
			free_node(stmt.v);
			break;
		}
		pprint_node(parser, stmt.v, 0);
		vec_rt_kv_push(&parser.statements, stmt);
		if (peek_token_type(&parser.tokenizer) == RT_EOF)
			break;
	}
	if (ret && !consume_parsed_nodes(parser, state))
		ret = false;
	for (size_t i = 0; i < parser.statements.len; i++) {
		free_node(parser.statements.buff[i].v);
	}
	free(parser.statements.buff);
    free(file.buff);
    return (ret);
}
