/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 23:56:32 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/17 23:59:52 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "structs.h"
#include "stdio.h"

int	env_main(t_data *data)
{
	t_node	*node;

	node = data->envp;
	while (node != NULL)
	{
		printf("%s\n", node->content);
		node = node->next;
	}
	return (0);
}
