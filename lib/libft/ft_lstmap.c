/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 12:03:38 by jkasper           #+#    #+#             */
/*   Updated: 2021/06/25 19:54:51 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*curr;
	t_list	*new;
	t_list	*next;

	if (lst == NULL || del == NULL || f == NULL)
		return (NULL);
	curr = lst;
	new = NULL;
	while (curr != NULL)
	{
		next = ft_lstnew(f(curr->content));
		if (next == NULL)
		{
			ft_lstclear(&new, del);
			return (NULL);
		}
		ft_lstadd_back(&new, next);
		curr = curr->next;
	}
	return (new);
}
