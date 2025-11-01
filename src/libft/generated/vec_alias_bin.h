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
#ifndef VEC_ALIAS_BIN_H
# define VEC_ALIAS_BIN_H
# include "../dsa/vec_gen.h"
# include "../utils/utils.h"

typedef struct s_vec_alias_bin
{
	size_t			cap;
	size_t			len;
	t_alias_bin		*buff;
}	t_vec_alias_bin;

inline static bool	vec_alias_bin_init(
	t_vec_alias_bin *ret, size_t init_alloc)
{
	return (vec_gen_init((t_vec_gen *)ret,
			sizeof(t_alias_bin), init_alloc));
}

inline static bool	vec_alias_bin_push(
	t_vec_alias_bin *v, t_alias_bin el)
{
	return (vec_gen_push((t_vec_gen *)v,
			sizeof(t_alias_bin), &el));
}

inline static t_alias_bin	vec_alias_bin_pop(
	t_vec_alias_bin *v)
{
	t_alias_bin	el;

	ft_assert(vec_gen_pop((t_vec_gen *)v,
			sizeof(t_alias_bin), &el));
	return (el);
}

inline static t_alias_bin	*vec_alias_bin_idx(
	t_vec_alias_bin *v, size_t idx)
{
	return ((t_alias_bin *)vec_gen_idx((t_vec_gen *)v,
			sizeof(t_alias_bin), idx));
}
#endif
