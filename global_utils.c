/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 19:32:03 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/06 17:11:46 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include "struct.h"
#include "stdio.h"

char	*mini_getenv(t_data *data, char *var_name)
{
	t_node	*temp;
	int		start;
	char	*format_s;

	temp = data->envp;
	format_s = ft_strjoin(var_name, "=");
	start = ft_strlen(format_s);
	while (temp != NULL)
	{
		if (!ft_strncmp(temp->content, format_s, start)
			&& ft_strnstr((const char *)temp->content, format_s, \
			ft_strlen(temp->content)) != NULL)
			break ;
		temp = temp->next;
	}
	free (format_s);
	format_s = NULL;
	if (temp == NULL)
		return (NULL);
	return (&((char *)temp->content)[start]);
}

char	**list_to_array(t_node *node)
{
	char	**ret;
	int		i;

	ret = ft_calloc(ft_s_node_iter(node) + 1, sizeof(char *));
	i = 0;
	while (node != NULL)
	{
		ret[i] = ft_strdup(((char *)node->content));
		node = node->next;
		i++;
	}
	ret[i] = NULL;
	return (ret);
}

int	ft_s_node_iter(struct s_node *head)
{
	int	i;

	i = 0;
	while (head != NULL)
	{
		head = head->next;
		i++;
	}
	return (i);
}

t_node	*detach_node(t_node **head, t_node *node)
{
	if (head == NULL || *head == NULL)
		return (NULL);
	if ((*head)->next == NULL && (*head)->prev == NULL)
		*head = NULL;
	else if (node && node->prev == NULL)
	{
		*head = (*head)->next;
		(*head)->prev = NULL;
	}
	else if (node && node->next == NULL)
		((t_node *)node->prev)->next = NULL;
	else if (node)
	{
		((t_node *)node->next)->prev = node->prev;
		((t_node *)node->prev)->next = node->next;
	}
	return (node);
}
