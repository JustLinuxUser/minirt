/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*                                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:11:48 by anddokhn          #+#    #+#             */
/*   Updated: 2025/04/08 22:26:02 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef VEC_INT_H
# define VEC_INT_H
# include "../dsa/vec_gen.h"
# include "../utils/utils.h"

typedef struct s_vec_int
{
	size_t	cap;
	size_t	len;
	int		*buff;
}	t_vec_int;

inline static bool	vec_int_init(
	t_vec_int *ret, size_t init_alloc)
{
	return (vec_gen_init((t_vec_gen *)ret,
			sizeof(int), init_alloc));
}

inline static bool	vec_int_push(
	t_vec_int *v, int el)
{
	return (vec_gen_push((t_vec_gen *)v,
			sizeof(int), &el));
}

inline static int	vec_int_pop(
	t_vec_int *v)
{
	int	el;

	ft_assert(vec_gen_pop((t_vec_gen *)v,
			sizeof(int), &el));
	return (el);
}

inline static int	*vec_int_idx(
	t_vec_int *v, size_t idx)
{
	return ((int *)vec_gen_idx((t_vec_gen *)v,
			sizeof(int), idx));
}
#endif
