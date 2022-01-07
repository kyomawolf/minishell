/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_io.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/30 12:19:31 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/07 18:13:50 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "libft.h"
#include "minis.h"

//create

t_node_io	*ft_t_node_io_create(char *file, char io_type)
{
	t_node_io	*ret;

	ret = ft_calloc(1, sizeof(t_node_io));
	if (ret == NULL)
		return (NULL);
	ret->next = NULL;
	ret->prev = NULL;
	ret->file = ft_strdup(file);
	if (ret->file == NULL)
	{
		free(ret);
		return (NULL);
	}
	ret->io_type = io_type;
	ret->active_hd = 1;
	return (ret);
}

//free

int	ft_t_node_io_size(t_node_io *head)
{
	int	len;

	len = 0;
	while (head != NULL)
	{
		head = head->next;
		len++;
	}
	return (len);
}

t_node_io	*ft_t_node_io_get_last(t_node_io *head, int type)
{
	if (head != NULL)
	{
		if (type == HERE_DOC)
		{
			while (head->next != NULL)
			{
				if (head->io_type == HERE_DOC && head->active_hd)
					head->active_hd = 0;
				head = head->next;
			}
			if (head->io_type == HERE_DOC && head->active_hd)
				head->active_hd = 0;
		}
		else
		{
			while (head->next != NULL)
				head = head->next;
		}
	}
	return (head);
}

void	ft_t_node_io_add_back(t_node_io **head, t_node_io *node)
{
	t_node_io	*last;

	if (*head == NULL)
		*head = node;
	else
	{
		last = ft_t_node_io_get_last(*head, node->io_type);
		node->prev = last;
		last->next = node;
	}
}
