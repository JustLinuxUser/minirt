#include "rt_consumer.h"

bool process_file(char* fpath, t_state *state)
{
    t_dyn_str file;
	bool ret = true;

    dyn_str_init(&file);
    if (!dyn_str_read_file(fpath, &file))
		return (false);
	t_rt_parser parser;
    parser = (t_rt_parser){.tokenizer.file.contents = file, .tokenizer.file.fname = fpath};
	if (!parse_file(&parser))
		ret = false;
	if (ret && !consume_parsed_nodes(parser, state))
		ret = false;
	for (size_t i = 0; i < parser.statements.len; i++) {
		free_node(parser.statements.buff[i].v);
	}
	free(parser.statements.buff);
    free(file.buff);
    return (ret);
}
