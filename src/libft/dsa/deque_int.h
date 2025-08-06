/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_int.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 00:32:23 by anddokhn          #+#    #+#             */
/*   Updated: 2024/10/15 04:00:57 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEQUE_INT_H
# define DEQUE_INT_H
# include <stdlib.h>

typedef struct s_deque_int
{
	int		cap;
	int		len;
	int		start;
	int		end;
	int		*buff;
}	t_deque_int;

int		deque_int_init(t_deque_int *ret, int size);
void	deque_int_push_end(t_deque_int *ret, int item);
void	deque_int_push_start(t_deque_int *ret, int item);
int		deque_int_pop_end(t_deque_int *ret);
int		deque_int_pop_start(t_deque_int *ret);
int		deque_int_clone(t_deque_int *ret, const t_deque_int proto);
#endif
