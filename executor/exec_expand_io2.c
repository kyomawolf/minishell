/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand_io2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/30 13:51:14 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/30 13:57:05 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "libft.h"
#include "lexer.h"
#include "exec.h"
#include "minis.h"
#include <stdlib.h>

static char	**ft_redir_get_variable_name(t_node_io *node, int i, t_data *data)
{
	char	**var;

	var = ft_calloc(3, sizeof(char *));
	if (var == NULL)
		return (NULL);
	var[0] = ft_heredoc_get_var_name(node->file, i);
	if (var[0] == NULL)
		return (NULL);
	ft_set_variable_name_and_value(&var, data);
	return (var);
}

int	ft_redir_handle_var_expansion(t_node_io *node, t_word *word, int *i, \
									t_data *data)
{
	char	**var;
	int		j;
	int		flag;

	flag = 1;
	var = ft_redir_get_variable_name(node, *i + 1, data);
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