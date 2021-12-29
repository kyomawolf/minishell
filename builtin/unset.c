/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 22:45:31 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/29 01:34:02 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include "struct.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

t_node	*detach_node(t_node **head, t_node *node);

static void	unset_check_expo_list(char **argv, t_data *data, int i)
{
	t_node	*expo_cp;
	int		len;

	expo_cp = data->expo;
	len = ft_strlen(argv[i]);
	while (expo_cp != NULL && len > 0)
	{
		if (!ft_strncmp(expo_cp->content, argv[i], len))
		{
			free(expo_cp->content);
			free(detach_node(&data->expo, expo_cp));
			expo_cp = NULL;
			break ;
		}
		expo_cp = expo_cp->next;
	}
}

void	print_error_inv_ident(char **argv, int i, char *name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(argv[i], 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

int	builtin_invalid_name_check(char **argv, int i, char *name)
{
	int	j;
	int	es;

	es = 0;
	j = 0;
	if (ft_isalpha(argv[i][j]) || argv[i][j] == '_')
		j++;
	else
	{
		print_error_inv_ident(argv, i, name);
		es = 1;
	}
	if (es != 1)
	{
		while (ft_isalnum(argv[i][j]) || argv[i][j] == '_')
			j++;
		if (argv[i][j] != '\0' && argv[i][j] != '=')
		{
			print_error_inv_ident(argv, i, name);
			es = 1;
		}
	}
	return (es);
}

static void	unset_detach(t_data *data, char **argv, int i)
{
	t_node	*envp_cp;
	int		len;

	envp_cp = data->envp;
	len = ft_strlen(argv[i]);
	while (envp_cp != NULL && len > 0)
	{
		if (!ft_strncmp(envp_cp->content, argv[i], len))
		{
			free(envp_cp->content);
			free(detach_node(&data->envp, envp_cp));
			break ;
		}
		envp_cp = envp_cp->next;
	}
}

int	unset_main(char **argv, t_data *data)
{
	int		i;
	int		es;
	int		es_cpy;

	es = 0;
	es_cpy = es;
	i = -1;
	while (argv[++i] != NULL)
	{
		es = builtin_invalid_name_check(argv, i, "unset");
		if (es == 1)
		{
			es_cpy = es;
			continue ;
		}
		unset_detach(data, argv, i);
		unset_check_expo_list(argv, data, i);
	}
	return (es_cpy);
}
