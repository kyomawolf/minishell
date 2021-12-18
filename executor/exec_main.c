/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 17:13:25 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/18 15:22:39 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "structs.h"
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void	wait_child(int sig)
{
	(void) sig;
	return ;
}

int	ft_execution_init(t_node *head, t_exec *exec_data, t_data *data, pid_t *pid)
{
	int	i;

	i = 0;
	while (exec_data->cmd_count < exec_data->num_cmds)
	{
		pid[i] = fork();
		if (pid[i] < 0)
			return (-1);
		if (pid[i] == 0)
		{
			ft_child_process(head, data, exec_data);
		}
		ft_parent_close_used_pipes(exec_data);
		exec_data->cmd_count++;
		head = head->next;
		i++;
	}
	return (0);
}

int	ft_execute(t_node *head, t_data *data)
{
	pid_t		*pid;
	int			exit_status;
	t_exec		exec_data;
	t_e_builtin	builtin_code;

	ft_t_exec_init(&exec_data, head);
	builtin_code = builtin_check(head);
	if (exec_data.num_cmds == 1 && builtin_code != NONE)
		return (ft_builtin_exec_init(builtin_code, head, data, &exec_data));
	else
	{
		pid = (pid_t *)malloc(sizeof(pid_t) * exec_data.num_cmds);
		if (pid == NULL)
			return (-1);
		if (ft_open_pipes(&exec_data, pid))
			return (-1);
		if (ft_execution_init(head, &exec_data, data, pid))
			return (-1);
		signal(SIGQUIT, wait_child);
		signal(0, wait_child);
		signal(1, wait_child);
		signal(2, wait_child);
		signal(3, wait_child);
		signal(4, wait_child);
		signal(5, wait_child);
		signal(6, wait_child);
		signal(7, wait_child);
		signal(11, wait_child);
		signal(13, wait_child);
		exit_status = ft_parent_waitpid(&exec_data, pid);
	}
	return (exit_status);
}
