/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:59:46 by anddokhn          #+#    #+#             */
/*   Updated: 2024/10/14 18:52:05 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*lst_new;
	t_list	*temp;
	void	*content_temp;

	lst_new = 0;
	while (lst)
	{
		content_temp = f(lst->content);
		temp = ft_lstnew(content_temp);
		if (temp == 0)
		{
			del(content_temp);
			ft_lstclear(&lst_new, del);
			break ;
		}
		ft_lstadd_back(&lst_new, temp);
		lst = lst->next;
	}
	return (lst_new);
}
