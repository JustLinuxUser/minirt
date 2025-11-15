/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 20:55:24 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 20:55:50 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "MLX42/MLX42.h"
#include "libft/dsa/dyn_str.h"
#include "libft/ft_printf/ft_printf.h"
#include "rt_utils.h"
#include "error.h"

void	free_zero(void *ptr)
{
	void	**real_ptr;

	real_ptr = (void **)ptr;
	free(*real_ptr);
	*real_ptr = 0;
}

bool	dyn_str_write_file(t_dyn_str buff, char *fname)
{
	int		fd;
	size_t	written;
	int		ret;

	fd = open(fname, O_CREAT | O_TRUNC | O_WRONLY, 0777);
	if (fd < 0)
	{
		print_err();
		ft_eprintf("Opening the file for writing: %s: %s\n",
			fname, strerror(errno));
		return (false);
	}
	written = 0;
	while (1)
	{
		ret = write(fd, buff.buff + written, buff.len - written);
		if (ret < 0)
			return (print_err(),
				ft_printf("Failed to write to file: %s: %s\n", fname,
					strerror(errno)), close(fd), false);
		if (ret <= 0)
			break ;
		written += ret;
	}
	return (close(fd), true);
}

bool	write_image_to_ppm(mlx_image_t *img, char *path)
{
	t_dyn_str	buff;

	buff = (t_dyn_str){0};
	dyn_str_pushstr(&buff, "P3\n");
	dyn_str_pushnbr(&buff, img->width);
	dyn_str_pushstr(&buff, " ");
	dyn_str_pushnbr(&buff, img->height);
	dyn_str_pushstr(&buff, "\n");
	dyn_str_pushnbr(&buff, 255);
	dyn_str_pushstr(&buff, "\n");
	for (int y = 0; y < img->height; y++)
	{
		for (int x = 0; x < img->width; x++)
		{
			uint32_t rgba =
				((uint32_t*)
				img->pixels)[y * img->width + x];
			uint8_t* p = (uint8_t*)&rgba;

			dyn_str_pushnbr(&buff, p[0]);
			dyn_str_pushstr(&buff, " ");
			dyn_str_pushnbr(&buff, p[1]);
			dyn_str_pushstr(&buff, " ");
			dyn_str_pushnbr(&buff, p[2]);
			dyn_str_pushstr(&buff, " ");
		}
		dyn_str_pushstr(&buff, "\n");
	}
	dyn_str_write_file(buff, path);
	free(buff.buff);
}
