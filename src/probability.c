/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   probability.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:07:28 by mhornero          #+#    #+#             */
/*   Updated: 2025/08/24 17:52:47 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdio.h>
#include "libft/ft_printf/ft_printf.h"

typedef struct t_outcome {
    float pHat;
    int index;
} t_outcome;

#include "libft/generated/vec_outcome.h"

void create_alias_table(t_lights *lights)
{
    int i = -1;
    float sum = lights->total_power;
    while(++i < lights->lights.len) {
		vec_alias_bin_push(&lights->bins, (t_alias_bin){.prob = (float)lights->lights.len * (float)lights->lights.buff[i].intensity / sum});
	}

	t_vec_outcome under;
	t_vec_outcome over;

	vec_outcome_init(&under, lights->lights.len);
	vec_outcome_init(&over, lights->lights.len);
	i = -1;
	while (++i < lights->lights.len) {
		t_outcome outcome = {.index = i, .pHat = lights->bins.buff[i].prob};
		if (lights->lights.buff[i].intensity * lights->lights.len > sum) {
			vec_outcome_push(&over, outcome);
		} else {
			vec_outcome_push(&under, outcome);
		}
	}
	for (size_t i = 0; i < under.len; i++) {
		ft_printf("under: %f\n", under.buff[i].pHat);
	}

	for (size_t i = 0; i < over.len; i++) {
		ft_printf("over: %f\n", over.buff[i].pHat);
	}

	while (under.len > 0 && over.len > 0) {
		t_outcome un = vec_outcome_pop(&under);
		t_outcome ov = vec_outcome_pop(&over);

		ft_printf("got under: %i\n", un.index);
		ft_printf("got over: %i\n", ov.index);
		lights->bins.buff[un.index].alias = ov.index;

		ft_printf("un phat: %f\n", un.pHat);
		ft_printf("ov phat: %f\n", ov.pHat);
		ov.pHat -= 1.0 - un.pHat;
		ft_printf("new ov phat: %f\n", ov.pHat);
		lights->bins.buff[ov.index].prob = ov.pHat;
		if (ov.pHat > 1) {
			vec_outcome_push(&over, ov);
		} else {
			vec_outcome_push(&under, ov);
		}
	}

	while (under.len > 0) {
		t_outcome un = vec_outcome_pop(&under);
		lights->bins.buff[un.index].prob = un.pHat;
		lights->bins.buff[un.index].alias = un.index;
	}

	while (over.len > 0) {
		t_outcome ov = vec_outcome_pop(&over);
		lights->bins.buff[ov.index].prob = ov.pHat;
		lights->bins.buff[ov.index].alias = ov.index;
	}
	for (int i = 0; i < lights->lights.len; i++) {
		ft_printf("prob: %f, %i\n", lights->bins.buff[i].prob, lights->bins.buff[i].alias);
	}

}

int sample_alias_table(t_lights *lights, float lu)
{
    int offset;
	if (lights->lights.len == 0)
		return (-1);
    if (lu * lights->lights.len < lights->lights.len - 1)
        offset = lu * lights->lights.len;

    else
        offset = lights->lights.len - 1;
    float up;
    if (lu * lights->lights.len - offset < 0.9999f)
        up = lu * lights->lights.len - offset;
    else
        up = 0.9999f;
    if (up < lights->bins.buff[offset].prob)
        return offset;
    else
        return lights->bins.buff[offset].alias; 
}
