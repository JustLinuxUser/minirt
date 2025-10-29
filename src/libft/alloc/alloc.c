/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 16:38:17 by anddokhn          #+#    #+#             */
/*   Updated: 2025/08/25 16:38:17 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "alloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/utils.h"
#include "mmalloc.h"
#include "../ft_printf/ft_printf.h"

static int	bump_add_arena(t_bump_allocator *alloc, size_t size)
{
	t_bump_arena	*new;

	if (alloc->arena_size == 0)
		size = ft_max(size, 1024);
	size = ft_max(size, alloc->arena_size);
	ft_printf("Allocating a new arena, of size: %zu\n", size);
	new = mmalloc(size + sizeof(t_bump_arena),
			"adding a new arena to a bump allocator");
	new->next = 0;
	new->free_pos = 0;
	new->size = size;
	if (!new)
		return (1);
	if (!alloc->first)
		alloc->first = new;
	if (alloc->last)
		alloc->last->next = new;
	alloc->last = new;
	return (0);
}

// static bool	is_aligned(void *ptr, uintptr_t alignment)
// {
// 	return (((uintptr_t)ptr & ~(-alignment)) == 0);
// }

// static uintptr_t	alignment_of(uint8_t *ptr)
// {
// 	int	i;
//
// 	i = 0;
// 	while (i < 64)
// 	{
// 		if (!is_aligned(ptr, (1ul << i)))
// 			return (1ul << (i - 1));
// 		i++;
// 	}
// 	return (1ul << 63);
// }

static uint8_t	*align_offset(uint8_t *ptr, uintptr_t align)
{
	return ((uint8_t *)((((uintptr_t)ptr) + (align - 1)) & (~(align - 1))));
}

static bool	try_alloc_arena(t_bump_arena *arena, size_t size,
	size_t alignment, void **ret)
{
	uint8_t	*align_ptr;

	if (!arena)
		return (false);
	align_ptr = align_offset(arena->data + arena->free_pos, alignment);
	if (align_ptr + size <= arena->data + arena->size)
	{
		*ret = align_ptr;
		arena->free_pos = (align_ptr - arena->data) + size;
		return (true);
	}
	return (false);
}

void	*__bump_alloc_aligned(t_bump_allocator *alloc, size_t size,
			size_t alignment)
{
	void	*ret;

	ft_assert(alignment && !(alignment & (alignment - 1)));
	if (!try_alloc_arena(alloc->last, size, alignment, &ret))
	{
		if (bump_add_arena(alloc,
				ft_max(alloc->arena_size, size + (alignment & ~(16 - 1)))))
			return (0);
		ft_assert(try_alloc_arena(alloc->last, size, alignment, &ret));
		return (ret);
	}
	return (ret);
}
