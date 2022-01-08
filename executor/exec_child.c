/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 20:59:19 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/08 21:43:03 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "struct.h"
#include "libft.h"
#include "exec.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void	ft_child_process_error(char **cmd_arr)
{
	if (errno == 13)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_arr[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_arr[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
}

void	ft_child_process(t_node *pl, t_data *data, t_exec *exec_data, \
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
	ft_child_process_error(cmd_arr);
	child_free_at_exit(pl, data, exec_data, ori_head);
	free_char_array(&envp_arr);
	exit(127);
}
