/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_append1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 16:46:20 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/22 22:52:06 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "lexer.h"
#include "libft.h"

//handles wildcard appandage
static int	ft_append_wildcard(char *input, int *i, t_word *word)
{
	if (input[*i] == '*' && (word->status == '\'' || word->status == '"'))
	{
		if (ft_t_word_append_char(word, '*') == -1)
			return (-1);
		if (ft_t_word_append_char(word, WC_QUOTED) == -1)
			return (-1);
		return (1);
	}
	else
	{
		if (ft_t_word_append_char(word, '*') == -1)
			return (-1);
		if (ft_t_word_append_char(word, WC_END) == -1)
			return (-1);
	}
	return (0);
}

//manages appendage of characters PART II
static void	ft_append_helper(char *input, int *i, t_word *word, t_node **head)
{
	char	c;

	if (word->status == -1 && (input[*i] == '(' || input[*i] == ')'))
	{
		ft_t_word_append_char(word, input[*i]);
		ft_terminate_token(input, i, word, head);
	}
	else if (word->status == -1 && ft_strchr("<>|&", input[*i]))
	{
		c = input[*i];
		while (input[*i] == c)
			ft_t_word_append_char(word, input[(*i)++]);
		(*i)--;
		ft_terminate_token(input, i, word, head);
	}
	else
		ft_t_word_append_char(word, input[*i]);
}

//manages appendage of characters PART I
int	ft_append(char *input, int *i, t_word *word, t_node **head)
{
	if (input[*i] == '\0')
		return (0);
	else if (word->status == '\'' && input[*i] != '*' && input[*i] != '$')
		ft_t_word_append_char(word, input[*i]);
	else if (input[*i] == '$')
	{
		if (ft_append_variable(input, i, word, head))
		{
			ft_putstr_fd("Error: invalid variable name\n", 2);
			return (1);
		}
	}
	else if (input[*i] == '*')
		ft_append_wildcard(input, i, word);
	else
		ft_append_helper(input, i, word, head);
	return (0);
}
