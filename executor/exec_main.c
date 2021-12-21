/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 17:25:23 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/21 21:04:44 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "minis.h"
#include "libft.h"
#include "lexer.h"
#include "exec.h"
#include <stdlib.h>

static void	change_env_exit_status(t_data *data, int es)
{
	char	*status;
	t_node	*temp;

	temp = data->envp;
	while (temp != NULL)
	{
		if (ft_strnstr(temp->content, "?=\0", ft_strlen(temp->content)))
		{
			free (temp->content);
			status = ft_itoa(es);
			temp->content = ft_strjoin("?=", status);
			free (status);
			status = NULL;
			break ;
		}
		temp = temp->next;
	}
}

int	executor_rec(t_node *head, t_data *data, int es, t_node *start)
{
	int	last_depth;

	if (head != NULL)
	{
		if (start == head)
			head = head->next;
		last_depth = ((t_bin *)((t_node *)head->prev)->content)->depth;
		if (head == NULL)
			return (1);
		if (((t_bin *)head->content)->control_op == OR && es == 0)
		{
			while (head != NULL && ((t_bin *)head->content)->depth > last_depth)
				head = head->next;
		}
		executor(head, data, es);
	}
	(void)last_depth;
	return (0);
}

void	executor(t_node *head, t_data *data, int es)
{
	t_node	*start;
	t_node	*pipeline;

	start = head;
	pipeline = create_execution_pipeline(&head, data);
	if (pipeline == NULL)
		return ;
	if ((es != 0 && ((t_bin *)pipeline->content)->control_op == OR)
		|| (es == 0 && ((t_bin *)pipeline->content)->control_op == AND))
		es = ft_execute(pipeline, data);
	ft_free_pipeline(pipeline);
	change_env_exit_status(data, es);
	if (executor_rec(head, data, es, start))
		return ;
}
