/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_utils.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:27:42 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/05 14:27:42 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_UTILS_H
# define RT_UTILS_H
# include "MLX42/MLX42.h"
# include "libft/dsa/dyn_str.h"

void	free_zero(void *ptr);
void	write_image_to_ppm(mlx_image_t *img, char *path);
bool	dyn_str_write_file(t_dyn_str buff, char *fname);
#endif
