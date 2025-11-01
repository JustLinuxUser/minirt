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
#ifndef VEC_DENSELY_SAMPLED_SPECTRUM_H
# define VEC_DENSELY_SAMPLED_SPECTRUM_H
# include "../dsa/vec_gen.h"
# include "../utils/utils.h"

typedef struct s_vec_densely_sampled_spectrum
{
	size_t							cap;
	size_t							len;
	t_densely_sampled_spectrum		*buff;
}	t_vec_densely_sampled_spectrum;

inline static bool	vec_densely_sampled_spectrum_init(
	t_vec_densely_sampled_spectrum *ret, size_t init_alloc)
{
	return (vec_gen_init((t_vec_gen *)ret,
			sizeof(t_densely_sampled_spectrum), init_alloc));
}

inline static bool	vec_densely_sampled_spectrum_push(
	t_vec_densely_sampled_spectrum *v, t_densely_sampled_spectrum el)
{
	return (vec_gen_push((t_vec_gen *)v,
			sizeof(t_densely_sampled_spectrum), &el));
}

inline static t_densely_sampled_spectrum	vec_densely_sampled_spectrum_pop(
	t_vec_densely_sampled_spectrum *v)
{
	t_densely_sampled_spectrum	el;

	ft_assert(vec_gen_pop((t_vec_gen *)v,
			sizeof(t_densely_sampled_spectrum), &el));
	return (el);
}

inline static t_densely_sampled_spectrum	*vec_densely_sampled_spectrum_idx(
	t_vec_densely_sampled_spectrum *v, size_t idx)
{
	return ((t_densely_sampled_spectrum *)vec_gen_idx((t_vec_gen *)v,
			sizeof(t_densely_sampled_spectrum), idx));
}
#endif
