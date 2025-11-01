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
#ifndef VEC_OBJ_TOKEN_H
# define VEC_OBJ_TOKEN_H
# include "../dsa/vec_gen.h"
# include "../utils/utils.h"

typedef struct s_vec_obj_token
{
	size_t			cap;
	size_t			len;
	t_obj_token		*buff;
}	t_vec_obj_token;

inline static bool	vec_obj_token_init(
	t_vec_obj_token *ret, size_t init_alloc)
{
	return (vec_gen_init((t_vec_gen *)ret,
			sizeof(t_obj_token), init_alloc));
}

inline static bool	vec_obj_token_push(
	t_vec_obj_token *v, t_obj_token el)
{
	return (vec_gen_push((t_vec_gen *)v,
			sizeof(t_obj_token), &el));
}

inline static t_obj_token	vec_obj_token_pop(
	t_vec_obj_token *v)
{
	t_obj_token	el;

	ft_assert(vec_gen_pop((t_vec_gen *)v,
			sizeof(t_obj_token), &el));
	return (el);
}

inline static t_obj_token	*vec_obj_token_idx(
	t_vec_obj_token *v, size_t idx)
{
	return ((t_obj_token *)vec_gen_idx((t_vec_gen *)v,
			sizeof(t_obj_token), idx));
}
#endif
