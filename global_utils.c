/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 19:32:03 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/16 01:01:46 by jkasper          ###   ########.fr       */
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
