/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_loader.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 15:06:04 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/01 15:06:04 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJ_LOADER_H
# define OBJ_LOADER_H
# include "mymath.h"
# include <stdbool.h>

typedef struct s_obj_spec
{
	char	*path;
	t_fvec3	pos;
	float	scale;
	t_fvec2	rotation;
	int		spectrum_idx;
	bool	forward_z;
}	t_obj_spec;
#endif
