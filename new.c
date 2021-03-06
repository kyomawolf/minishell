/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 15:42:10 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/29 18:58:11 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "minis.h"
#include "libft.h"
#include "struct.h"
#include <stdio.h>

t_data	*new_t_data(char **envp)
{
	t_data	*ret;
	t_node	*head;
	int		i;

	ret = ft_calloc(1, sizeof(t_data));
	if (ret == NULL)
		return (NULL);
	head = ft_s_node_create(ft_strdup(envp[0]));
	if (head == NULL)
		return (NULL);
	ret->currdir = NULL;
	ret->currdir = getcwd(NULL, 0);
	if (ret->currdir == NULL)
		return (NULL);
	ret->input = NULL;
	i = 1;
	while (envp[i] != NULL)
		ft_s_node_add_back(&head, ft_s_node_create(ft_strdup(envp[i++])));
	ft_s_node_add_back(&head, ft_s_node_create(ft_strdup("?=0")));
	ret->envp = head;
	get_prompt(ret);
	return (ret);
}
