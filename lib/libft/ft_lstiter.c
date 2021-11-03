/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 20:32:15 by jkasper           #+#    #+#             */
/*   Updated: 2021/06/23 17:06:29 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*curr;
	t_list	*next;

	if (lst == NULL)
		return ;
	curr = lst;
	next = lst->next;
	while (curr != NULL)
	{
		f(curr->content);
		next = curr->next;
		curr = next;
	}
}
