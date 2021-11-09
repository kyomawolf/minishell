/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 16:45:35 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/08 16:56:02 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minis.h"

struct s_simple_com	**free_simple_com_list(struct s_simple_com ***tofree, int l)
{
	int	i;
	int	ii;

	i = 0;
	while (i < l && (*tofree)[i] != NULL)
	{
		ii = 0;
		while (ii < (*tofree)[i]->number_arguments)
		{
			free((*tofree)[i]->arguments[ii]);
			ii++;
		}
		free((*tofree)[i]->arguments);
		free((*tofree)[i]);
		i++;
	}
	free(*tofree);
	*tofree = NULL;
	return (*tofree);
}
