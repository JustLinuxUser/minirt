/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:42:56 by anddokhn          #+#    #+#             */
/*   Updated: 2025/08/25 17:42:56 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALLOC_H
# define ALLOC_H
# include <stddef.h>
# include <stdint.h>

# ifndef DEBUG_ALLOC
#  define DEBUG_ALLOC 0
# else
#  undef DEBUG_ALLOC
#  define DEBUG_ALLOC 1
# endif

typedef struct s_bump_arena
{
	struct s_bump_arena	*next;
	size_t				size;
	size_t				free_pos;
	uint8_t				data[];
}	t_bump_arena;

typedef struct s_bump_allocator
{
	size_t			arena_size;
	t_bump_arena	*first;
	t_bump_arena	*last;
}	t_bump_allocator;

void	*__bump_alloc_aligned(t_bump_allocator *alloc, size_t size,
			size_t alignment);
void	bump_alloc_free(t_bump_allocator *alloc);
void	*bump_alloc_aligned(t_bump_allocator *alloc,
			size_t size, size_t alignment);
void	*bump_alloc(t_bump_allocator *alloc, size_t size);
#endif
