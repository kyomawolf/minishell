/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 20:27:21 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/06 00:27:36 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "libft.h"
#include "exec.h"
#include <stdlib.h>
#include "minis.h"
#include "lexer.h"
#include <stdio.h>

void	ft_get_variable_name_guard(char **var)
{
	free(var[0]);
	var[0] = NULL;
	free(var);
	var = NULL;
}

void	ft_set_variable_name_and_value(char ***var, t_data *data)
{
	char	*var_value_copy;

	var_value_copy = NULL;
	if (var[0][0][0] == '$' && var[0][0][1] == '\0')
		var_value_copy = ft_strdup(var[0][0]);
	else
	{
		var[0][1] = mini_getenv(data, var[0][0]);
		if (var[0][1] == NULL)
			var[0][1] = "\0";
		var_value_copy = ft_strdup(var[0][1]);
	}
	if (var_value_copy == NULL)
	{
		ft_get_variable_name_guard(*var);
		return ;
	}
	var[0][1] = var_value_copy;
	var[0][2] = NULL;
}

// returns char array with variable name and variable value
static char	**ft_get_var_name_and_value(t_node **head, int *i, t_data *data)
{
	t_token	*token;
	char	**var;

	var = ft_calloc(3, sizeof(char *));
	if (var == NULL)
		return (NULL);
	token = (t_token *)(*head)->content;
	var[0] = ft_expander_get_var_name(token->string, *i + 2);
	if (var[0] == NULL)
	{
		while (token->string[*i] != VAR_END)
			(*i)++;
		(*i)++;
		free (var);
		return (NULL);
	}
	ft_set_variable_name_and_value(&var, data);
	return (var);
}

// handles how the variable should be expanded according to the quoted state
int	ft_handle_var_expansion(t_node **head, t_expand *exp_data, t_data *data)
{
	t_token	*token;
	char	**var;
	int		len;

	token = (t_token *)(*head)->content;
	exp_data->quote_status = token->string[exp_data->i + 1];
	if (exp_data->quote_status == VAR_SQUOTED)
		ft_handle_squoted_var(*head, exp_data->word, &(exp_data->i));
	else
	{
		var = ft_get_var_name_and_value(head, &(exp_data->i), data);
		if (var == NULL)
			return (1);
		if (exp_data->quote_status == VAR_UQUOTED)
			ft_handle_unquoted_var(exp_data, var);
		else if (exp_data->quote_status == VAR_DQUOTED)
			ft_handle_dquoted_var(exp_data->word, var);
		if (!ft_strncmp(var[0], "$", 2))
			(exp_data->i) = (exp_data->i) - 1;
		len = ft_strlen(var[0]);
		(exp_data->i) = (exp_data->i) + len + 2;
		ft_free_char_array(var);
	}
	return (0);
}

int	ft_init_handle_var_expansion(t_node **head, t_data *data, \
									t_expand *exp_data, char *temp)
{
	if (temp[exp_data->i] == '$')
	{
		if (ft_handle_var_expansion(head, exp_data, data))
			return (1);
		exp_data->i++;
		if (temp[exp_data->i] != '\0' \
			&& exp_data->word->chars[exp_data->word->write_head - 1] == '\0')
			exp_data->word->write_head--;
		if (temp[exp_data->i] == '\0')
		{
			ft_t_word_append_char(exp_data->word, temp[exp_data->i]);
			return (2);
		}
		return (3);
	}
	return (0);
}
