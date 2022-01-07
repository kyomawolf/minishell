/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parent.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 17:11:43 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/07 22:34:07 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minis.h"
#include "struct.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>

void	ft_exec_data_free_pipes(t_exec *exec_data)
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

int	ft_open_pipes(t_exec *exec_data)
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
		{
			if(pipe(exec_data->pipes[i++]) == -1)
			{
				//ft_putstr_fd(strerror(errno), 2);
				//errno = 0;
				//close and free
				// return error
				//
			}
		}
		close(exec_data->pipes[0][0]);
		close(exec_data->pipes[0][1]);
	}
	else
	{
		free(exec_data->pid);
		exec_data->pid = NULL;
	}
	return (ret);
}

int	ft_parent_waitpid(t_exec *exec_data)
{
	int	status;
	int	es;
	int	i;

	i = 0;
	while (i < exec_data->num_cmds)
		waitpid(exec_data->pid[i++], &status, 0);
	es = 0;
	if (WIFEXITED(status))
		es = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		es = 128 + WTERMSIG(status);
		if (es == 128 + SIGQUIT)
			ft_putstr_fd("Quit: 3\n", 2);
		else if (es == 128 + SIGINT)
			ft_putchar_fd('\n', 2);
	}
	free(exec_data->pid);
	exec_data->pid = NULL;
	ft_exec_data_free_pipes(exec_data);
	return (es);
}
