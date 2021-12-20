/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 13:53:37 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/20 14:57:36 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include "struct.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void	cd_change_pwd(t_data *data)
{
	t_node	*envpcp;
	char	*cwd;

	cwd = NULL;
	envpcp = data->envp;
	while (envpcp != NULL)
	{
		if (!ft_strncmp((char *)envpcp->content, "PWD=", 4))
		{
			free(envpcp->content);
			cwd = getcwd(NULL, 0);
			envpcp->content = ft_strjoin("PWD=", cwd);
			free(cwd);
		}
		envpcp = envpcp->next;
	}
}

int	cd_execute(char	*path)
{
	if (access(path, X_OK))
	{
		printf("cd: permission denied.\n");
		return (-2);
	}
	return (chdir(path));
}

int	cd_main(char **argv, t_data *data)
{
	int	err;

	err = 0;
	if (argv[1] == NULL)
		err = cd_execute(mini_getenv(data, "HOME"));
	else if (!ft_strncmp(argv[1], "-\0", 2))
		err = cd_execute(mini_getenv(data, "OLDPWD"));
	else
		err = cd_execute(argv[1]);
	if (err == -1)
	{
		printf("cd: no such file or directory: %s\n", argv[1]);
		return (1);
	}
	else if (err == -2)
		return (1);
	cd_change_pwd(data);
	data->currdir = ft_strdup(mini_getenv(data, "PWD"));
	get_prompt(data);
	return (0);
}
