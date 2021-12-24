/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 01:33:48 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/24 01:00:07 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "struct.h"
#include "exec.h"
#include "libft.h"
#include <errno.h> //?
#include <stdlib.h>
#include <unistd.h> //?
#include <stdio.h> //?

static t_e_builtin	get_builtin_code(char *cmd_name)
{
	t_e_builtin	builtin;

	if (!ft_strncmp(cmd_name, "echo\0", 5))
		builtin = ECHO;
	else if (!ft_strncmp(cmd_name, "cd\0", 3))
		builtin = CD;
	else if (!ft_strncmp(cmd_name, "pwd\0", 4))
		builtin = PWD;
	else if (!ft_strncmp(cmd_name, "export\0", 7))
		builtin = EXPORT;
	else if (!ft_strncmp(cmd_name, "unset\0", 6))
		builtin = UNSET;
	else if (!ft_strncmp(cmd_name, "env\0", 4))
		builtin = ENV;
	else if (!ft_strncmp(cmd_name, "exit\0", 5))
		builtin = EXIT;
	else
		builtin = NONE;
	return (builtin);
}

static int	run_builtin(t_e_builtin builtin, char **cmd_arr, t_data *data, t_node *head)
{
	int	exit_status;

	exit_status = 0;
	if (builtin == ECHO)
		exit_status = echo_main(cmd_arr);
	else if (builtin == CD)
		exit_status = cd_main(cmd_arr, data);
	else if (builtin == PWD)
		exit_status = pwd_main(data);
	else if (builtin == EXPORT)
		exit_status = export_main(cmd_arr, data);
	else if (builtin == UNSET)
		exit_status = unset_main(cmd_arr, data);
	else if (builtin == ENV)
		exit_status = env_main(data);
	else if (builtin == EXIT)
		exit_status = exit_main(cmd_arr, data, head);
	return (exit_status);
}

void	builtin_check_child(char **cmd_arr, t_data *data, t_node *head)
{
	char		*cmd_name;
	int			exit_status;
	t_e_builtin	builtin;

	cmd_name = ft_strmapi(cmd_arr[0], ft_tolower2);
	builtin = get_builtin_code(cmd_name);
	free(cmd_arr[0]);
	cmd_arr[0] = cmd_name;
	if (builtin != NONE)
	{
		exit_status = run_builtin(builtin, cmd_arr, data, head);
		//free everything ?
		exit(exit_status);
	}
}

t_e_builtin	builtin_check(t_node *head)
{
	char		**cmd_arr;
	char		*cmd_name;
	t_e_builtin	builtin_code;

	if (((t_bin *)head->content)->command->arguments == NULL)
		return (NONE);
	cmd_arr = ((t_bin *)head->content)->command->arguments;
	cmd_name = ft_strmapi(cmd_arr[0], ft_tolower2);
	builtin_code = get_builtin_code(cmd_name);
	free(cmd_arr[0]);
	cmd_arr[0] = cmd_name;
	return (builtin_code);
}

int	ft_builtin_exec_init(t_e_builtin builtin, t_node *head, t_data *data, \
		t_exec *exec_data)
{
	char	**cmd_arr;
	int		es;
	int		fd_stdin;
	int		fd_stdout;

	ft_t_exec_heredoc_check(head, exec_data);
	cmd_arr = ((t_bin *)head->content)->command->arguments;
	fd_stdin = dup(STDIN_FILENO);
	fd_stdout = dup(STDOUT_FILENO);
	if (ft_adjust_pipes(exec_data, head))
	{
		dup2(fd_stdin, STDIN_FILENO);
		close (fd_stdin);
		dup2(fd_stdout, STDOUT_FILENO);
		close (fd_stdout);
		return (1);
	}
	es = run_builtin(builtin, cmd_arr, data, head);
	dup2(fd_stdin, STDIN_FILENO);
	close (fd_stdin);
	dup2(fd_stdout, STDOUT_FILENO);
	close (fd_stdout);
	return (es);
}
