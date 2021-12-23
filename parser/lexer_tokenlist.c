/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokenlist.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 16:54:58 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/22 22:35:24 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "structs.h"
#include "lexer.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h>

//changes quotation status
static int	ft_change_status(char *input, int *i, t_word *word, t_node **head)
{
	int	ret;

	ret = 0;
	if (word->status == -1 && (input[*i] == '"' || input[*i] == '\''))
	{
		word->status = input[*i];
		if (input[*i + 1] == word->status && word->write_head == 0
			&& ft_strlen(input) > (size_t)(*i + 1)
			&& ft_strchr(" \t\n", input[*i + 2]))
			ft_t_token_add_empty_string(word, head, i);
		else
		{
			word->type = word->status;
			(*i)++;
		}
		ret = 1;
	}
	else if (input[*i] == word->status)
	{
		word->status = -1;
		(*i)++;
		ret = 1;
	}
	return (ret);
}

static void	ft_token_list_handle_last_word(t_word *word, t_node **head, int sw)
{
	if (word->write_head != 0)
		ft_append_token(word, head, 1);
	if (word->chars != NULL)
	{
		free(word->chars);
		word->chars = NULL;
	}
	if (word->status == '\'' || word->status == '"' || sw == 1)
	{
		free_t_node_list2(head);
		if (sw == 0)
			printf("missing quote\n");
	}
	return ;
}

t_node	*ft_get_token_list(char *input)
{
	int		i;
	int		sw;
	t_word	word;
	t_node	*head;

	sw = 0;
	head = NULL;
	ft_t_word_init(&word);
	i = 0;
	ft_skip_set(input, &i, " \t\n");
	while (input[i] != '\0')
	{
		if (ft_terminate_token(input, &i, &word, &head))
			continue ;
		if (ft_change_status(input, &i, &word, &head))
			continue ;
		if (ft_append(input, &i, &word, &head))
		{
			sw = 1;
			break ;
		}
		i++;
	}
	ft_token_list_handle_last_word(&word, &head, sw);
	return (head);
}
