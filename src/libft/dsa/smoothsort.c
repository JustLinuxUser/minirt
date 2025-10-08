/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smoothsort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 12:09:40 by anddokhn          #+#    #+#             */
/*   Updated: 2025/09/07 12:09:40 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "smoothsort.h"
#include "../libft.h"

static void	leo_init(t_sort_state *state)
{
	size_t	i;

	state->lns[0] = 1;
	state->lns[1] = 1;
	i = 2;
	while (i < sizeof(state->lns) / sizeof(state->lns[0]))
	{
		state->lns[i] = state->lns[i - 1]
			+ state->lns[i - 2] + 1;
		i++;
	}
}

static void	heapify_tree(t_sort_state *state, uint8_t *arr, int tree_order)
{
	void	*chld[2];
	int		chld_ord[2];

	while (tree_order > 1)
	{
		chld[1] = arr - 1 * state->args->el_size;
		chld_ord[1] = tree_order - 2;
		chld[0] = arr - (1 + state->lns[chld_ord[1]]) * state->args->el_size;
		chld_ord[0] = tree_order - 1;
		if (state->args->cmp(chld[0], chld[1], state->args->arg) >= 0
			&& state->args->cmp(chld[0], arr, state->args->arg) > 0)
		{
			ft_memswap(chld[0], arr, state->args->el_size);
			arr = chld[0];
			tree_order = chld_ord[0];
		}
		else if (state->args->cmp(chld[1], arr, state->args->arg) > 0)
		{
			ft_memswap(chld[1], arr, state->args->el_size);
			arr = chld[1];
			tree_order = chld_ord[1];
		}
		else
			break ;
	}
}

static void	order_roots(t_sort_state *state, uint8_t *curr_offs)
{
	int		i;
	void	*curr;
	void	*next_offs;

	i = state->ntrees;
	while (--i > 0)
	{
		curr = max_top_3(state, curr_offs, state->tree_orders[i]);
		next_offs = curr_offs - state->args->el_size
			* state->lns[state->tree_orders[i]];
		if (state->args->cmp(next_offs, curr, state->args->arg) <= 0)
			break ;
		ft_memswap(curr_offs, next_offs, state->args->el_size);
		heapify_tree(state, curr_offs, state->tree_orders[i]);
		curr_offs = next_offs;
	}
	heapify_tree(state, curr_offs, state->tree_orders[i]);
}

static void	build_heap(t_sort_state *state)
{
	while (state->pos < state->args->arr_len)
	{
		if (state->ntrees >= 2 && state->tree_orders[state->ntrees - 1] + 1
			== state->tree_orders[state->ntrees - 2])
		{
			state->ntrees--;
			state->tree_orders[state->ntrees - 1]
				= state->tree_orders[state->ntrees - 1] + 1;
		}
		else if
		(state->ntrees >= 1 && state->tree_orders[state->ntrees - 1] == 1)
			state->tree_orders[state->ntrees++] = 0;
		else
			state->tree_orders[state->ntrees++] = 1;
		order_roots(state, (uint8_t *)state->args->arr
			+ (state->pos * state->args->el_size));
		state->pos++;
	}
	state->pos--;
}

void	my_smoothsort(t_sort_args args)
{
	t_sort_state	state;

	state = (t_sort_state){.args = &args};
	leo_init(&state);
	build_heap(&state);
	while (state.pos > 0)
	{
		state.pos--;
		if (state.tree_orders[state.ntrees - 1] <= 1)
		{
			state.ntrees--;
			continue ;
		}
		state.tree_orders[state.ntrees - 1]
			= state.tree_orders[state.ntrees - 1] - 1;
		order_roots(&state, (uint8_t *)args.arr + (state.pos
				- state.lns[state.tree_orders[state.ntrees - 1] - 1])
			* state.args->el_size);
		state.tree_orders[state.ntrees]
			= state.tree_orders[state.ntrees - 1] - 1;
		state.ntrees++;
		order_roots(&state, (uint8_t *)args.arr + state.pos
			* state.args->el_size);
	}
}
