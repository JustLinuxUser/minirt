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
#ifndef VEC_OUTCOME_H
# define VEC_OUTCOME_H
# include "../dsa/vec_gen.h"
# include "../utils/utils.h"

typedef struct s_vec_outcome
{
	size_t			cap;
	size_t			len;
	t_outcome		*buff;
}	t_vec_outcome;

inline static bool	vec_outcome_init(
	t_vec_outcome *ret, size_t init_alloc)
{
	return (vec_gen_init((t_vec_gen *)ret,
			sizeof(t_outcome), init_alloc));
}

inline static bool	vec_outcome_push(
	t_vec_outcome *v, t_outcome el)
{
	return (vec_gen_push((t_vec_gen *)v,
			sizeof(t_outcome), &el));
}

inline static t_outcome	vec_outcome_pop(
	t_vec_outcome *v)
{
	t_outcome	el;

	ft_assert(vec_gen_pop((t_vec_gen *)v,
			sizeof(t_outcome), &el));
	return (el);
}

inline static t_outcome	*vec_outcome_idx(
	t_vec_outcome *v, size_t idx)
{
	return ((t_outcome *)vec_gen_idx((t_vec_gen *)v,
			sizeof(t_outcome), idx));
}
#endif
