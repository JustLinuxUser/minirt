/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smoothsort2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 15:40:57 by anddokhn          #+#    #+#             */
/*   Updated: 2025/09/10 15:40:57 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "smoothsort.h"
#include <stdint.h>

void	*max_top_3(t_sort_state *state, uint8_t *arr, int tree_order)
{
	void	*curr_right;
	int		right_order;
	void	*curr_left;

	if (tree_order <= 1)
		return (arr);
	curr_right = arr - 1 * state->args->el_size;
	right_order = tree_order - 2;
	curr_left = arr - (1 + state->lns[right_order])
		* state->args->el_size;
	if (state->args->cmp(curr_left, curr_right, state->args->arg) > 0)
	{
		if (state->args->cmp(curr_left, arr, state->args->arg) > 0)
			return (curr_left);
		else
			return (arr);
	}
	if (state->args->cmp(curr_right, arr, state->args->arg) > 0)
		return (curr_right);
	else
		return (arr);
}
