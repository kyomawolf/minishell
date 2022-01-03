/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 17:25:23 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/03 20:03:07 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "minis.h"
#include "libft.h"
#include "lexer.h"
#include "exec.h"
#include <stdlib.h>
#include <stdio.h>

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

int	executor_rec(t_node **head, t_data *data, int es)
{
	int		last_depth;
	t_node	*temp;

	if (*head != NULL)
	{
		last_depth = ((t_bin *)((t_node *)(*head)->prev)->content)->depth;
		if (((t_bin *)(*head)->content)->control_op == OR && es == 0)
		{
			while (*head != NULL && \
				((t_bin *)(*head)->content)->depth > last_depth)
			{
				temp = *head;
				*head = (*head)->next;
				free (temp);
			}
		}
		executor(head, data, es);
	}
	(void)last_depth;
	return (0);
}

/* void	ft_print_env(t_data *data)
{
	t_node	*envp;

	envp = data->envp;
	while (envp != NULL)
	{
		printf("[exec_main: executor] %s\n", envp->content);
		envp = envp->next;
	}
	printf("next: execution\n");
} */

void	executor(t_node **head, t_data *data, int es)
{
	t_node	*pipeline;

	pipeline = create_execution_pipeline(head, data);
	if (pipeline == NULL)
	{
		free_list_wo_content(head);
		return ;
	}
	if ((es != 0 && ((t_bin *)pipeline->content)->control_op == OR)
		|| (es == 0 && ((t_bin *)pipeline->content)->control_op == AND))
		es = ft_execute(pipeline, data);
	ft_free_pipeline(&pipeline);
	change_env_exit_status(data, es);
	executor_rec(head, data, es);
}
