/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 17:21:48 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/06 13:10:42 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include "lexer.h"
#include "struct.h"
#include <stdlib.h>
#include <stdio.h>

int	export_detach_helper_envp(t_node *envp, char *new, int len, char *equalSign)
{
	while (envp != NULL && len > 0)
	{
		if (!ft_strncmp(envp->content, new, len))
		{
			if (equalSign != NULL)
			{
				if (envp->content != NULL)
					free(envp->content);
				envp->content = ft_strdup(new);
				return (2);
			}
			else if (((char *)envp->content)[len] == '=')
				return (3);
		}
		envp = envp->next;
	}
	return (0);
}

int	export_checkoverwrite(t_data *data, char *new)
{
	t_node	*envp;
	int		len;
	char	*ptr_equal_sign;
	int		ret;

	envp = data->envp;
	ptr_equal_sign = ft_strchr(new, '=');
	if (ptr_equal_sign == NULL)
		len = ft_strlen(new);
	else
		len = ptr_equal_sign - new + 1;
	ret = export_detach_helper_envp(envp, new, len, ptr_equal_sign);
	if (ret != 2 && ret != 3)
	{
		if (ptr_equal_sign == NULL)
			ret = 1;
		ret = export_check_var_wo_value_list(data, new, ret);
	}
	return (ret);
}

int	export_add_nodes(t_data *data, char **argv, int i, int ret)
{
	t_node	*new_node;

	if (ret == 0 || ret == 1)
	{
		new_node = ft_t_node_create(ft_strdup(argv[i]));
		if (new_node == NULL)
			return (1);
		if (ret == 0)
			ft_t_node_add_back(&(data->envp), new_node);
		else
			ft_t_node_add_back(&(data->expo), new_node);
	}
	return (0);
}

int	export_main(char **argv, t_data *data)
{
	int		i;
	int		es;
	int		es_cpy;

	es_cpy = 0;
	if (argv[1] == NULL)
		export_print_list(data);
	if (argv[1] == NULL)
		return (0);
	i = 0;
	while (argv[++i] != NULL)
	{
		es = builtin_invalid_name_check(argv, i, "export");
		if (es == 1)
			es_cpy = es;
		if (es == 1)
			continue ;
		es = export_checkoverwrite(data, argv[i]);
		if (export_add_nodes(data, argv, i, es))
		{
			es_cpy = 2;
			break ;
		}
	}
	return (es_cpy);
}
