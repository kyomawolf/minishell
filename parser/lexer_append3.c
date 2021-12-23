/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_append3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 16:49:54 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/23 22:46:39 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "libft.h"
#include "lexer.h"
#include <stdlib.h>

//defines type of the token (operator or word)
//&&word->w_head == 2 (at && check)
char	ft_set_type(t_word *word)
{
	char	ret;

	ret = WORD;
	if (word->type == -1)
	{
		if (word->chars[0] == '&' && word->chars[1] == '&')
			ret = AND;
		else if (word->chars[0] == '|' && word->write_head == 1)
			ret = PIPE;
		else if (word->chars[0] == '|' && word->chars[1] == '|')
			ret = OR;
		else if (word->chars[0] == '(' && word->write_head == 1)
			ret = OPAR;
		else if (word->chars[0] == ')' && word->write_head == 1)
			ret = CPAR;
		else if (word->chars[0] == '<' && word->write_head == 1)
			ret = IRD;
		else if (word->chars[0] == '<' && word->chars[1] == '<')
			ret = HERE_DOC;
		else if (word->chars[0] == '>' && word->write_head == 1)
			ret = ORD_TRC;
		else if (word->chars[0] == '>' && word->chars[1] == '>')
			ret = ORD_APP;
	}
	return (ret);
}

//creates t_token * and t_node * and adds t_node * to the back of the list
int	ft_append_token_helper(char *str, t_node **head, int type, int status)
{
	t_token	*token;
	t_node	*node;

	token = ft_t_token_create(str);
	if (token == NULL)
	{
		free (str);
		str = NULL;
		return (1);
	}
	token->type = type;
	token->quote_status = status;
	node = ft_t_node_create(token);
	if (node == NULL)
	{
		free(str);
		str = NULL;
		free(token);
		token = NULL;
		return (1);
	}
	ft_t_node_add_back(head, node);
	return (0);
}

//gets the type of the token, does the appendage and if necessary inits
//a new t_word *
int	ft_append_token(t_word *word, t_node **head, int end)
{
	char	*str;
	int		token_type;
	int		quote_status;

	quote_status = word->type;
	token_type = ft_set_type(word);
	str = ft_t_word_get_str(word);
	if (str == NULL)
		return (1);
	if (ft_append_token_helper(str, head, token_type, quote_status))
		return (1);
	if (end == 0)
		ft_t_word_init(word);
	return (0);
}

void	ft_t_token_add_empty_string(t_word *word, t_node **head, int *i)
{
	ft_t_word_append_char(word, '\0');
	ft_append_token(word, head, 0);
	(*i) = (*i) + 2;
}

// terminates_token on unquoted metacharacter, inits appendage of token
int	ft_terminate_token(char *input, int *i, t_word *word, t_node **head)
{
	int	ret;

	ret = 0;
	if (word->status == -1 && word->write_head != 0)
	{
		if (ft_strchr(" \t\n", input[*i]))
			ret = 2;
		else if (ft_strchr("<>|&()", input[*i]))
			ret = 1;
		if (ret > 0)
			ft_append_token(word, head, 0);
		if (ret == 2)
			ft_skip_set(input, i, " \t\n");
	}
	else if (word->status == -1 && word->write_head == 0)
		ft_skip_set(input, i, " \t\n");
	return (ret);
}
