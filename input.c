/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 15:33:39 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/07 18:36:31 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include "struct.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

//maybe include pipecontrol
//if (sig == SIGPIPE)
//{
//	printf("PIPE FAULT\n");
//	input_attributes_clear();
//	//close_pipe(data, 1);
//	return ;
//}

void	input_interrupt(int sig)
{
	struct termios	loc;

	tcgetattr(1, &loc);
	if (sig == SIGSEGV)
	{
		printf("SEGMENTATION FAULT\n");
		input_attributes_clear();
		exit(11);
	}
	if (sig == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
	}
	rl_on_new_line();
	rl_redisplay();
}

//tcsetattr for disabling signal printing
//		->other file
void	input_attributes_add(void)
{
	struct termios	loc;

	signal(SIGQUIT, input_interrupt);
	signal(0, input_interrupt);
	signal(1, input_interrupt);
	signal(2, input_interrupt);
	signal(3, input_interrupt);
	signal(4, input_interrupt);
	signal(5, input_interrupt);
	signal(6, input_interrupt);
	signal(7, input_interrupt);
	signal(11, input_interrupt);
	signal(13, input_interrupt);
	tcgetattr(1, &loc);
	if ((loc.c_lflag & (0x1 << 6)) == ECHOCTL)
	{
		loc.c_lflag -= ECHOCTL;
		tcsetattr(1, TCSANOW, &loc);
	}
}

void	input_attributes_clear(void)
{
	struct termios	loc;

	tcgetattr(1, &loc);
	if ((loc.c_lflag & (0x1 << 6)) != ECHOCTL)
	{
		loc.c_lflag += ECHOCTL;
		tcsetattr(1, TCSANOW, &loc);
	}
}

void	input_readline(t_data *data)
{
	input_attributes_add();
	if (data->input != NULL)
	{
		free(data->input);
		data->input = NULL;
	}
	if (data->prompt == NULL)
	{
		printf("\n\n		NO MEMORY LEFT\n");
		data->input = readline("minishell -> ");
	}
	else
		data->input = readline(data->prompt);
	if (data->input != NULL && data->input[0] != '\0')
		add_history(data->input);
}
