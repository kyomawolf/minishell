/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_append1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 16:46:20 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/08 00:07:21 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "lexer.h"
#include "libft.h"

//handles wildcard appandage
static int	ft_append_wildcard(char *input, int *i, t_word *word)
{
	if (input[*i] == '*' && (word->status == '\'' || word->status == '"'))
	{
		if (ft_t_word_append_char(word, '*'))
			return (1);
		if (ft_t_word_append_char(word, WC_QUOTED))
			return (1);
	}
	else
	{
		if (ft_t_word_append_char(word, '*'))
			return (1);
		if (ft_t_word_append_char(word, WC_END))
			return (1);
	}
	return (0);
}

//manages appendage of characters PART III
static int	ft_app_helper_op(char *input, int *i, t_word *word, t_node **head)
{
	char	c;

	if (word->status == VAR_UQUOTED && (input[*i] == '(' || input[*i] == ')'))
	{
		if (ft_t_word_append_char(word, input[*i]))
			return (1);
		if (ft_terminate_token(input, i, word, head) == -1)
			return (1);
		return (2);
	}
	else if (word->status == VAR_UQUOTED && ft_strchr("<>|&", input[*i]))
	{
		c = input[*i];
		while (input[*i] == c)
		{
			if (ft_t_word_append_char(word, input[(*i)++]))
				return (1);
		}
		(*i)--;
		if (ft_terminate_token(input, i, word, head) == -1)
			return (1);
		return (2);
	}
	return (0);
}

//manages appendage of characters PART II
static int	ft_append_helper(char *input, int *i, t_word *word, t_node **head)
{
	int	ret;

	ret = ft_app_helper_op(input, i, word, head);
	if (ret == 1)
		return (1);
	else if (ret == 2)
		return (0);
	else if (ret == 0)
	{
		if (ft_t_word_append_char(word, input[*i]))
			return (1);
	}
	return (ret);
}

// manages appendage of variables or wildcards
static int	ft_app_var_or_wc(char *input, int *i, t_word *word, t_node **head)
{
	int	ret;

	ret = 0;
	if (input[*i] == '$')
	{
		ret = 1;
		if (ft_append_variable(input, i, word, head) == -1)
			return (-1);
	}
	else if (input[*i] == '*')
	{
		ret = 1;
		if (ft_append_wildcard(input, i, word))
			return (-1);
	}
	return (ret);
}

//manages appendage of characters PART I
int	ft_append(char *input, int *i, t_word *word, t_node **head)
{
	int	ret;

	if (input[*i] == '\0')
		return (0);
	else if (word->status == '\'' && input[*i] != '*' && input[*i] != '$')
	{
		if (ft_t_word_append_char(word, input[*i]))
			return (-1);
	}
	else if (input[*i] == '$' && (input[*i + 1] == '\'' || input[*i + 1] == '"')
		&& (input[*i + 2] != '\0' && input[*i + 2] != ' '))
		return (0);
	else
	{
		ret = ft_app_var_or_wc(input, i, word, head);
		if (ret == -1)
			return (-1);
		else if (ret == 0)
		{
			if (ft_append_helper(input, i, word, head))
				return (-1);
		}
	}
	return (0);
}
