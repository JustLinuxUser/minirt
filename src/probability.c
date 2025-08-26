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

typedef struct t_outcome {
    float pHat;
    int index;
} t_outcome;

void createAliasTable(t_lights *lights)
{
    t_AliasTable ret;

    int i = -1;
    float sum = lights->total_power;
    while(++i < lights->n_lights)
        ret.bins[i].p = lights->lights[i].intensity / sum;

    t_outcome under[3] = {0}, over[3] = {0}, un = {0}, ov = {0};
    i = -1;
    int j = 0;
    int k = 0;
    while (++i < lights->n_lights)
    {
        float pHat = ret.bins[i].p * lights->n_lights;
        if (pHat < 1)
        {
            under[j].pHat = pHat;
            under[j].index = i;
            j++;
        }
        else {
            over[k].pHat = pHat;
            over[k].index = i;
            k++;
        }
    }
    j--;
    k--;
    while (j >= 0 && k >= 0)
    {
        un = under[j];
        ov = over[k];

        while (un.pHat == -100)
            j--;

        while (ov.index == -100)
            k--;
        
        ret.bins[un.index].q = un.pHat;
        ret.bins[un.index].alias = ov.index;

        float pExcess = un.pHat + ov.pHat - 1;
        if (pExcess < 1)
        {
            under[j].pHat = pExcess;
            under[j].index = ov.index; 
            k--;
        }
        else
        {
            over[k].pHat = pExcess;
            over[k].index = ov.index;
            j--;     
        }  
    }
    
    if (k >= 0)
    {
        ov = over[k];
        ret.bins[ov.index].q = 1;
        ret.bins[ov.index].alias = -1;
    }

    if (j >= 0)
    {
        un = under[j];
        ret.bins[un.index].q = 1;
        ret.bins[un.index].alias = -1;
    }
    lights->table = ret;
}

int SampleAliasTable(t_lights *lights, float lu)
{
    int offset;
    if (lu * lights->n_lights < lights->n_lights - 1)
        offset = lu * lights->n_lights;
    else
        offset = lights->n_lights - 1;
    float up;
    if (lu * lights->n_lights - offset < 0.9999f)
        up = lu * lights->n_lights - offset;
    else
        up = 0.9999f;
    if (up < lights->table.bins[offset].q)
        return offset;
    else
        return lights->table.bins[offset].alias; 
}