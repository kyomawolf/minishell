/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/29 01:36:31 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/29 01:43:06 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include "lexer.h"
#include "struct.h"
#include <stdlib.h>
#include <stdio.h>

void	export_print_list(t_data *data)
{
	t_node	*temp;

	if (data->envp != NULL)
	{
		temp = data->envp;
		while (temp)
		{
			printf("declare -x %s\n", temp->content);
			temp = temp->next;
		}
	}
	if (data->expo != NULL)
	{
		temp = data->expo;
		while (temp != NULL)
		{
			printf("declare -x %s\n", temp->content);
			temp = temp->next;
		}
	}
}

int	export_detach_helper_expo(t_data *data, char *new, int ret_case)
{
	int		len;
	t_node	*expo_cp;

	len = ft_strlen(new);
	if (ret_case == 0)
		len = ft_strchr(new, '=') - new;
	expo_cp = data->expo;
	while (expo_cp && len > 0)
	{
		if (!ft_strncmp(expo_cp->content, new, len))
		{
			if (ret_case == 0)
			{
				free(expo_cp->content);
				free(detach_node(&data->expo, expo_cp));
				expo_cp = NULL;
				break ;
			}
			else
				return (3);
		}
		expo_cp = expo_cp->next;
	}
	return (ret_case);
}

int	export_check_var_wo_value_list(t_data *data, char *new, int ret_case)
{
	int	ret;

	ret = ret_case;
	if (data->expo == NULL)
		return (ret);
	ret = export_detach_helper_expo(data, new, ret_case);
	return (ret);
}
