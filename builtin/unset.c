/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 22:45:31 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/17 23:15:49 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include "structs.h"
#include <stdlib.h>
#include <unistd.h>

int	unset_main(char **argv, t_data *data)
{
	int		len;
	t_node	*envp_cp;
	int		i;

	i = -1;
	while (argv[++i] != NULL)
	{
		envp_cp = data->envp;
		len = ft_strchr(argv[i], '=') - argv[i] + 1;
		while (envp_cp != NULL && len > 0)
		{
			if (!ft_strncmp((char *)envp_cp->content, argv[i], len))
			{
				free(envp_cp->content);
				if (envp_cp->prev != NULL)
					((t_node *)envp_cp->prev)->next = envp_cp->next;
				if (envp_cp->next != NULL)
					((t_node *)envp_cp->next)->prev = envp_cp->prev;
				break ;
			}
			envp_cp = envp_cp->next;
		}
	}
	return (0);
}
