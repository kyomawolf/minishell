/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_append2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 16:48:27 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/08 00:32:27 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "lexer.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h>

static int	ft_append_variable_var_name(t_word *word, char *input, int *i)
{
	char	*var_name;
	int		len;

	var_name = ft_lexer_get_var_name(input, *i + 1);
	if (var_name == NULL)
		return (1);
	len = ft_strlen(var_name);
	while (len > 0)
	{
		if (ft_t_word_append_char(word, input[++(*i)]))
		{
			free(var_name);
			return (1);
		}
		len--;
	}
	free(var_name);
	var_name = NULL;
	return (0);
}

static int	ft_append_variable_prefix(t_word *word, char *input, int *i)
{
	if (ft_isdigit(input[*i + 1]))
	{
		*i = *i + 1;
		return (0);
	}
	if (ft_t_word_append_char(word, input[*i]))
		return (-1);
	if (word->status == VAR_UQUOTED)
	{
		if (ft_t_word_append_char(word, VAR_UQUOTED))
			return (-1);
	}
	else if (word->status == '"')
	{
		if (ft_t_word_append_char(word, VAR_DQUOTED))
			return (-1);
	}
	else if (word->status == '\'')
	{
		if (ft_t_word_append_char(word, VAR_SQUOTED))
			return (-1);
	}
	return (1);
}

static int	ft_append_real_variable(char *input, int *i, t_word *word)
{
	int	ret;

	ret = ft_append_variable_prefix(word, input, i);
	if (ret == 0)
		return (0);
	else if (ret == -1)
		return (-1);
	if (ft_append_variable_var_name(word, input, i))
		return (-1);
	else
	{
		if (ft_t_word_append_char(word, VAR_END))
			return (-1);
	}
	return (ret);
}

// is called if variable is found: checks for
// valid var_name, gets var_value, checks for wildcard character
// and appends chars to word and terminates word if necessary
//new: status of var is stored in char after $.
int	ft_append_variable(char *input, int *i, t_word *word, t_node **head)
{
	t_node	*last_node;
	int		ret;

	ret = 0;
	last_node = ft_t_node_get_last(*head);
	if (last_node != NULL && ((t_token *)last_node->content)->type == HERE_DOC)
	{
		if (ft_t_word_append_char(word, input[*i]))
			return (-1);
	}
	else
	{
		ret = ft_append_real_variable(input, i, word);
		if (ret == 0)
			return (0);
		else if (ret == -1)
			return (-1);
	}
	return (ret);
}
