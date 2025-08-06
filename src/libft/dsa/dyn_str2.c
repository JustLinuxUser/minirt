/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 23:12:21 by anddokhn          #+#    #+#             */
/*   Updated: 2025/04/19 20:09:25 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_str.h"
#include "../libft.h"

void	dyn_str_clear(t_dyn_str *v)
{
	v->len = 0;
}

int	dyn_str_pushstr(t_dyn_str *v, char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (dyn_str_push(v, str[i]))
			return (1);
	return (0);
}

int	dyn_str_pushnstr(t_dyn_str *v, char *str, int len)
{
	int	i;

	i = -1;
	while (++i < len && str[i])
		if (dyn_str_push(v, str[i]))
			return (1);
	return (0);
}

bool	dyn_str_ends_with_str(t_dyn_str *s, char *s2)
{
	size_t	len_s2;

	len_s2 = ft_strlen(s2);
	if (s->len < len_s2)
		return (false);
	if (ft_strcmp(s->buff + s->len - len_s2, s2) == 0)
		return (true);
	return (false);
}

void	dyn_str_init_alloc(t_dyn_str *v)
{
	dyn_str_init(v);
	dyn_str_push(v, '\0');
	v->len--;
}
