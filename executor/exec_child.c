/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 17:10:47 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/17 16:14:32 by mstrantz         ###   ########.fr       */
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

static void	ft_adjust_pipe_helper(t_exec *exec_data, t_node *head)
{
	int	fd;
	int	i;

	//IRD if IO true
	if (((t_bin *)head->content)->io != NULL && ((t_bin *)head->content)->io->infile != NULL)
	{
		fd = open(((t_bin *)head->content)->io->infile, O_RDONLY);
		/* if (fd == -1)
			ft_free_input(exec_data, EXIT); */
		dup2(fd, STDIN_FILENO);
		close(fd);
	}// IRD  NOT first cmd
	else if (exec_data->cmd_count != 0)
	{
		dup2(exec_data->pipes[exec_data->cmd_count][0], STDIN_FILENO);
		//added close
		close(exec_data->pipes[exec_data->cmd_count][0]);
	}
	// else read from stdin
	//ORD if IO is true
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
		}
	}// ORD pipe: more than 1 cmd and not last cmd
	else if (exec_data->cmd_count != exec_data->num_cmds - 1)
	{
		dup2(exec_data->pipes[exec_data->cmd_count + 1][1], STDOUT_FILENO);
		//added close
		close(exec_data->pipes[exec_data->cmd_count + 1][1]);
	}//first cmd, but not last
	else if (exec_data->cmd_count == 0 && exec_data->num_cmds != 1)
	{
		dup2(exec_data->pipes[1][1], STDOUT_FILENO);
		close(exec_data->pipes[1][1]);
	}
	//else output in stdout: last cmd
}

void	ft_close_unused_pipes(t_exec *exec_data)
{
	int	i;

	i = 1;
	while (exec_data->pipes[i])
	{
		if (i != exec_data->cmd_count)
			close(exec_data->pipes[i][0]);
		if (i != exec_data->cmd_count + 1)
			close(exec_data->pipes[i][1]);
		i++;
	}
}

static void	ft_adjust_pipes(t_exec *exec_data, t_node *head)
{
	ft_close_unused_pipes(exec_data);
	if (exec_data->here_doc == 1)
		ft_exec_here_doc(exec_data, head);
	else
		ft_adjust_pipe_helper(exec_data, head);
}

void	ft_child_process(t_node *head, t_data *data, t_exec *exec_data)
{
	char	**cmd_arr;

	cmd_arr = ((t_bin *)head->content)->command->arguments;
	ft_adjust_pipes(exec_data, head);
	builtin_check(cmd_arr, data);
	path_main(data, cmd_arr);
	execve(cmd_arr[0], cmd_arr, list_to_array(data->envp));
	exit(EXIT_FAILURE);
	//free and exit in case of failure
}
