/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 17:13:25 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/17 03:03:50 by mstrantz         ###   ########.fr       */
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

int	ft_execute(t_node *head, t_data *data)
{
	pid_t		*pid;
	int			i;
	int			err_status;
	t_exec		exec_data;
	t_bin		*cmd;

	ft_t_exec_init(&exec_data, head);
	pid = (pid_t *)malloc(sizeof(pid_t) * exec_data.num_cmds);
	if (pid == NULL)
		return (-1);
	if (ft_open_pipes(&exec_data))
	{
		free(pid);
		pid = NULL;
		//free exec_data + members
		return (-1);
	}
	i = 0;
	while (exec_data.cmd_count < exec_data.num_cmds)
	{
		cmd = (t_bin *)head->content;
		if (cmd->io != NULL && cmd->io->infile == NULL && cmd->io->heredoc_node != NULL)
			exec_data.here_doc = 1;
		else
			exec_data.here_doc = 0;
		pid[i] = fork();
		if (pid[i] < 0)
			return (-1);
		if (pid[i] == 0)
			ft_child_process(head, data, &exec_data);
		ft_parent_close_used_pipes(&exec_data);
		exec_data.cmd_count++;
		head = head->next;
		i++;
	}
	err_status = ft_parent_waitpid(&exec_data, pid);
	return (err_status);
}
