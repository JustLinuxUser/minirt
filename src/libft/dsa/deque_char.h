/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_char.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 00:32:23 by anddokhn          #+#    #+#             */
/*   Updated: 2025/02/05 00:22:40 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEQUE_CHAR_H
# define DEQUE_CHAR_H
# include <stdlib.h>

typedef struct s_deque_char
{
	int		cap;
	int		len;
	int		start;
	int		end;
	char	*buff;
}	t_deque_char;

int		deque_char_init(t_deque_char *ret, int size);
void	deque_char_push_end(t_deque_char *ret, char item);
void	deque_char_push_start(t_deque_char *ret, char item);
char	deque_char_pop_end(t_deque_char *ret);
char	deque_char_pop_start(t_deque_char *ret);
int		deque_char_clone(t_deque_char *ret, const t_deque_char proto);
void	deque_char_double_if_needed(t_deque_char *ret);
#endif
