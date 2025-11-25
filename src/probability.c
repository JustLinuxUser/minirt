/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   probability.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:07:28 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 19:54:37 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "light_structs.h"
#include "minirt.h"
#include "probability.h"
#include "libft/generated/vec_outcome.h"

static void	init_outcome_arrays(t_lights *lights,
		t_vec_outcome *over, t_vec_outcome *under)
{
	size_t		i;
	t_outcome	outcome;

	i = 0;
	vec_outcome_init(under, lights->lights.len);
	vec_outcome_init(over, lights->lights.len);
	while (i < lights->lights.len)
	{
		outcome = (t_outcome){.index = i,
			.p_hat = lights->bins.buff[i].prob};
		if (lights->pdfs.buff[i] * lights->lights.len
			> 1.f)
			vec_outcome_push(over, outcome);
		else
			vec_outcome_push(under, outcome);
		i++;
	}
}

static void	init_bins(t_lights *lights)
{
	size_t	i;

	i = 0;
	while (i < lights->lights.len)
	{
		vec_alias_bin_push(&lights->bins,
			(t_alias_bin){.prob = (float)lights->lights.len
			* lights->pdfs.buff[i]});
		i++;
	}
}

static void	handle_remaining_outcomes(t_lights *lights,
		t_vec_outcome *under, t_vec_outcome *over)
{
	t_outcome		un;
	t_outcome		ov;

	while (under->len > 0)
	{
		un = vec_outcome_pop(under);
		lights->bins.buff[un.index].prob = un.p_hat;
		lights->bins.buff[un.index].alias = un.index;
	}
	while (over->len > 0)
	{
		ov = vec_outcome_pop(over);
		lights->bins.buff[ov.index].prob = ov.p_hat;
		lights->bins.buff[ov.index].alias = ov.index;
	}
}

void	create_alias_table(t_lights *lights)
{
	t_vec_outcome	under;
	t_vec_outcome	over;
	t_outcome		un;
	t_outcome		ov;

	init_bins(lights);
	init_outcome_arrays(lights, &over, &under);
	while (under.len > 0 && over.len > 0)
	{
		un = vec_outcome_pop(&under);
		ov = vec_outcome_pop(&over);
		lights->bins.buff[un.index].alias = ov.index;
		ov.p_hat -= 1.0 - un.p_hat;
		lights->bins.buff[ov.index].prob = ov.p_hat;
		if (ov.p_hat > 1)
			vec_outcome_push(&over, ov);
		else
			vec_outcome_push(&under, ov);
	}
	handle_remaining_outcomes(lights, &under, &over);
	free(under.buff);
	free(over.buff);
}

int	sample_alias_table(t_lights *lights, float lu)
{
	int		offset;
	float	up;

	if (lights->lights.len == 0)
		return (-1);
	if (lu * lights->lights.len < lights->lights.len - 1)
		offset = lu * lights->lights.len;
	else
		offset = lights->lights.len - 1;
	up = lu * lights->lights.len - offset;
	if (up < lights->bins.buff[offset].prob)
		return (offset);
	else
		return (lights->bins.buff[offset].alias);
}
