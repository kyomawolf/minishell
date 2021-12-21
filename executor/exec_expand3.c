/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 20:10:27 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/21 22:03:05 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "exec.h"
#include "lexer.h"
#include "libft.h"
#include <stdlib.h>

// terminates a word. Addes token to the token list
static void	ft_terminate_word(t_word *word, t_node **list)
{
	if (word->write_head != 0)
		ft_terminate_word_and_add_to_list(list, &word);
	if (word->chars != NULL)
	{
		free(word->chars);
		word->chars = NULL;
	}
	free(word);
	word = NULL;
}

void	ft_t_expand_init(t_expand *exp_data)
{
	exp_data->i = 0;
	exp_data->list = NULL;
	exp_data->word = ft_calloc(1, sizeof(t_word));
	ft_t_word_init(exp_data->word);
}

// initializes the variable expansion
t_node	*ft_init_var_expansion(t_node **head, t_data *data, t_expand *exp_data)
{
	t_token	*token;
	char	*temp;

	token = (t_token *)(*head)->content;
	temp = token->string;
	while (temp[exp_data->i] != '\0')
	{
		if (temp[exp_data->i] == '$')
		{
			if (ft_handle_var_expansion(head, exp_data, data))
				return (NULL);
			exp_data->i++;
			if (temp[exp_data->i] == '\0')
			{
				ft_t_word_append_char(exp_data->word, temp[exp_data->i]);//need?
				break ;
			}
			continue ;
		}
		ft_t_word_append_char(exp_data->word, temp[exp_data->i++]);
	}
	ft_terminate_word(exp_data->word, &exp_data->list);
	return (exp_data->list);
}

static void	ft_exchange_tokens_helper(t_node **head, t_node *list)
{
	t_node	*last_in_list;

	if ((*head)->prev != NULL)
		((t_node *)(*head)->prev)->next = list;
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
int	ft_exchange_tokens(t_node **head, t_node *list, t_expand *exp_data)
{
	t_node	*temp;

	if (((t_token *)list->content)->string[0] == '\0' \
		&& exp_data->quote_status == VAR_UQUOTED)
	{
		temp = *head;
		*head = (*head)->next;
		ft_t_node_detach_and_free(temp);
		free (((t_token *)list->content)->string);
		((t_token *)list->content)->string = NULL;
		free (((t_token *)list->content));
		list->content = NULL;
		return (1);
	}
	else
		ft_exchange_tokens_helper(head, list);
	return (0);
}
