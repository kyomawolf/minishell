/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 22:45:31 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/27 22:24:29 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include "struct.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int	unset_main(char **argv, t_data *data)
{
	int		len;
	t_node	*envp_cp;
	int		i;

	i = -1;
	while (argv[++i] != NULL)
	{
		envp_cp = data->envp;
		len = ft_strlen(argv[i]);
		while (envp_cp != NULL && len > 0)
		{
			if (!ft_strncmp((char *)envp_cp->content, argv[i], len))
			{
				free(envp_cp->content);
				if (envp_cp->prev != NULL)
					((t_node *)envp_cp->prev)->next = envp_cp->next;
				if (envp_cp->next != NULL)
					((t_node *)envp_cp->next)->prev = envp_cp->prev;
				free(envp_cp);
				envp_cp = NULL;
				break ;
			}
			envp_cp = envp_cp->next;
		}
	}
	return (0);
}
