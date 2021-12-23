/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parent.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 17:11:43 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/23 22:50:25 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "struct.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

static void	ft_exec_data_free_pipes(t_exec *exec_data)
{
	int	i;

	i = 0;
	while (exec_data->pipes != NULL && exec_data->pipes[i] != NULL)
	{
		free (exec_data->pipes[i]);
		exec_data->pipes[i] = NULL;
		i++;
	}
	free (exec_data->pipes);
	exec_data->pipes = NULL;
}

static int	ft_create_pipe_fd(t_exec *exec_data)
{
	int	i;

	i = 0;
	while (i < exec_data->num_cmds)
	{
		exec_data->pipes[i] = malloc(sizeof(int) * 2);
		if (exec_data->pipes[i] == NULL)
		{
			ft_exec_data_free_pipes(exec_data);
			return (1);
		}
		i++;
	}
	exec_data->pipes[i] = NULL;
	return (0);
}

int	ft_open_pipes(t_exec *exec_data, pid_t *pid)
{
	int	ret;
	int	i;

	ret = 1;
	exec_data->pipes = malloc(sizeof(int *) * (exec_data->num_cmds + 1));
	if (exec_data->pipes != NULL && !ft_create_pipe_fd(exec_data))
	{
		ret = 0;
		i = 0;
		while (exec_data->pipes[i])
			pipe(exec_data->pipes[i++]);
		close(exec_data->pipes[0][0]);
		close(exec_data->pipes[0][1]);
	}
	else
	{
		free(pid);
		pid = NULL;
	}
	return (ret);
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
	ft_exec_data_free_pipes(exec_data);
	return (es);
}
