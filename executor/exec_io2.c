/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_io2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 18:28:48 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/21 21:53:30 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include "structs.h"
#include "libft.h"
#include "exec.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static void	ft_exec_here_doc_helper(t_node *head)
{
	int		pipe_fd[2];
	t_node	*here_doc;
	t_node	*temp;

	here_doc = ((t_bin *)head->content)->io->heredoc_node;
	pipe(pipe_fd);
	while (here_doc != NULL)
	{
		write(pipe_fd[1], here_doc->content, ft_strlen(here_doc->content));
		write(pipe_fd[1], "\n", 1);
		temp = here_doc;
		here_doc = here_doc->next;
		free(temp->content);
		temp->content = NULL;
		free(temp);
		temp = NULL;
	}
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
}

void	ft_exec_here_doc(t_exec *exec_data, t_node *head)
{
	ft_exec_here_doc_helper(head);
	//if ORD given: create files and set ORD to last file
	if (((t_bin *)head->content)->io != NULL \
		&& ((t_bin *)head->content)->io->output != NULL)
		ft_set_outout_redirection_helper(head);
	else if (exec_data->cmd_count != exec_data->num_cmds - 1)
	{
		// if no ORD && not last cmd: set stdout to writeend of next pipe
		dup2(exec_data->pipes[exec_data->cmd_count + 1][1], STDOUT_FILENO);
		close(exec_data->pipes[exec_data->cmd_count + 1][1]);
	}
	else if (exec_data->cmd_count == 0 && exec_data->num_cmds != 1)
	{
		dup2(exec_data->pipes[1][1], STDOUT_FILENO);
		close(exec_data->pipes[1][1]);
	}
	//else redirect output to stdout
}

void	ft_t_exec_heredoc_check(t_node *head, t_exec *exec_data)
{
	t_bin	*cmd;

	cmd = (t_bin *)head->content;
	if (cmd->io != NULL && cmd->io->infile == NULL \
		&& cmd->io->heredoc_node != NULL)
		exec_data->here_doc = 1;
	else
		exec_data->here_doc = 0;
}
