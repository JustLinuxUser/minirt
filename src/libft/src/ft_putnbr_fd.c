/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:53:09 by anddokhn          #+#    #+#             */
/*   Updated: 2024/09/18 17:53:13 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>

void	ft_putnbr_fd(int n, int fd)
{
	long	nl;

	nl = n;
	if (nl < 0)
	{
		write(fd, "-", 1);
		nl *= -1;
	}
	if (nl >= 10)
	{
		ft_putnbr_fd(nl / 10, fd);
	}
	write(fd, &"0123456789"[nl % 10], 1);
}
