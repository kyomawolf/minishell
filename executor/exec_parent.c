/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parent.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 17:11:43 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/17 03:04:25 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "structs.h"
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>

void	ft_t_exec_init(t_exec *exec_data, t_node *head)
{
	exec_data->num_cmds = ft_s_node_iter(head);
	exec_data->cmd_count = 0;
	exec_data->pipes = NULL;
	exec_data->here_doc = 0;
}

int	ft_open_pipes(t_exec *exec_data)
{
	int	i;
	int	*pipe_x;

	exec_data->pipes = (int **)malloc(sizeof(int *) * exec_data->num_cmds + 1);
	if (exec_data->pipes == NULL)
		return (1);
	i = 0;
	while (i < exec_data->num_cmds)
	{
		pipe_x = (int *)malloc(sizeof(int) * 2);
		exec_data->pipes[i] = pipe_x;
		if (pipe_x == NULL)
			return (1);
		i++;
	}
	exec_data->pipes[i] = NULL;
	i = 0;
	while (exec_data->pipes[i])
	{
		pipe(exec_data->pipes[i]);
		i++;
	}
	close(exec_data->pipes[0][0]);
	close(exec_data->pipes[0][1]);
	return (0);
}

void	ft_parent_close_used_pipes(t_exec *exec_data)
{
	if (exec_data->cmd_count != exec_data->num_cmds - 1)
		close(exec_data->pipes[exec_data->cmd_count + 1][1]);
	if (exec_data->cmd_count > 0)
		close(exec_data->pipes[exec_data->cmd_count][0]);
}

int	ft_parent_waitpid(t_exec *exec_data, pid_t *pid)
{
	int	status;
	int	es;
	int	i;

	i = 0;
	while (i < exec_data->num_cmds)
		waitpid(pid[i++], &status, 0);
	es = 0;
	if (WIFEXITED(status))
		es = WEXITSTATUS(status);
	free(pid);
	pid = NULL;
	i = 0;
	while (exec_data->pipes != NULL && exec_data->pipes[i] != NULL)
	{
		free (exec_data->pipes[i]);
		exec_data->pipes[i] = NULL;
		i++;
	}
	free (exec_data->pipes);
	exec_data->pipes = NULL;
	return (es);
}

