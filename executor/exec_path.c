/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 21:07:35 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/23 22:50:28 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "struct.h"
#include "minis.h"
#include "libft.h"
#include <stdio.h>

char	**path_splitter(t_data *data)
{
	int		i;
	char	*path;
	t_node	*cp;

	i = 0;
	path = NULL;
	cp = data->envp;
	while (cp != NULL)
	{
		if (!ft_strncmp(cp->content, "PATH=", 5))
		{
			path = cp->content;
			break ;
		}
		cp = cp->next;
	}
	if (path == NULL)
		return (NULL);
	return (ft_split(path + 5, ':'));
}

char	*path_check_access(char **all_p, char **args)
{
	int		i;
	char	*ret;
	char	*temp;

	i = 0;
	while (all_p[i] != NULL)
	{
		temp = ft_strjoin(all_p[i], "/");
		free (all_p[i]);
		all_p[i] = ft_strjoin(temp, args[0]);
		free (temp);
		temp = NULL;
		if (!access(all_p[i], F_OK) && !access(all_p[i], X_OK))
		{
			ret = ft_strdup(all_p[i]);
			free_char_array(&all_p);
			return (ret);
		}
		i++;
	}
	free_char_array(&all_p);
	return (NULL);
}

int	path_main(t_data *data, char **args)
{
	char	**all_p;
	char	*to_app;

	all_p = path_splitter(data);
	if (all_p == NULL)
		return (-1);
	to_app = path_check_access(all_p, args);
	if (to_app == NULL)
		return (-1);
	free (args[0]);
	args[0] = to_app;
	return (0);
}
