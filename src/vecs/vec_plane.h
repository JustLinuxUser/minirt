/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_plane.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:52:15 by anddokhn          #+#    #+#             */
/*   Updated: 2025/01/14 00:34:37 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC_PLANE_H
# define VEC_PLANE_H
# include <stdlib.h>
typedef int t_plane;

typedef struct s_vec_plane
{
	size_t	cap;
	size_t	len;
	t_plane		*buff;
}	t_vec_plane;
int	vec_plane_init(t_vec_plane *ret);
int	vec_plane_double(t_vec_plane *v);
int	vec_plane_push(t_vec_plane *v, t_plane el);
t_plane	vec_plane_pop(t_vec_plane *v);
t_plane	vec_plane_idx(t_vec_plane *v, size_t idx);
#endif
