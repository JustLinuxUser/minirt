/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 16:56:14 by anddokhn          #+#    #+#             */
/*   Updated: 2025/08/26 16:56:14 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "alloc.h"
#include "mmalloc.h"

void	*bump_alloc_aligned(t_bump_allocator *alloc,
			size_t size, size_t alignment)
{
	void	*ret;
	void	**allocated_ptr;

	if (DEBUG_ALLOC)
	{
		ret = mmalloc(size, "creating a new debug allocation for a bump alloc");
		allocated_ptr = __bump_alloc_aligned(alloc, sizeof(void *), 8);
		*allocated_ptr = ret;
		return (ret);
	}
	return (__bump_alloc_aligned(alloc, size, alignment));
}

void	*bump_alloc(t_bump_allocator *alloc, size_t size)
{
	return (bump_alloc_aligned(alloc, size, 16));
}

// void print_bits(uintptr_t bits)
// {
// 	for (intptr_t i = sizeof(bits) * 8 - 1; i >= 0; i--) {
// 		if ((1ul << i) & bits)
// 			printf("1");
// 		else
// 			printf("0");
// 		if (i % 8 == 0)
// 			printf(" ");
// 	}
// 	printf("\n");
// }

void	bump_alloc_free(t_bump_allocator *alloc)
{
	t_bump_arena	*curr;
	t_bump_arena	*temp;
	size_t			i;

	curr = alloc->first;
	while (curr)
	{
		temp = curr->next;
		if (DEBUG_ALLOC)
		{
			i = 0;
			while (i * 8 < curr->free_pos)
			{
				free(*(void **)(curr->data + (i * 8)));
				i++;
			}
		}
		free(curr);
		curr = temp;
	}
	alloc->first = 0;
	alloc->last = 0;
}
