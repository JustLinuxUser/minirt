/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_file.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 12:37:47 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/01 12:37:47 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_consumer.h"
#include <stddef.h>

bool	process_file(char *fpath, t_state *state)
{
	t_dyn_str	file;
	bool		ret;
	t_rt_parser	parser;
	size_t		i;

	ret = true;
	dyn_str_init(&file);
	if (!dyn_str_read_file(fpath, &file))
		return (false);
	parser = (t_rt_parser){.tokenizer.file.contents = file,
		.tokenizer.file.fname = fpath};
	if (!parse_file(&parser))
		ret = false;
	if (ret && !consume_parsed_nodes(parser, state))
		ret = false;
	i = 0;
	while (i < parser.statements.len)
	{
		free_node(parser.statements.buff[i].v);
		i++;
	}
	free(parser.statements.buff);
	free(file.buff);
	return (ret);
}
