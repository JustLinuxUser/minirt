/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:42:44 by anddokhn          #+#    #+#             */
/*   Updated: 2025/01/23 19:12:20 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif
# include <stdlib.h>
# include <sys/types.h>

typedef struct s_read_buffer
{
	unsigned char	*buff;
	size_t			len;
	int				finished;
}	t_read_buffer;

typedef struct s_deque
{
	unsigned char	*buff;
	int				len;
	int				cap;
	int				start;
	int				end;
	int				initialized;
	int				fd;
	int				finished;
	int				errored;
}	t_deque;

typedef struct s_buff_line_reader
{
	t_deque	*buff;
	int		len;
	int		cap;
	int		initialized;
}	t_buff_line_reader;

char			*get_next_line(int fd);

int				deque_init(t_deque *ret, size_t size);
int				deque_find_newline(t_deque q);
int				deque_get_line(t_deque *q, char **ret);
int				deque_push(t_deque *q, char c);
#endif
