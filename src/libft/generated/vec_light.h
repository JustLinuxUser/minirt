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
#ifndef VEC_LIGHT_H
# define VEC_LIGHT_H
# include "../dsa/vec_gen.h"
# include "../utils/utils.h"

typedef struct s_vec_light
{
	size_t		cap;
	size_t		len;
	t_light		*buff;
}	t_vec_light;

inline static bool	vec_light_init(
	t_vec_light *ret, size_t init_alloc)
{
	return (vec_gen_init((t_vec_gen *)ret,
			sizeof(t_light), init_alloc));
}

inline static bool	vec_light_push(
	t_vec_light *v, t_light el)
{
	return (vec_gen_push((t_vec_gen *)v,
			sizeof(t_light), &el));
}

inline static t_light	vec_light_pop(
	t_vec_light *v)
{
	t_light	el;

	ft_assert(vec_gen_pop((t_vec_gen *)v,
			sizeof(t_light), &el));
	return (el);
}

inline static t_light	*vec_light_idx(
	t_vec_light *v, size_t idx)
{
	return ((t_light *)vec_gen_idx((t_vec_gen *)v,
			sizeof(t_light), idx));
}
#endif
