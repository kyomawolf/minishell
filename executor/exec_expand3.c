/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 20:10:27 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/09 01:25:38 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "struct.h"
#include "exec.h"
#include "lexer.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h>

// terminates a word. Addes token to the token list
static void	ft_terminate_word(t_expand *exp_data)
{
	if (exp_data->word->write_head != 0)
		ft_terminate_word_and_add_to_list(exp_data);
	if (exp_data->word->chars != NULL)
	{
		free(exp_data->word->chars);
		exp_data->word->chars = NULL;
	}
	free(exp_data->word);
	exp_data->word = NULL;
}

// initializes the variable expansion
t_node	*ft_init_var_expansion(t_node **head, t_data *data, t_expand *exp_data)
{
	t_token	*token;
	char	*temp;
	int		es;

	token = (t_token *)(*head)->content;
	temp = token->string;
	while (temp[exp_data->i] != '\0')
	{
		es = ft_init_handle_var_expansion(head, data, exp_data, temp);
		if (es == 1 || es == 3)
			continue ;
		else if (es == 2)
			break ;
		ft_t_word_append_char(exp_data->word, temp[exp_data->i++]);
	}
	ft_terminate_word(exp_data);
	return (exp_data->list);
}

void	ft_exchange_tokens_free_exp_data_word(t_expand *exp_data)
{
	if (exp_data->word != NULL)
	{
		if (exp_data->word->chars != NULL)
		{
			free(exp_data->word->chars);
			exp_data->word->chars = NULL;
		}
		free(exp_data->word);
		exp_data->word = NULL;
	}
}

void	ft_exchange_tokens_helper(t_node **head, t_node *list, \
									t_node **temp_ori)
{
	t_node	*last_in_list;

	if ((*head)->prev != NULL)
		((t_node *)(*head)->prev)->next = list;
	else
		*temp_ori = list;
	last_in_list = ft_t_node_get_last(list);
	if ((*head)->next != NULL)
		((t_node *)(*head)->next)->prev = last_in_list;
	list->prev = (*head)->prev;
	last_in_list->next = (*head)->next;
	free(((t_token *)(*head)->content)->string);
	((t_token *)(*head)->content)->string = NULL;
	free((t_token *)(*head)->content);
	(*head)->content = NULL;
	free(*head);
	*head = last_in_list;
}

// exchanges token with information about var_expansion with expaned tokens
int	ft_exchange_tokens_var(t_node ***head, t_expand *exp_data, \
							t_node **temp_ori)
{
	t_node	*temp;

	if (*head != NULL && (**head) != NULL \
		&& ((t_token *)exp_data->list->content)->string[0] == '\0' \
		&& exp_data->quote_status == VAR_UQUOTED)
	{
		temp = detach_node(*head, **head);
		free (((t_token *)temp->content)->string);
		free ((t_token *)temp->content);
		if (**head != NULL)
			**head = (**head)->next;
		else
			*temp_ori = NULL;
		free (temp);
		free (((t_token *)exp_data->list->content)->string);
		free(((t_token *)exp_data->list->content));
		exp_data->list->content = NULL;
		free(exp_data->list);
		exp_data->list = NULL;
		ft_exchange_tokens_free_exp_data_word(exp_data);
		return (2);
	}
	else
		ft_exchange_tokens_helper(*head, exp_data->list, temp_ori);
	return (0);
}
