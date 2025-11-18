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
#include "minirt.h"
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
				ft_eprintf("Failed to write to file: %s: %s\n", fname,
					strerror(errno)), close(fd), false);
		if (ret <= 0)
			break ;
		written += ret;
	}
	return (close(fd), true);
}

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

void	free_state(t_state *state)
{
	size_t	i;

	free_zero(&state->unbounded_shapes.buff);
	free_zero(&state->shapes.buff);
	free_zero(&state->triangles.buff);
	i = 0;
	while (i < state->meshes.len)
	{
		free_zero(&state->meshes.buff[i].vertex_idxs.buff);
		free_zero(&state->meshes.buff[i].vertices.buff);
		i++;
	}
	free_zero(&state->meshes.buff);
	free_zero(&state->planes.buff);
	free_zero(&state->spheres.buff);
	free_zero(&state->cylinders.buff);
	free_zero(&state->s_colors);
	free_zero(&state->spectrums.buff);
	if (state->mlx_image)
		mlx_delete_image(state->mlx, state->mlx_image);
	if (state->mlx)
		mlx_terminate(state->mlx);
	free_zero(&state->output_path.buff);
}
