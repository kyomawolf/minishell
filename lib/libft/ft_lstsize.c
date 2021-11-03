/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 20:06:07 by jkasper           #+#    #+#             */
/*   Updated: 2021/06/30 16:36:49 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*curr;
	int		i;

	curr = lst;
	i = 1;
	if (lst == NULL)
		return (0);
	while (curr != NULL)
	{
		curr = curr->next;
		i++;
	}
	return (i - 1);
}
