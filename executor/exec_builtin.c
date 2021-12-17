/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 01:33:48 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/17 02:10:09 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "structs.h"
#include "libft.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

t_e_builtin	get_builtin_code(char *cmd_name)
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

void	builtin_check(char **cmd_arr, t_data *data)
{
	char		*cmd_name;
	int			exit_status;
	t_e_builtin	builtin;

	cmd_name = cmd_arr[0];
	builtin = get_builtin_code(ft_tolower(cmd_name));
	if (builtin != NONE)
	{
		exit_status = run_builtin(builtin, cmd_arr, data);
		//free everything ?
		exit(exit_status);
	}
}

int	run_builtin(t_e_builtin builtin, char **cmd_arr, t_data *data)
{
	int	exit_status;

	if (builtin == ECHO)
	{
		exit_status = ft_echo();
	}
	else if (builtin == CD)
	{
		exit_status = ft_cd();
	}
	else if (builtin == PWD)
	{
		exit_status = ft_pwd();
	}
	else if (builtin == EXPORT)
	{
		exit_status = ft_export();
	}
	else if (builtin == UNSET)
	{
		exit_status = ft_unset();
	}
	else if (builtin == ENV)
	{
		exit_status = ft_env();
	}
	else if (builtin == EXIT)
	{
		exit_status = ft_exit();
	}
	return (exit_status);
}
