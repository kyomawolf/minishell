/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 18:23:13 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/03 19:44:22 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "minis.h"
#include "libft.h"
#include "struct.h"

void	get_prompt(t_data *data)
{
	int		i;
	int		ii;
	char	*temp;

	i = ft_strlen(data->currdir) - 1;
	temp = malloc(ft_strlen(data->currdir) + 2);
	ii = 0;
	while (data->currdir[i] != '/')
		i--;
	i++;
	while (data->currdir[i] != '\0')
	{
		temp[ii] = data->currdir[i];
		ii++;
		i++;
	}
	temp[ii++] = '-';
	temp[ii++] = '>';
	temp[ii++] = ' ';
	temp[ii] = '\0';
	data->prompt = ft_strjoin("minishell@", temp);
	free(temp);
}

void	main_loop(t_data *data)
{
	while (1)
	{
		input_attributes_add();
		input_readline(data);
		input_attributes_clear();
		if (data->input == NULL)
			continue ;
		if (!ft_strncmp(data->input, "exit\0", 5))
			return ;
		//filter no newline from history
		//if (!par_main(data))
		//	error_main(data);
		//else
		//	execution_main(data);
		//memory_loop_main(data);
		get_prompt(data);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	data = NULL;
	data = new_t_data();
	(void) envp;
	(void) argc;
	(void) argv;
	if (argc > 1)
	{
		data->list = ft_lexer(argv[1]);
		data->tree = builder_main(data->list);
		return (0);
	}
	if (data == NULL)
		return (0);
	main_loop(data);
	//memory_main(data);
}
