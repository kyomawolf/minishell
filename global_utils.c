/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 19:32:03 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/16 16:55:31 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include "structs.h"
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
		if (ft_strnstr((const char *)temp->content, format_s, \
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
