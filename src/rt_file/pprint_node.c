#include "rt_parser.h"
#include <stdio.h>
#include "../libft/libft.h"

bool is_tuple(enum RT_NODE_TYPE t) {
	switch (t) {
        case RT_ND_TUPLE_F1:
        case RT_ND_TUPLE_F2:
        case RT_ND_TUPLE_F3:
        case RT_ND_TUPLE_I1:
        case RT_ND_TUPLE_I2:
        case RT_ND_TUPLE_I3:
			return (true);
		default:
			return (false);
	}
	return (false);
}

void pprint_node(t_rt_parser parser, t_rt_node nd, int indent) {
	for (int i = 0; i < indent; i++) {
		ft_printf("    ");
	}
	if (nd.t == RT_ND_IDENT || nd.t == RT_ND_STRING || is_tuple(nd.t)) {
		ft_printf("%.*s\n", nd.token.len, parser.tokenizer.file.contents.buff + nd.token.start_idx);
	}
	if (nd.t == RT_ND_DICT)
	{
		ft_printf("{\n");
		for (size_t i = 0; i < nd.dict.len; i++) {
			t_rt_token k = nd.dict.buff[i].k;
			for (int i = 0; i < indent + 1; i++) {
				ft_printf("    ");
			}
			ft_printf("%.*s:\n", k.len, parser.tokenizer.file.contents.buff + k.start_idx);
			pprint_node(parser, nd.dict.buff[i].v, indent + 2);
		}
	}
	if (nd.t == RT_ND_LIST)
	{
		ft_printf("[\n");
		for (size_t i = 0; i < nd.list.len; i++) {
			pprint_node(parser, nd.list.buff[i], indent + 1);
		}
	}

	if (nd.t == RT_ND_LIST || nd.t == RT_ND_DICT)
	{
		for (int i = 0; i < indent; i++) {
			ft_printf("    ");
		}
	}
	if (nd.t == RT_ND_LIST)
		ft_printf("]\n");
	if (nd.t == RT_ND_DICT)
		ft_printf("}\n");
}

