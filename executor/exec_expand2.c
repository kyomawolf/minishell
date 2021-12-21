/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 20:05:19 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/21 20:08:40 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "exec.h"
#include "libft.h"
#include "lexer.h"
#include <stdlib.h>

static void	ft_exchange_str_arr(char ***str_arr, t_node *head)
{
	int		len;
	int		i;
	t_node	*temp;

	len = ft_t_node_size(head);
	*str_arr = ft_calloc(len + 1, sizeof(char *));
	i = 0;
	while (head != NULL)
	{
		(*str_arr)[i] = ((t_token *)head->content)->string;
		free((t_token *)head->content);
		head->content = NULL;
		temp = head;
		head = head->next;
		free(temp);
		temp = NULL;
		i++;
	}
	(*str_arr)[i] = NULL;
}

static t_node	*convert_str_arr_to_t_node(char **sel_dir)
{
	t_node	*ret;
	t_node	*curr;
	int		i;

	curr = ft_calloc(1, sizeof(t_node));
	curr->content = ft_calloc(1, sizeof(t_token));
	if (sel_dir[0] == NULL)
		return (NULL);
	((t_token *)curr->content)->string = ft_strdup(sel_dir[0]);
	((t_token *)curr->content)->type = WORD;
	ret = curr;
	i = 1;
	while (sel_dir[i] != NULL)
	{
		curr->next = ft_calloc(1, sizeof(t_node));
		((t_node *)curr->next)->prev = curr;
		curr = curr->next;
		curr->content = ft_calloc(1, sizeof(t_token));
		((t_token *)curr->content)->string = ft_strdup(sel_dir[i]);
		((t_token *)curr->content)->type = WORD;
		i++;
	}
	return (ret);
}

int	ft_str_array_var_expansion(char ***str_arr, t_data *data)
{
	t_node	*head_token;

	if (*str_arr != NULL)
	{
		head_token = NULL;
		head_token = convert_str_arr_to_t_node(*str_arr);
		if (head_token == NULL)
			return (0);
		if (ft_t_token_variable_expansion(&head_token, data))
			return (1);
		if (ft_wildcard_expansion(&head_token))
			return (1);
		ft_exchange_str_arr(str_arr, head_token);
	}
	return (0);
}
