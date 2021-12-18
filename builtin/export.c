/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 17:21:48 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/18 10:43:04 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h>

//WARNING NEW HAS TO BE FREED OUT OF EXPORT

int	export_checkoverwrite(t_node *envp, char *new)
{
	int		len;

	len = ft_strchr(new, '=') - new + 1;
	while (envp != NULL)
	{
		if (!ft_strncmp((char *)envp->content, new, len))
		{
			if (envp->content != NULL)
				free(envp->content);
			envp->content = new;
			return (42);
		}
		envp = envp->next;
	}
	return (0);
}

int	export_main(char **argv, t_data *data)
{
	t_node	*new_node;
	int		i;

	i = 1;
	while (argv[i] != NULL)
	{
		if (ft_strchr(argv[i], '=') == NULL)
			return (42);
		i++;
	}
	i = 1;
	while (argv[i] != NULL)
	{
		if (!export_checkoverwrite(data->envp, argv[i]))
		{
			new_node = ft_s_node_create(ft_strdup(argv[i]));
			if (new_node == NULL)
				return (42);
			ft_s_node_add_back(&(data->envp), new_node);
		}
		i++;
	}
	return (0);
}
