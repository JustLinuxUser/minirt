/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_rt_kv2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 09:54:07 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 09:54:07 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_parser.h"
#include "../libft/libft.h"

t_rt_kv	*vec_rt_kv_get(t_vec_rt_kv *v, char *buff, t_rt_token t)
{
	size_t	i;

	i = 0;
	while (i < v->len)
	{
		if (t.len && t.len == v->buff[i].k.len
			&& ft_strncmp(t.start_idx + buff,
				v->buff[i].k.start_idx + buff, t.len) == 0)
			return (v->buff + i);
		i++;
	}
	return (0);
}

t_rt_kv	*vec_rt_kv_get_str(t_vec_rt_kv *v, char *buff, char *str)
{
	size_t	i;
	int		len;

	i = 0;
	len = ft_strlen(str);
	while (i < v->len)
	{
		if (len && len == v->buff[i].k.len
			&& ft_strncmp(str, v->buff[i].k.start_idx + buff, len) == 0)
			return (v->buff + i);
		i++;
	}
	return (0);
}
