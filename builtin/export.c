/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 17:21:48 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/04 21:52:54 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include <stdlib.h>

//WARNING NEW HAS TO BE FREED OUT OF EXPORT

int	export_checkoverwrite(t_node *envp, char *new)
{
	char	*varname;
	int		len;

	len = ft_strchr(new, '=') - new + 1;
	while (envp != NULL)
	{
		if (!ft_strncmp((char *)envp->content, new, len))
		{
			free(envp->content);
			envp->content = new;
			return (1);
		}
		envp = envp->next;
	}
	return (0);
}

int	export_main(t_node *envp, char *new) //exits always with EXITCODE 0
{
	t_node	*new_node;

	if (!ft_strchr(new, '='))
		return (-1);
	if (export_checkoverwrite(envp, new))
		return (0);
	new_node = ft_s_node_create(void *content);
	if (new_node == NULL)
		return (-1);
	ft_s_node_add_back(&envp, new_node);
	return (0);
}
