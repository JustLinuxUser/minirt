/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:15:53 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/05 16:15:53 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/ft_printf/ft_printf.h"
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

void	print_err(void)
{
	static bool	first = true;

	if (first)
		ft_eprintf("Error\n");
	first = false;
}

void	critical_error(char *msg)
{
	print_err();
	ft_eprintf("[CRITICAL ERROR]: %s\n", msg);
	exit(1);
}
