/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 18:47:45 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/06 21:46:54 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "libft.h"
#include "struct.h"
#include "lexer.h"
#include "exec.h"
#include <signal.h>

void	child_free_at_exit(t_node *pl, t_data *data, t_exec *exec_data, \
								t_node **ori_head)
{
	ft_exec_data_free_pipes(exec_data);
	free(exec_data->pid);
	ft_get_beginning_of_list(pl, &pl);
	free_list_wo_content(&pl);
	free_list_wo_content(ori_head);
	free_tree(data->tree);
	free_main(data);
}

char	ft_tolower2(unsigned int i, char c)
{
	(void)i;
	return (ft_tolower(c));
}

void	wait_child(int sig)
{
	(void) sig;
	return ;
}

void	ft_t_expand_init(t_expand *exp_data)
{
	exp_data->i = 0;
	exp_data->list = NULL;
	exp_data->word = ft_calloc(1, sizeof(t_word));
	ft_t_word_init(exp_data->word);
}

void	ft_signals(void)
{
	signal(SIGQUIT, wait_child);
	signal(0, wait_child);
	signal(1, wait_child);
	signal(2, wait_child);
	signal(3, wait_child);
	signal(4, wait_child);
	signal(5, wait_child);
	signal(6, wait_child);
	signal(7, wait_child);
	signal(11, wait_child);
	signal(13, wait_child);
}
