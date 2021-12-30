/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 17:13:25 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/30 23:39:21 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "struct.h"
#include "libft.h"
#include "exec.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static void	ft_t_exec_init(t_exec *exec_data, t_node *head)
{
	exec_data->num_cmds = ft_s_node_iter(head);
	exec_data->cmd_count = 0;
	exec_data->pipes = NULL;
	exec_data->here_doc = 0;
}

static void	ft_child_process(t_node *head, t_data *data, t_exec *exec_data)
{
	char	**cmd_arr;
	char	**envp_arr;

	if (ft_adjust_pipes(exec_data, head))
		exit(EXIT_FAILURE);
	if (((t_bin *)head->content)->command->arguments == NULL)
		exit(EXIT_SUCCESS);
	cmd_arr = ((t_bin *)head->content)->command->arguments;
	builtin_check_child(cmd_arr, data, head);
	path_main(data, cmd_arr);
	envp_arr = list_to_array(data->envp);
	execve(cmd_arr[0], cmd_arr, envp_arr);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_arr[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_main(data);
	free_char_array(&envp_arr);
	exit(127);
}

static void	ft_parent_close_used_pipes(t_exec *exec_data)
{
	if (exec_data->cmd_count != exec_data->num_cmds - 1)
		close(exec_data->pipes[exec_data->cmd_count + 1][1]);
	if (exec_data->cmd_count > 0)
		close(exec_data->pipes[exec_data->cmd_count][0]);
}

static int	ft_execution_init(t_node *head, t_exec *exec_data, t_data *data, \
								pid_t *pid)
{
	int	i;

	i = 0;
	while (exec_data->cmd_count < exec_data->num_cmds)
	{
		pid[i] = fork();
		if (pid[i] < 0)
			return (-1);
		if (pid[i] == 0)
			ft_child_process(head, data, exec_data);
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
		ft_signals();
		exit_status = ft_parent_waitpid(&exec_data, pid);
	}
	return (exit_status);
}
