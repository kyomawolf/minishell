/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 19:57:44 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/06 21:41:35 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "struct.h"
#include "exec.h"
#include "libft.h"
#include <stdio.h>

//loops through string and searches for char $.
// if necessary calls variable expansion initialization function
//  and calls function to exchange the old token with the new ones.
static int	ft_t_token_var_expansion_check(t_node ***head, t_data *data)
{
	t_token		*token;
	t_expand	exp_data;
	int			ret;

	ft_t_expand_init(&exp_data);
	token = (t_token *)(**head)->content;
	ret = 0;
	if (token->type == WORD && ft_strchr(token->string, '$'))
	{
		exp_data.list = ft_init_var_expansion(*head, data, &exp_data);
		if (exp_data.list == NULL)
			return (-1);
		ret = ft_exchange_tokens_var(head, &exp_data);
	}
	if (exp_data.word != NULL)
	{
		if (exp_data.word->chars != NULL)
		{
			free (exp_data.word->chars);
			exp_data.word->chars = NULL;
		}
		free(exp_data.word);
		exp_data.word = NULL;
	}
	return (ret);
}

static int	ft_t_bin_var_expansion_check(t_node *head, t_data *data)
{
	t_bin	*tree;

	tree = (t_bin *)head->content;
	if (tree == NULL)
		return (0);
	else
	{
		if (tree->command->arguments != NULL)
			ft_str_array_var_expansion(&(tree->command->arguments), data);
		if (tree->io != NULL)
		{
			ft_t_io_heredoc_var_expansion(tree->io, data);
			ft_t_io_redir_expansion(tree->io, data);
		}
	}
	return (0);
}

void	ft_t_node_t_token_detach_n_free(t_node **head_token, t_node **temp_ori)
{
	t_node	*temp;

	temp = (*head_token);
	if ((*head_token)->next == NULL && (*head_token)->prev == NULL)
		*head_token = NULL;
	else if (*head_token && (*head_token)->prev == NULL)
	{
		*head_token = (*head_token)->next;
		(*head_token)->prev = NULL;
	}
	else if ((*head_token) && (*head_token)->next == NULL)
		((t_node *)(*head_token)->prev)->next = NULL;
	else if ((*head_token))
	{
		((t_node *)(*head_token)->next)->prev = (*head_token)->prev;
		((t_node *)(*head_token)->prev)->next = (*head_token)->next;
	}
	if (*head_token && (*head_token)->prev != NULL)
		*head_token = (*head_token)->next;
	else
		*temp_ori = *head_token;
	free (((t_token *)temp->content)->string);
	free (temp->content);
	free (temp);
}

int	ft_t_token_variable_expansion(t_node **head_token, t_data *data)
{
	t_node	*temp_ori;
	int		ret;

	temp_ori = *head_token;
	while (*head_token != NULL)
	{
		ret = ft_t_token_var_expansion_check(&head_token, data);
		if (ret == 1 || ret == 2)
			continue ;
		else if (ret == -1)
		{
			ft_t_node_t_token_detach_n_free(head_token, &temp_ori);
			continue ;
		}
		if (*head_token)
			*head_token = (*head_token)->next;
		else
			return (0);
	}
	ret = ft_get_beginning_of_list(temp_ori, head_token);
	return (ret);
}

int	ft_t_bin_variable_expansion(t_node *head, t_data *data)
{
	if (head != NULL && head->content != NULL)
	{
		if (ft_t_bin_var_expansion_check(head, data))
			return (1);
	}
	return (0);
}
