/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 13:53:37 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/29 15:37:02 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include "struct.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void	cd_change_pwd(t_data *data)
{
	t_node	*envpcp;
	char	*cwd;

	envpcp = data->envp;
	while (envpcp != NULL)
	{
		if (!ft_strncmp((char *)envpcp->content, "PWD=", 4))
		{
			free(envpcp->content);
			cwd = getcwd(NULL, 0);
			envpcp->content = ft_strjoin("PWD=", cwd);
			free(cwd);
			return ;
		}
		envpcp = envpcp->next;
	}
}

void	cd_change_oldpwd(t_data *data)
{
	t_node	*envpcp;
	char	*cwd;

	envpcp = data->envp;
	while (envpcp != NULL)
	{
		if (!ft_strncmp((char *)envpcp->content, "OLDPWD=", 7))
		{
			free(envpcp->content);
			cwd = getcwd(NULL, 0);
			envpcp->content = ft_strjoin("OLDPWD=", cwd);
			free (cwd);
			return ;
		}
		envpcp = envpcp->next;
	}
}

int	cd_execute(char	*path, t_data *data)
{
	if (access(path, F_OK | X_OK))
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (2);
	}
	cd_change_oldpwd(data);
	return (chdir(path));
}

int	cd_main_special_cases(char **argv, t_data *data)
{
	char	*env_name;
	char	*env_value;
	int		err;

	if (argv[1] == NULL)
		env_name = "HOME";
	else if (!ft_strncmp(argv[1], "-\0", 2))
		env_name = "OLDPWD";
	else
		return (1);
	env_value = mini_getenv(data, env_name);
	if (env_value == NULL)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(env_name, 2);
		ft_putstr_fd(" is not set\n", 2);
		return (2);
	}
	err = cd_execute(env_value, data);
	return (err);
}

int	cd_main(char **argv, t_data *data)
{
	int		err;

	err = cd_main_special_cases(argv, data);
	if (err == 1)
		err = cd_execute(argv[1], data);
	if (err == 2)
		return (1);
	cd_change_pwd(data);
	if (data->currdir != NULL)
		free(data->currdir);
	data->currdir = getcwd(NULL, 0);
	get_prompt(data);
	return (0);
}
