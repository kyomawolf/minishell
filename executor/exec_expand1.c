/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 19:57:44 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/23 16:32:10 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "exec.h"
#include "libft.h"

//loops through string and searches for char $.
// if necessary calls variable expansion initialization function
//  and calls function to exchange the old token with the new ones.
static int	ft_t_token_var_expansion_check(t_node **head, t_data *data)
{
	t_token		*token;
	t_node		*list;
	t_expand	exp_data;

	ft_t_expand_init(&exp_data);
	token = (t_token *)(*head)->content;
	if (token->type == WORD && ft_strchr(token->string, '$'))
	{
		list = ft_init_var_expansion(head, data, &exp_data);
		if (list == NULL)
			return (-1);
		if (ft_exchange_tokens(head, &exp_data))
			return (1);
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
	return (0);
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
			ft_str_array_var_expansion(&(tree->io->input), data);
			ft_str_array_var_expansion(&(tree->io->output), data);
		}
	}
	return (0);
}

int	ft_t_token_variable_expansion(t_node **head_token, t_data *data)
{
	t_node	*temp;
	int		ret;

	while (*head_token != NULL)
	{
		ret = ft_t_token_var_expansion_check(head_token, data);
		if (ret == 1)
			continue ;
		else if (ret == -1)
			return (1);
		temp = *head_token;
		*head_token = (*head_token)->next;
	}
	ret = ft_get_beginning_of_list(temp, head_token);
	return (ret);
}

int	ft_t_bin_variable_expansion(t_node *head, t_data *data)
{
	while (head != NULL && head->content != NULL)
	{
		if (ft_t_bin_var_expansion_check(head, data))
			return (1);
		head = head->next;
	}
	return (0);
}
