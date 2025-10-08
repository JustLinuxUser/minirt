/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partition.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 15:17:34 by anddokhn          #+#    #+#             */
/*   Updated: 2025/09/10 15:17:34 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTITION_H
# define PARTITION_H
# include <stddef.h>
# include <stdbool.h>

typedef bool	(*t_separate_fn)(void *el, void *arg);
typedef struct s_partition_args
{
	void			*data;
	size_t			len;
	size_t			el_size;
	t_separate_fn	sep;
	void			*arg;
}	t_partition_args;
void	*partition(t_partition_args args);
#endif
