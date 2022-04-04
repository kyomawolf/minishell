/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 15:33:39 by jkasper           #+#    #+#             */
/*   Updated: 2022/04/04 13:50:18 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include "struct.h"
#include "get_next_line.h"
#include <stdio.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#ifdef READLINE
	#include <readline/readline.h>
	#include <readline/history.h>
#endif

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
		//printf("%s", data->prompt);
		fflush(0);
	}
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

char *ft_gnl_append(char *line, char c)
{
    int i;
    char *longer;
    
    i = 0;
    while(line[i] != '\0')
        i++;
    longer = malloc(i + 2);
    if(longer == NULL)
        return(NULL);
    i = 0;
    while(line[i] != '\0')
    {
        longer[i] = line[i];
        i++;
    }
    longer[i] = c;
    longer[++i] = '\0';
    free(line);
    return(longer);
}

int get_next_line_wrapper(char **line, int fd)
{
    char    buffer;
    int     flag;
    if(line == NULL)
        return(-1);
    *line = malloc(1);
    if(*line == NULL)
        return(-1);
    *line[0] = '\0';
    while ((flag = read(fd, &buffer, 1) > 0))
    {
        if(buffer == '\n')
            break;
        *line = ft_gnl_append(*line, buffer);
    }
    return(flag);       
}

void	input_readline(t_data *data)
{
	input_attributes_add();
	#ifndef READLINE
		printf("%s", data->prompt);
		fflush(0);
	#endif
	if (data->input != NULL)
	{
		free(data->input);
		data->input = NULL;
	}
	#ifndef READLINE
		if (get_next_line_wrapper(&data->input, 0) == 0)
			data->input = NULL;
	#endif
	#ifdef READLINE
		data->input = readline(data->prompt);
		if (data->input != NULL && data->input[0] != '\0')
			add_history(data->input);
	#endif
}
