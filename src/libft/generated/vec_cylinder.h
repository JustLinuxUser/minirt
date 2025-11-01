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
#ifndef VEC_CYLINDER_H
# define VEC_CYLINDER_H
# include "../dsa/vec_gen.h"
# include "../utils/utils.h"

typedef struct s_vec_cylinder
{
	size_t			cap;
	size_t			len;
	t_cylinder		*buff;
}	t_vec_cylinder;

inline static bool	vec_cylinder_init(
	t_vec_cylinder *ret, size_t init_alloc)
{
	return (vec_gen_init((t_vec_gen *)ret,
			sizeof(t_cylinder), init_alloc));
}

inline static bool	vec_cylinder_push(
	t_vec_cylinder *v, t_cylinder el)
{
	return (vec_gen_push((t_vec_gen *)v,
			sizeof(t_cylinder), &el));
}

inline static t_cylinder	vec_cylinder_pop(
	t_vec_cylinder *v)
{
	t_cylinder	el;

	ft_assert(vec_gen_pop((t_vec_gen *)v,
			sizeof(t_cylinder), &el));
	return (el);
}

inline static t_cylinder	*vec_cylinder_idx(
	t_vec_cylinder *v, size_t idx)
{
	return ((t_cylinder *)vec_gen_idx((t_vec_gen *)v,
			sizeof(t_cylinder), idx));
}
#endif
