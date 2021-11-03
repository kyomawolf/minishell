/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 20:15:10 by jkasper           #+#    #+#             */
/*   Updated: 2021/08/28 22:55:09 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

	//removed for project, may change in the future
	//if (lst == NULL)
	//	return ;

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*curr;

	curr = ft_lstlast(*lst);
	if (curr == NULL)
		ft_lstadd_front(lst, new);
	else
	{
		curr->next = new;
		new->prev = curr;
	}
}
