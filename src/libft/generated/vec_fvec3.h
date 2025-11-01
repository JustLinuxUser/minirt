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
#ifndef VEC_FVEC3_H
# define VEC_FVEC3_H
# include "../dsa/vec_gen.h"
# include "../utils/utils.h"

typedef struct s_vec_fvec3
{
	size_t		cap;
	size_t		len;
	t_fvec3		*buff;
}	t_vec_fvec3;

inline static bool	vec_fvec3_init(
	t_vec_fvec3 *ret, size_t init_alloc)
{
	return (vec_gen_init((t_vec_gen *)ret,
			sizeof(t_fvec3), init_alloc));
}

inline static bool	vec_fvec3_push(
	t_vec_fvec3 *v, t_fvec3 el)
{
	return (vec_gen_push((t_vec_gen *)v,
			sizeof(t_fvec3), &el));
}

inline static t_fvec3	vec_fvec3_pop(
	t_vec_fvec3 *v)
{
	t_fvec3	el;

	ft_assert(vec_gen_pop((t_vec_gen *)v,
			sizeof(t_fvec3), &el));
	return (el);
}

inline static t_fvec3	*vec_fvec3_idx(
	t_vec_fvec3 *v, size_t idx)
{
	return ((t_fvec3 *)vec_gen_idx((t_vec_gen *)v,
			sizeof(t_fvec3), idx));
}
#endif
