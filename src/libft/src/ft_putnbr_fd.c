/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:53:09 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/01 17:04:02 by anddokhn         ###   ########.fr       */
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
		if (write(fd, "-", 1) < 0)
			return ;
		nl *= -1;
	}
	if (nl >= 10)
	{
		ft_putnbr_fd(nl / 10, fd);
	}
	if (write(fd, &"0123456789"[nl % 10], 1))
		return ;
}
