/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 18:51:36 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/08 17:42:11 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minis.h"

//< > | ' ' '\t' '\'' '\"'

int	par_sim_commands(t_data *data)
{
	int	i;

	i = 0;
	//while (!ft_strchr(data->input[i], "|&<>"))
}

int	par_commands(t_data *data)
{
	if (data->allcom->number_commands == 0)
		data->allcom->command_list = ft_calloc (2,
				sizeof(struct s_simple_com *));
	else
	{
		data->allcom->command_list = utils_realloc_simple_com_list(
				&(data->allcom->command_list), data->allcom->number_commands++,
				data->allcom->number_commands);
	}
	if (data->allcom->command_list == NULL)
	{
		data->allcom->number_commands = 0;
		return (-1);
	}
	if (par_count_commands(data))
		par_commands(data);
	return (data->allcom->number_commands);
}

int	par_main(t_data *data)
{
	int	comm;
	int	i;

	i = 0;
	comm = par_commands(data);
	if (comm == -1)
		return (-1);
	while (i < comm)
	{
		par_args(data, i);
		i++;
	}
}
