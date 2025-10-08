/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smoothsort.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 12:09:28 by anddokhn          #+#    #+#             */
/*   Updated: 2025/09/07 12:09:28 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SMOOTHSORT_H
# define SMOOTHSORT_H
# include <stddef.h>
# include <stdint.h>

typedef int	(*t_cmpfun)(const void *, const void *, void *);

typedef struct s_sort_args
{
	void		*arr;
	size_t		arr_len;
	size_t		el_size;
	t_cmpfun	cmp;
	void		*arg;
}	t_sort_args;

typedef struct s_sort_state
{
	t_sort_args	*args;
	size_t		ntrees;
	size_t		lns[100];
	size_t		tree_orders[50];
	size_t		pos;
}	t_sort_state;
void	my_smoothsort(t_sort_args args);
void	*max_top_3(t_sort_state *state, uint8_t *arr, int tree_order);
#endif
