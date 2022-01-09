/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokenlist.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 16:54:58 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/09 13:30:39 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "struct.h"
#include "lexer.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//changes quotation status
static int	ft_change_status(char *input, int *i, t_word *word, t_node **head)
{
	if (word->status == VAR_UQUOTED && (input[*i] == '"' || input[*i] == '\''))
	{
		word->status = input[*i];
		if (input[*i + 1] == word->status && word->w_head == 0
			&& ft_strlen(input) > (size_t)(*i + 1)
			&& ft_strchr(" \t\n", input[*i + 2]))
		{
			if (ft_t_token_add_empty_string(word, head, i))
				return (-1);
		}
		else
		{
			word->type = word->status;
			(*i)++;
		}
		return (1);
	}
	else if (input[*i] == word->status)
	{
		word->status = VAR_UQUOTED;
		(*i)++;
		return (1);
	}
	return (0);
}

static int	ft_token_list_handle_last_word(t_word *word, t_node **head, int sw)
{
	if (word->w_head != 0)
	{
		if (ft_append_token(word, head, 1))
			return (1);
	}
	if (word->chars != NULL)
	{
		free(word->chars);
		word->chars = NULL;
	}
	if (word->status == '\'' || word->status == '"' || sw == 1)
	{
		free_t_node_list2(head);
		if (sw == 0)
			ft_putstr_fd("minishell: missing quote\n", 2);
	}
	return (0);
}

static int	ft_get_token_list_actions(t_data *data, int *i, t_word *word, \
										t_node **head)
{
	int		ret;

	ret = ft_terminate_token(data->input, i, word, head);
	if (ret == 1)
		return (1);
	else if (ret == -1)
		return (2);
	ret = ft_change_status(data->input, i, word, head);
	if (ret == 1)
		return (1);
	else if (ret == -1)
		return (2);
	ret = ft_append(data->input, i, word, head);
	if (ret == 1)
		return (3);
	else if (ret == -1)
		return (2);
	return (0);
}

static int	ft_gtl_input_loop(t_data *data, t_word *word, t_node **head, \
								int *sw)
{
	int	i;
	int	ret;

	i = 0;
	ft_skip_set(data->input, &i, " \t\n");
	while (data->input[i] != '\0')
	{
		ret = ft_get_token_list_actions(data, &i, word, head);
		if (ret == 1)
			continue ;
		else if (ret == 2)
			break ;
		else if (ret == 3)
		{
			*sw = 1;
			break ;
		}
		i++;
	}
	return (ret);
}

t_node	*ft_get_token_list(t_data *data)
{
	int		sw;
	int		ret;
	t_word	word;
	t_node	*head;

	sw = 0;
	head = NULL;
	if (ft_t_word_init(&word))
		return (NULL);
	ret = ft_gtl_input_loop(data, &word, &head, &sw);
	if (ret == 2)
	{
		ft_free_lexer(&word, &head, data);
		return (NULL);
	}
	if (ft_token_list_handle_last_word(&word, &head, sw))
		ft_free_lexer(&word, &head, data);
	return (head);
}
