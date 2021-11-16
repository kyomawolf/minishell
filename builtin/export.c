/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 17:21:48 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/16 20:48:12 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include <stdlib.h>

int	chararr_len(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
		i++;
	return (i);
}

int	export_fill_array(char ***envp, char **new_envp, char *new, int amount)
{
	int	i;

	i = 0;
	while (i < amount)
	{
		new_envp[i] = ft_strdup(*(envp[i]));
		if (new_envp[i] == NULL)
		{
			free_char_arr(new_envp);
			return (-1);
		}
		i++;
	}
	new_envp[i] = ft_strdup(new);
	if (new_envp[i] == NULL)
	{
		free_char_arr(new_envp);
		return (-1);
	}
	return (0);
}

int	export_set_value(char **envp, int i, char *str)
{
	char	*temp;

	temp = envp[i];
	envp[i] = ft_strdup(str);
	if (envp[i] == NULL)
	{
		envp[i] = temp;
		return (0);
	}
	else
	{
		free(temp);
	}
	return (1);
}

int	export_check_input(char *str, char **envp)
{
	int		i;
	char	*name;
	int		name_len;
	char	*point;

	i = 0;
	while (str[i] != NULL && str[i] != '=')
		name[i] = str[i++];
	name_len = i;
	if (str[name_len] == '\0')
		return (0);
	i = 0;
	while (envp[i] != NULL)
	{
		point = ft_strnstr(envp[i], name, ft_strlen(envp[i]));
		if (point != NULL)
		{
			if (point + name_len == '=')
				break ;
		}
		i++;
	}
	if (envp[i] != NULL && !export_set_val(envp, i, str))
		return (0);
	return (1);
}

//WARNING NEW HAS TO BE FREED OUT OF EXPORT
int	export_main(char ***envp, char *new)
{
	int		amount;
	int		i;
	char	**new_envp;

	if (!export_check_input(new, *envp))
		return (-1);
	amount = chararr_len(*envp);
	new_envp = ft_calloc(amount + 2, sizeof(char *));
	if (new_envp == NULL)
		return (-1);
	if (export_fill_array(envp, new_envp, new, amount) == -1)
		return (-1);
	free_char_arr(envp);
	envp = &new_envp;
	return (0);
}
