/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 23:56:32 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/23 22:54:56 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "struct.h"
#include "stdio.h"

int	env_main(t_data *data)
{
	t_node	*node;

	node = data->envp;
	while (node != NULL)
	{
		printf("%s\n", (char *) node->content);
		node = node->next;
	}
	return (0);
}
