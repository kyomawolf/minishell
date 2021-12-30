/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand_io1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/30 13:38:00 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/30 20:05:09 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "minis.h"
#include "libft.h"
#include "lexer.h"
#include "exec.h"
#include <stdlib.h>

static int	ft_redir_var_expansion_init(t_node_io **node, t_data *data)
{
	t_word	word;
	int		i;
	int		flag;

	ft_t_word_init(&word);
	flag = 1;
	i = 0;
	while ((*node)->file[i] != '\0')
	{
		if ((*node)->file[i] == '$')
		{
			flag = 0;
			if (ft_redir_handle_var_expansion(*node, &word, &i, data))
				return (1);
			i++;
			continue ;
		}
		ft_t_word_append_char(&word, (*node)->file[i++]);
	}
	free((*node)->file);
	if (flag)
		ft_t_word_append_char(&word, '\0');
	(*node)->file = ft_t_word_get_str(&word);
	return (0);
}

void	ft_t_node_io_reset_head(t_node_io *temp, t_node_io **head)
{
	*head = temp;
	if (temp != NULL)
	{
		while ((*head)->prev != NULL)
			*head = (*head)->prev;
	}
}

void	ft_redir_var_expansion_check(t_node_io ** node, t_data *data)
{
	t_node_io	*temp;

	temp = *node;
	while (*node != NULL)
	{
		if (ft_strchr((*node)->file, '$'))
			ft_redir_var_expansion_init(node, data);
		*node = (*node)->next;
	}
	ft_t_node_io_reset_head(temp, node);
}

void	ft_t_io_redir_expansion(t_io *io, t_data *data)
{
	if (io != NULL)
	{
		ft_redir_var_expansion_check(&(io->redir), data);
	}
}