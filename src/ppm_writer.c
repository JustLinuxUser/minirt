/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppm_writer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 17:28:15 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/19 17:28:15 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "libft/dsa/dyn_str.h"
#include "libft/ft_printf/ft_printf.h"
#include "rt_utils.h"

static void	ppm_header(t_dyn_str *buff, mlx_image_t *img)
{
	*buff = (t_dyn_str){0};
	dyn_str_pushstr(buff, "P3\n");
	dyn_str_pushnbr(buff, img->width);
	dyn_str_pushstr(buff, " ");
	dyn_str_pushnbr(buff, img->height);
	dyn_str_pushstr(buff, "\n");
	dyn_str_pushnbr(buff, 255);
	dyn_str_pushstr(buff, "\n");
}

void	write_image_to_ppm(mlx_image_t *img, char *path)
{
	t_dyn_str	buff;
	int			x;
	int			y;
	uint8_t		*p;

	ppm_header(&buff, img);
	y = -1;
	while ((size_t)++y < img->height)
	{
		x = -1;
		while ((size_t)++x < img->width)
		{
			p = (uint8_t *)&((uint32_t *)img->pixels)[y * img->width + x];
			dyn_str_pushnbr(&buff, p[0]);
			dyn_str_pushstr(&buff, " ");
			dyn_str_pushnbr(&buff, p[1]);
			dyn_str_pushstr(&buff, " ");
			dyn_str_pushnbr(&buff, p[2]);
			dyn_str_pushstr(&buff, " ");
		}
		dyn_str_pushstr(&buff, "\n");
	}
	ft_eprintf("Writing the render output to an image: \"%s\"\n", path);
	dyn_str_write_file(buff, path);
	free(buff.buff);
}
