/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand_heredoc1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 20:39:41 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/21 20:48:56 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "libft.h"
#include "lexer.h"
#include "exec.h"
#include <stdlib.h>

static int	ft_heredoc_var_expansion_init(t_node **node, t_data *data)
{
	t_word	word;
	int		i;

	ft_t_word_init(&word);
	i = 0;
	while (((char *)(*node)->content)[i] != '\0')
	{
		if (((char *)(*node)->content)[i] == '$')
		{
			if (ft_heredoc_handle_var_expansion(*node, &word, &i, data))
				return (1);
			i++;
			continue ;
		}
		ft_t_word_append_char(&word, ((char *)(*node)->content)[i++]);
	}
	free((*node)->content);
	(*node)->content = ft_t_word_get_str(&word);
	return (0);
}

static void	ft_heredoc_var_expansion_check(t_node **node, t_data *data)
{
	t_node	*temp;

	temp = *node;
	while (*node != NULL)
	{
		if (ft_strchr((*node)->content, '$'))
		{
			ft_heredoc_var_expansion_init(node, data);
		}
		*node = (*node)->next;
	}
	ft_get_beginning_of_list(temp, node);
}

void	ft_t_io_heredoc_var_expansion(t_io *io, t_data *data)
{
	if (io != NULL)
	{
		if (io->quoted_status == VAR_UQUOTED)
		{
			ft_heredoc_var_expansion_check(&(io->heredoc_node), data);
		}
	}
}
