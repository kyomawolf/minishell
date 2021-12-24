/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 20:33:25 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/24 02:17:00 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "libft.h"
#include "lexer.h"
#include "exec.h"

char	*ft_get_var_name_special_cases(char *str, int i)
{
	char	*var_name;

	var_name = NULL;
	if (str[i] == '?')
	{
		var_name = ft_calloc(2, sizeof(char));
		var_name[0] = '?';
		var_name[1] = '\0';
	}
	else if (str[i] == VAR_END)
	{
		var_name = ft_calloc(2, sizeof(char));
		var_name[0] = '$';
		var_name[1] = '\0';
	}
	return (var_name);
}

// gets variable name, checks for valid variable name as well
char	*ft_expander_get_var_name(char *str, int i)
{
	int		i_start;
	char	*var_name;

	i_start = i;
	if (str[i] == '?' || str[i] == VAR_END)
		return (ft_get_var_name_special_cases(str, i));
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (NULL);
	while (str[i] != VAR_END)
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (NULL);
		i++;
	}
	var_name = ft_substr(str, i_start, i - i_start);
	return (var_name);
}

// appends chars to word in case of a single quoted variable
void	ft_handle_squoted_var(t_node *head, t_word *word, int *i)
{
	t_token	*token;

	token = (t_token *)head->content;
	ft_t_word_append_char(word, token->string[*i]);
	*i = *i + 2;
	while (token->string[*i] != VAR_END)
	{
		ft_t_word_append_char(word, token->string[*i]);
		(*i)++;
	}
}

// appends chars to word in case of an unquoted variable
//  creates new word and token if necessary
void	ft_handle_unquoted_var(t_expand *exp_data, char **var)
{
	int	j;

	j = 0;
	while (var[1][j] != '\0')
	{
		if (ft_whitespaces(var[1][j]))
		{
			ft_skip_set(var[1], &j, " \t\n");
			if (exp_data->word->write_head != 0)
			{
				ft_terminate_word_and_add_to_list(exp_data);
			}
			continue ;
		}
		ft_t_word_append_char(exp_data->word, var[1][j]);
		if (var[1][j] == '*')
		{
			ft_t_word_append_char(exp_data->word, -2);
		}
		j++;
	}
}

// appends chars to word in case of a double quoted variable
void	ft_handle_dquoted_var(t_word *word, char **var)
{
	int	j;

	j = 0;
	while (var[1][j] != '\0')
	{
		if (var[1][j] == '*')
		{
			ft_t_word_append_char(word, var[1][j]);
			ft_t_word_append_char(word, -1);
		}
		else
			ft_t_word_append_char(word, var[1][j]);
		j++;
	}
	ft_t_word_append_char(word, var[1][j]);
}
