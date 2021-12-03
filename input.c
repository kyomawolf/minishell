/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 15:33:39 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/03 19:36:37 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "struct.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>

void	input_interrupt(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
	}
	if (sig == 6)
	{
		//call exit function
		exit(1);
	}
	rl_on_new_line();
	rl_redisplay();
}

//tcsetattr for disabling signal printing
//		->other file
void	input_attributes_add(void)
{
	struct termios	loc;

	signal(SIGINT, input_interrupt);
	signal(SIGQUIT, input_interrupt);
	signal(6, input_interrupt);
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

	signal(SIGINT, input_interrupt);
	signal(SIGQUIT, input_interrupt);
	signal(6, input_interrupt);
	tcgetattr(1, &loc);
	if ((loc.c_lflag & (0x1 << 6)) == ECHOCTL)
	{
		loc.c_lflag += ECHOCTL;
		tcsetattr(1, TCSANOW, &loc);
	}
}

void	input_readline(t_data *data)
{
	data->input = readline(data->prompt);
	input_attributes_add();
	add_history(data->input);
}
