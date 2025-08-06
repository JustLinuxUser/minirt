/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:52:15 by anddokhn          #+#    #+#             */
/*   Updated: 2025/04/07 18:17:41 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DYN_STR_H
# define DYN_STR_H
# include <stdbool.h>
# include <stdlib.h>

typedef struct s_dyn_str
{
	size_t	cap;
	size_t	len;
	char	*buff;
}	t_dyn_str;
int		dyn_str_init(t_dyn_str *ret);
int		dyn_str_double(t_dyn_str *v);
int		dyn_str_push(t_dyn_str *v, char el);
char	dyn_str_pop(t_dyn_str *v);
int		dyn_str_idx(t_dyn_str *v, size_t idx);
void	dyn_str_clear(t_dyn_str *v);
int		dyn_str_pushstr(t_dyn_str *v, char *str);
int		dyn_str_pushnstr(t_dyn_str *v, char *str, int len);
void	dyn_str_init_alloc(t_dyn_str *v);
bool	dyn_str_ends_with_str(t_dyn_str *s, char *s2);
#endif
