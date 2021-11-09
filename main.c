/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 18:23:13 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/09 13:20:20 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#include "minis.h"
#include "libft.h"

void	get_prompt(t_data *data)
{
	int		i;
	char	*temp;

	i = ft_strlen(data->currdir) -  1;
	temp = malloc()
	while (data->currdir[i] != '\\')
	{
		i--;
	}
}

void	main_interrupt(int sig)
{
	if (sig)
		return ;
	else
		return ;
}

void	main_loop(t_data *data)
{
	while (1)
	{
		signal(SIGINT, main_interrupt);
		signal(SIGQUIT, main_interrupt);
		signal(6, main_interrupt);
		data->input = readline(data->prompt);
		add_history(data->input);
		if (!ft_strncmp(data->input, "exit\0", 5))
			return ;
		//if (!par_main(data))
		//	error_main(data);
		//else
		//	execution_main(data);
		//memory_loop_main(data);
	}
}

t_data	*init_main(void)
{
	t_data	*ret;

	ret = malloc(sizeof(t_data));
	ret->currdir = NULL;
	ret->currdir = getcwd(NULL, 0);
	ret->allcom = malloc(sizeof(struct s_command *));
	ret->input = NULL;
	//get_prompt(ret);
	return (ret);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	data = NULL;
	data = init_main();
	(void) envp;
	(void) argc;
	(void) argv;
	//if (argc > 1)
	//{
	//	data->input = main_argv_to_input(argc, argv, data);
	//}
	if (data == NULL)
		return (0);
	main_loop(data);
	//memory_main(data);
}
