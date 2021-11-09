/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 18:23:13 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/09 19:43:52 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "minis.h"
#include "libft.h"

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

void	main_interrupt(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
	}
	rl_on_new_line();
	rl_redisplay();
}

//tcsetattr for disabling signal printing
//		->other file
void	attributes(void)
{
	struct termios	loc;

	signal(SIGINT, main_interrupt);
	signal(SIGQUIT, main_interrupt);
	signal(6, main_interrupt);
	tcgetattr(1, &loc);
	if ((loc.c_lflag & (0x1 << 6)) == ECHOCTL)
	{
		loc.c_lflag -= ECHOCTL;
		tcsetattr(1, TCSANOW, &loc);
	}
}

//		->other file
void	main_readline(t_data *data)
{
	data->input = readline(data->prompt);
	attributes();
	add_history(data->input);
}

void	main_loop(t_data *data)
{
	attributes();
	while (1)
	{
		main_readline(data);
		if (data->input == NULL)
			continue ;
		if (!ft_strncmp(data->input, "exit\0", 5))
			return ;
		//if (!par_main(data))
		//	error_main(data);
		//else
		//	execution_main(data);
		//memory_loop_main(data);
		get_prompt(data);
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
	//ret->promp = "minishell % ";
	get_prompt(ret);
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
