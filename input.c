/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 15:33:39 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/11 15:52:43 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "struct.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>

void	input_interrupt(int sig)
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
void	input_attributes_add(void)
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
