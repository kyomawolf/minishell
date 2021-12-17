/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 21:07:35 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/17 03:46:39 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "structs.h"
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
		if (access(all_p[i], F_OK) && access(all_p[i], X_OK))
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

int	path_replace(char **args, char *to_app)
{
	/* char	*temp;
	int		i;
	int		ii; */

	free (args[0]);
	args[0] = to_app;
	return (0);
	/* temp = ft_calloc(ft_strlen(args[0] + ft_strlen(to_app)) + 1, 1);
	if (temp == NULL)
		return (1);
	i = 0;
	ii = 0;
	while (to_app[i] != '\0' || args[0][ii] != '\0')
	{
		if (to_app[i] != '\0')
		{
			temp[i] = to_app[i];
			i++;
		}
		else if (args[0][ii] != '\0')
		{
			to_app[i + ii] = args[0][ii];
			ii++;
		}
	}
	free(args[0]);
	args[0] = temp;
	return (0); */
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
	path_replace(args, to_app);
	printf("path main %s\n", args[0]);
	return (0);
}
