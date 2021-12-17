/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filedes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 17:59:29 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/13 18:06:01 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "struct.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int	close_pipe(t_data *data, int mode)
{
	if (!mode)
	{
		if (close((int)(data->(*opendes)->content)))
			return (1);
	}
	data->(*opendes) = data->(*opendes)->next;
	free(data->(*opendes)->prev);
	data->(*opendes)->prev = NULL;
	return (0);
}
