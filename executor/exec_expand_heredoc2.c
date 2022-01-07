/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand_heredoc2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 20:44:48 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/07 19:35:33 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "libft.h"
#include "exec.h"
#include "lexer.h"

static char	*ft_heredoc_get_var_name_special_cases(char *str, int i)
{
	char	*var_name;

	var_name = NULL;
	if (str[i] == '?')
	{
		var_name = ft_calloc(2, sizeof(char));
		if (var_name == NULL)
			return (NULL);
		var_name[0] = '?';
		var_name[1] = '\0';
	}
	if (!ft_isalnum(str[i]) && str[i] != '_')
	{
		var_name = ft_calloc(2, sizeof(char));
		if (var_name == NULL)
			return (NULL);
		var_name[0] = '$';
		var_name[1] = '\0';
	}
	return (var_name);
}

char	*ft_heredoc_get_var_name(char *str, int i)
{
	int		i_start;
	char	*var_name;

	i_start = i;
	if (str[i] == '?' || (!ft_isalnum(str[i]) && str[i] != '_'))
		return (ft_heredoc_get_var_name_special_cases(str, i));
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (NULL);
	while (ft_isalnum(str[i]) || str[i] == '_')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (NULL);
		i++;
	}
	var_name = ft_substr(str, i_start, i - i_start);
	return (var_name);
}

static char	**ft_heredoc_get_variable_name(t_node *node, int i, t_data *data)
{
	char	**var;

	var = ft_calloc(3, sizeof(char *));
	if (var == NULL)
		return (NULL);
	var[0] = ft_heredoc_get_var_name(node->content, i);
	if (var[0] == NULL)
		return (NULL);
	ft_set_variable_name_and_value(&var, data);
	return (var);
}

int	ft_heredoc_handle_var_expansion(t_node *node, t_word *word, \
		int *i, t_data *data)
{
	char	**var;
	int		j;
	int		flag;

	flag = 1;
	var = ft_heredoc_get_variable_name(node, *i + 1, data);
	if (var == NULL)
		return (1);
	j = 0;
	while (var[1][j] != '\0')
	{
		flag = 0;
		ft_t_word_append_char(word, var[1][j]);
		j++;
	}
	if (flag)
		ft_t_word_append_char(word, var[1][j]);
	j = 0;
	while (var[0][j++] != '\0')
		(*i)++;
	ft_free_char_array(var);
	return (0);
}
