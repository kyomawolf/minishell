/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 13:53:37 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/17 23:55:50 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include "struct.h"
#include <unistd.h>
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
			getcwd(cwd, 0);
			envpcp->content = ft_strjoin("PWD=", cwd);
			free(cwd);
		}
		envpcp = envpcp->next;
	}
}

int	cd_main(char **argv, t_data *data)//does not work with relative path, needs rework
{
	if (!ft_strncmp(argv[1], "-\0", 2))
	{
		chdir(mini_getenv(data, "OLDPWD"));
	}
	else if (argv[1] == NULL)
	{
		chdir(mini_getenv(data, "HOME"));
	}
	else if (chdir(argv[1]) == -1)
	{
		ft_putstr_fd("No such file or directory: ", 1);
		ft_putstr_fd(argv[1], 1);
		write(1, "\n", 1);
		return (42);
	}
	cd_change_pwd(data);
	data->currdir = mini_getenv(data, "PWD");
	get_prompt(data);
	return (0);
}
