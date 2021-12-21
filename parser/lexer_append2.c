/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_append2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 16:48:27 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/21 17:55:00 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "lexer.h"
#include "libft.h"
#include <stdlib.h>

//checks if var_name is valid and returns string with var_name
static char	*ft_get_var_name(char *str, int i)
{
	int		i_start;
	char	*var_name;

	i_start = i;
	if (str[i] == '?')
	{
		var_name = malloc(2);
		var_name[0] = '?';
		var_name[1] = '\0';
		return (var_name);
	}
	while (ft_isalnum(str[i]) || str[i] == '_')
		i++;
	var_name = ft_substr(str, i_start, i - i_start);
	return (var_name);
}

static int	ft_append_variable_var_name(t_word *word, char *input, int *i)
{
	char	*var_name;
	int		len;

	var_name = ft_get_var_name(input, *i + 1);
	if (var_name == NULL)
		return (1);
	len = ft_strlen(var_name);
	while (len > 0)
	{
		ft_t_word_append_char(word, input[++(*i)]);
		len--;
	}
	free(var_name);
	var_name = NULL;
	return (0);
}

static void	ft_append_variable_prefix(t_word *word, char *input, int *i)
{
	ft_t_word_append_char(word, input[*i]);
	if (word->status == -1)
		ft_t_word_append_char(word, VAR_UQUOTED);
	else if (word->status == '"')
		ft_t_word_append_char(word, VAR_DQUOTED);
	else if (word->status == '\'')
		ft_t_word_append_char(word, VAR_SQUOTED);
	return ;
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
		ft_t_word_append_char(word, input[*i]);
	else
	{
		ft_append_variable_prefix(word, input, i);
		if (ft_append_variable_var_name(word, input, i))
			ret = 1;
		else
			ft_t_word_append_char(word, VAR_END);
	}
	return (ret);
}
