/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 19:38:10 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/01 19:38:10 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_parser.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "../libft/ft_printf/ft_printf.h"
#include "../error.h"

void	free_node(t_rt_node nd)
{
	size_t	i;

	i = 0;
	while (i < nd.list.len)
		free_node(nd.list.buff[i++]);
	while (i < nd.dict.len)
		free_node(nd.dict.buff[i++].v);
	free(nd.list.buff);
	free(nd.dict.buff);
}

bool	dyn_str_read_file(char *file, t_dyn_str *ret)
{
	char	buff[1024];
	int		len;
	int		fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		print_err();
		ft_eprintf("Opening the file for reading: %s: %s\n", file,
			strerror(errno));
		return (false);
	}
	while (1)
	{
		len = read(fd, buff, sizeof(buff));
		if (len == 0)
			break ;
		if (len > 0)
			dyn_str_pushnstr(ret, buff, len);
		else
			return (print_err(),
				ft_printf("Failed to read the file: %s: %s\n", file,
					strerror(errno)), close(fd), false);
	}
	return (close(fd), true);
}

bool	dyn_str_write_file(t_dyn_str buff, char *fname)
{
	int		fd;
	size_t	written;
	int		ret;

	fd = open(fname, O_CREAT | O_TRUNC | O_WRONLY, 0777);
	if (fd < 0)
	{
		print_err();
		ft_eprintf("Opening the file for writing: %s: %s\n",
			fname, strerror(errno));
		return (false);
	}
	written = 0;
	while (1)
	{
		ret = write(fd, buff.buff + written, buff.len - written);
		if (ret < 0)
			return (print_err(),
				ft_printf("Failed to write to file: %s: %s\n", fname,
					strerror(errno)), close(fd), false);
		if (ret <= 0)
			break ;
		written += ret;
	}
	return (close(fd), true);
}

char	*token_name(enum e_RT_TT tt)
{
	if (tt == RT_NONE)
		return ("RT_NONE");
	if (tt == RT_IDENT)
		return ("RT_IDENT");
	if (tt == RT_TUPLE)
		return ("RT_TUPLE");
	if (tt == RT_BOOL)
		return ("RT_BOOL");
	if (tt == RT_STRING)
		return ("RT_STRING");
	if (tt == RT_LBRACE)
		return ("RT_LBRACE");
	if (tt == RT_RBRACE)
		return ("RT_RBRACE");
	if (tt == RT_LBRACKET)
		return ("RT_LBRACKET");
	if (tt == RT_RBRACKET)
		return ("RT_RBRACKET");
	if (tt == RT_COLON)
		return ("RT_COLON");
	if (tt == RT_EOF)
		return ("RT_EOF");
	return (0);
}
