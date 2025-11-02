/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pprint_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 10:11:20 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 10:11:20 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_parser.h"
#include <stdio.h>
#include "../libft/ft_printf/ft_printf.h"

bool	is_tuple(enum e_RT_NODE_TYPE t)
{
	if (t == RT_ND_TUPLE_F1
		|| t == RT_ND_TUPLE_F2
		|| t == RT_ND_TUPLE_F3
		|| t == RT_ND_TUPLE_I1
		|| t == RT_ND_TUPLE_I2
		|| t == RT_ND_TUPLE_I3)
		return (true);
	return (false);
}

static void	print_indent(size_t indent)
{
	size_t	i;

	i = 0;
	while (i < indent)
	{
		ft_printf("    ");
		i++;
	}
}

void	pprint_dict(t_rt_parser parser, t_rt_node nd, size_t indent)
{
	size_t		i;
	t_rt_token	k;

	if (nd.t == RT_ND_DICT)
	{
		ft_printf("{\n");
		i = 0;
		while (i < nd.dict.len)
		{
			k = nd.dict.buff[i].k;
			print_indent(indent + 1);
			ft_printf("%.*s:\n", k.len,
				parser.tokenizer.file.contents.buff + k.start_idx);
			pprint_node(parser, nd.dict.buff[i].v, indent + 2);
			i++;
		}
	}
}

void	pprint_node(t_rt_parser parser, t_rt_node nd, size_t indent)
{
	size_t		i;

	print_indent(indent);
	if (nd.t == RT_ND_IDENT || nd.t == RT_ND_STRING || is_tuple(nd.t))
	{
		ft_printf("%.*s\n", nd.token.len,
			parser.tokenizer.file.contents.buff + nd.token.start_idx);
	}
	if (nd.t == RT_ND_LIST)
	{
		ft_printf("[\n");
		i = 0;
		while (i < nd.list.len)
			pprint_node(parser, nd.list.buff[i++], indent + 1);
	}
	pprint_dict(parser, nd, indent);
	if (nd.t == RT_ND_LIST || nd.t == RT_ND_DICT)
		print_indent(indent);
	if (nd.t == RT_ND_LIST)
		ft_printf("]\n");
	if (nd.t == RT_ND_DICT)
		ft_printf("}\n");
}
