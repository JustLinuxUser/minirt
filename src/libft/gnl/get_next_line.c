/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 23:34:34 by anddokhn          #+#    #+#             */
/*   Updated: 2024/10/13 03:50:37 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "get_next_line.h"

int	read_to_buff(t_deque *buff, int fd)
{
	int		res;
	char	*r_buff;
	int		i;

	r_buff = (char [1000]){0};
	if (BUFFER_SIZE > 1000)
		r_buff = malloc(BUFFER_SIZE);
	if (r_buff == 0)
		return (1);
	res = read(fd, r_buff, BUFFER_SIZE);
	i = -1;
	while (++i < res)
	{
		if (deque_push(buff, r_buff[i]))
		{
			if (BUFFER_SIZE > 1000)
				free(r_buff);
			return (1);
		}
	}
	buff->finished = res <= 0;
	buff->errored = res < 0;
	if (BUFFER_SIZE > 1000)
		free(r_buff);
	return (0);
}

int	get_next_line_buff(char **ret, int fd, t_deque *buff)
{
	int				error;

	if (!buff->initialized)
		if (deque_init(buff, 10))
			return (1);
	buff->fd = fd;
	error = 0;
	while (deque_find_newline(*buff) == -1 && !buff->finished)
	{
		error = read_to_buff(buff, fd) || error;
		if (error)
			break ;
	}
	if (!error && !buff->errored)
		error = deque_get_line(buff, ret) || error;
	else
		*ret = 0;
	if (buff->len == 0 || error || buff->errored)
	{
		free(buff->buff);
		*buff = (t_deque){.fd = -1};
	}
	return (error);
}

void	*resize_buff(void *b, int *nmemb, int el_size)
{
	int		i;
	char	*new;

	i = 0;
	new = malloc((*nmemb * 2 + 1) * el_size);
	if (new == 0)
		return (0);
	if (b)
	{
		while (i < *nmemb * el_size)
		{
			new[i] = ((char *)b)[i];
			i++;
		}
	}
	while (i < (*nmemb * 2 + 1) * el_size)
		new[i++] = 0;
	*nmemb *= 2;
	*nmemb += 1;
	return (new);
}

int	get_next_line_not_static(char **ret, t_buff_line_reader *reader, int fd)
{
	int		i;
	int		fi;
	t_deque	*temp;

	i = -1;
	fi = -1;
	while (++i < reader->len)
		if ((fi == -1 && (reader->buff[i].fd == -1 || reader->buff[i].fd == fd))
			|| (fi != -1 && reader->buff[i].fd == fd))
			fi = i;
	if (fi != -1)
		return (get_next_line_buff(ret, fd, &reader->buff[fi]));
	if (reader->len == reader->cap)
	{
		temp = resize_buff(reader->buff, &reader->cap, sizeof(t_deque));
		if (temp == 0)
			return (1);
		free(reader->buff);
		reader->buff = temp;
		reader->initialized = 1;
	}
	fi = reader->len++;
	reader->buff[fi].initialized = 0;
	return (get_next_line_buff(ret, fd, &reader->buff[fi]));
}

char	*get_next_line(int fd)
{
	char						*ret;
	int							i;
	int							total_used;
	int							exit_code;
	static t_buff_line_reader	reader = {0};

	ret = 0;
	exit_code = get_next_line_not_static(&ret, &reader, fd);
	total_used = 0;
	i = -1;
	while (++i < reader.len)
		if (reader.buff[i].initialized)
			total_used++;
	if (exit_code || total_used == 0)
	{
		i = -1;
		while (++i < reader.len)
			if (reader.buff[i].initialized)
				free(reader.buff[i].buff);
		if (reader.buff)
			free(reader.buff);
		reader = (t_buff_line_reader){0};
	}
	return (ret);
}
