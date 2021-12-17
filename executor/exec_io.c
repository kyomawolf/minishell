/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_io.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 18:28:48 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/17 02:13:42 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "structs.h"
#include "libft.h"
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

void	ft_exec_here_doc(t_exec *exec_data, t_node *head)
{
	int		pipe_here_doc[2];
	char	*line;
	int		i;
	t_node	*here_doc;
	t_node	*temp;

	here_doc = ((t_bin *)head->content)->io->heredoc_node;
	pipe(pipe_here_doc);
	while (here_doc != NULL)
	{
		write(pipe_here_doc[1], here_doc->content, ft_strlen(here_doc->content));
		write(pipe_here_doc[1], "\n", 1);
		temp = here_doc;
		here_doc = here_doc->next;
		free(temp->content);
		temp->content = NULL;
		free(temp);
		temp = NULL;
	}
	close(pipe_here_doc[1]);
	dup2(pipe_here_doc[0], STDIN_FILENO);
	close(pipe_here_doc[0]);
	//if ORD given: create files and set ORD to last file
	if (((t_bin *)head->content)->io != NULL && ((t_bin *)head->content)->io->output != NULL)
	{
		i = 0;
		while (((t_bin *)head->content)->io->output[i] != NULL)
		{
			//open/create files in accordance to the given mode (trunc/append)
			if (((t_bin *)head->content)->io->o_mode == 4)//mode is TRUNC)
			{
				fd = open(((t_bin *)head->content)->io->output[i], O_WRONLY | O_CREAT | O_TRUNC, 0777);
				//guard if fd == -1
			}
			else if (((t_bin *)head->content)->io->o_mode == 3) //if mode is APPEND
			{
				fd = open(((t_bin *)head->content)->io->output[i], O_WRONLY | O_CREAT | O_APPEND, 0777);
				//guard
			}
			/* if (fd == -1)
				free and exit */
			//close "unused files"
			if (((t_bin *)head->content)->io->output[i + 1] == NULL)
			{
			//redirect stdout to outfile (last file in output[])
				dup2(fd, STDOUT_FILENO);
			}
			close (fd);
			i++;
	}// if no ORD && not last cmd: set stdout to writeend of next pipe
	else if (exec_data->cmd_count != exec_data->num_cmds - 1)
	{
		dup2(exec_data->pipes[exec_data->cmd_count + 1][1], STDOUT_FILENO);//dup2(exec_data->pipes[1][1], STDOUT_FILENO);
		close(exec_data->pipes[exec_data->cmd_count + 1][1]);//exec_data->pipes[1][1]);
	}
	else if (exec_data->cmd_count == 0 && exec_data->num_cmds != 1)
	{
		dup2(exec_data->pipes[1][1], STDOUT_FILENO);
		close(exec_data->pipes[1][1]);
	}
	//else redirect output to stdout
}
