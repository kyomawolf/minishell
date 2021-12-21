/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 17:10:47 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/21 19:42:14 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "structs.h"
#include "libft.h"
#include "exec.h"
#include <stdlib.h>
#include <unistd.h>

/* void	ft_child_process(t_node *head, t_data *data, t_exec *exec_data)
{
	char	**cmd_arr;

	ft_adjust_pipes(exec_data, head);
	if (((t_bin *)head->content)->command->arguments == NULL)
		exit(EXIT_SUCCESS);
	cmd_arr = ((t_bin *)head->content)->command->arguments;
	builtin_check_child(cmd_arr, data);
	path_main(data, cmd_arr);
	execve(cmd_arr[0], cmd_arr, list_to_array(data->envp));
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_arr[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_main(data);
	exit(127);
} */
