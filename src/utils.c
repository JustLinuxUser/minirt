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

void	free_state2(t_state *state)
{
	free_zero(&state->unbounded_shapes.buff);
	free_zero(&state->shapes.buff);
	free_zero(&state->triangles.buff);
	free_zero(&state->bvh);
	free_zero(&state->lights.bins.buff);
	free_zero(&state->lights.lights.buff);
	free_zero(&state->lights.pdfs.buff);
}

void	free_state(t_state *state)
{
	size_t	i;

	free_state2(state);
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
	free_zero(&state->img_buffer);
	if (state->mlx_image)
		mlx_delete_image(state->mlx, state->mlx_image);
	state->mlx_image = 0;
	if (state->mlx)
		mlx_terminate(state->mlx);
	state->mlx = 0;
	free_zero(&state->output_path.buff);
	ft_eprintf("Finished exiting...\n");
}
