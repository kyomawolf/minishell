/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_io_adjust_pipes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/02 16:14:48 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/02 18:06:59 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "exec.h"
#include "libft.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

void	adjust_pipes_helper(t_exec *exec_data, int i_flag, int o_flag)
{
	if (i_flag == 0 && exec_data->cmd_count != 0)
	{
		dup2(exec_data->pipes[exec_data->cmd_count][0], STDIN_FILENO);
		close(exec_data->pipes[exec_data->cmd_count][0]);
	}
	if (o_flag == 0 && exec_data->cmd_count != exec_data->num_cmds - 1)
	{
		dup2(exec_data->pipes[exec_data->cmd_count + 1][1], STDOUT_FILENO);
		close(exec_data->pipes[exec_data->cmd_count + 1][1]);
	}
	else if (o_flag == 0 && exec_data->cmd_count == 0 \
			&& exec_data->num_cmds != 1)
	{
		dup2(exec_data->pipes[1][1], STDOUT_FILENO);
		close(exec_data->pipes[1][1]);
	}
}

int	adjust_redirections(t_node *head, int *inp_flag, int *outp_flag)
{
	t_io	*io;

	io = ((t_bin *)head->content)->io;
	while (io->redir)
	{
		if (io->redir->io_type == HERE_DOC && io->redir->active_hd)
		{
			ft_exec_here_doc_helper(head);
			*inp_flag = 1;
		}
		else if (io->redir->io_type == IRD)
		{
			if (ft_set_input_redirection(io, io->redir))
				return (1);
			*inp_flag = 1;
		}
		else if (io->redir->io_type == ORD_APP || io->redir->io_type == ORD_TRC)
		{
			if (ft_set_output_redirection(io, io->redir))
				return (1);
			*outp_flag = 1;
		}
		io->redir = io->redir->next;
	}
	return (0);
}
