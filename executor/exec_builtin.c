/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 01:33:48 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/18 15:43:27 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "structs.h"
#include "libft.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

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
	/* else if (!ft_strncmp(cmd_name, "exit\0", 5))
		builtin = EXIT; */
	else
		builtin = NONE;
	return (builtin);
}

char	ft_tolower2(unsigned int i, char c)
{
	(void)i;
	return(ft_tolower(c));
}

void	builtin_check_child(char **cmd_arr, t_data *data)
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
		exit_status = run_builtin(builtin, cmd_arr, data);
		//free everything ?
		exit(exit_status);
	}
}

t_e_builtin	builtin_check(t_node *head)
{
	char		**cmd_arr;
	char		*cmd_name;
	int			ret;
	t_e_builtin	builtin_code;

	if (((t_bin *)head->content)->command->arguments == NULL)
		return (NONE);
	cmd_arr = ((t_bin *)head->content)->command->arguments;
	ret = 0;
	cmd_name = ft_strmapi(cmd_arr[0], ft_tolower2);
	builtin_code = get_builtin_code(cmd_name);
	free(cmd_arr[0]);
	cmd_arr[0] = cmd_name;
	return (builtin_code);
}

int	ft_builtin_exec_init(t_e_builtin builtin, t_node *head, t_data *data, t_exec *exec_data)
{
	char	**cmd_arr;
	int		es;

	ft_t_exec_heredoc_check(head, exec_data);
	cmd_arr = ((t_bin *)head->content)->command->arguments;
	ft_adjust_pipes(exec_data, head);
	es = run_builtin(builtin, cmd_arr, data);
	return (es);
}

int	run_builtin(t_e_builtin builtin, char **cmd_arr, t_data *data)
{
	int	exit_status;

	exit_status = 0;
	if (builtin == ECHO)
	{
		exit_status = echo_main(cmd_arr);
	}
	else if (builtin == CD)
	{
		exit_status = cd_main(cmd_arr, data);
	}
	else if (builtin == PWD)
	{
		//exit_status = ft_pwd();
	}
	else if (builtin == EXPORT)
	{
		exit_status = export_main(cmd_arr, data);
	}
	else if (builtin == UNSET)
	{
		exit_status = unset_main(cmd_arr, data);
	}
	else if (builtin == ENV)
	{
		exit_status = env_main(data);
	}
	else if (builtin == EXIT)
	{
		//exit_status = ft_exit();
	}
	return (exit_status);
}
