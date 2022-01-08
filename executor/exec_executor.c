/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 17:13:25 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/08 14:00:16 by mstrantz         ###   ########.fr       */
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
#include <signal.h>

static void	ft_t_exec_init(t_exec *exec_data, t_node *head)
{
	exec_data->num_cmds = ft_s_node_iter(head);
	exec_data->cmd_count = 0;
	exec_data->pipes = NULL;
	exec_data->pid = NULL;
	exec_data->here_doc = 0;
}

static void	ft_child_process(t_node *pl, t_data *data, t_exec *exec_data, \
								t_node **ori_head)
{
	char	**cmd_arr;
	char	**envp_arr;

	if (ft_adjust_pipes(exec_data, pl))
	{
		child_free_at_exit(pl, data, exec_data, ori_head);
		exit(EXIT_FAILURE);
	}
	if (((t_bin *)pl->content)->command->arguments == NULL)
	{
		child_free_at_exit(pl, data, exec_data, ori_head);
		exit(EXIT_SUCCESS);
	}
	cmd_arr = ((t_bin *)pl->content)->command->arguments;
	builtin_check_child(cmd_arr, data, pl, exec_data);
	path_main(data, cmd_arr);
	envp_arr = list_to_array(data->envp);
	signal(SIGQUIT, child_signal);
	execve(cmd_arr[0], cmd_arr, envp_arr);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_arr[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	child_free_at_exit(pl, data, exec_data, ori_head);
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

static int	ft_execution_init(t_node *pipeline, t_exec *exec_data, \
								t_data *data, t_node **ori_head)
{
	int	i;

	i = 0;
	while (exec_data->cmd_count < exec_data->num_cmds)
	{
		exec_data->pid[i] = fork();
		if (exec_data->pid[i] < 0)
			return (-1);
		if (exec_data->pid[i] == 0)
			ft_child_process(pipeline, data, exec_data, ori_head);
		ft_parent_close_used_pipes(exec_data);
		exec_data->cmd_count++;
		pipeline = pipeline->next;
		i++;
	}
	return (0);
}

int	ft_execute(t_node *pl, t_data *data, t_node **ori_head)
{
	int			exit_status;
	t_exec		exec_data;
	t_e_builtin	builtin_code;

	ft_t_exec_init(&exec_data, pl);
	builtin_code = builtin_check(pl);
	if (exec_data.num_cmds == 1 && builtin_code != NONE)
		return (ft_builtin_exec_init(builtin_code, pl, data, &exec_data));
	else
	{
		exec_data.pid = (pid_t *)malloc(sizeof(pid_t) * exec_data.num_cmds);
		if (exec_data.pid == NULL)
			return (-1);
		if (ft_open_pipes(&exec_data))
			return (-1);
		if (ft_execution_init(pl, &exec_data, data, ori_head))
			return (-1);
		ft_signals();
		exit_status = ft_parent_waitpid(&exec_data);
	}
	return (exit_status);
}
