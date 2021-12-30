/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_io1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 19:13:00 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/30 21:50:57 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "exec.h"
#include "libft.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int	ft_check_last_redir(t_node_io *node_io, int type)
{
	int	ret;

	ret = 1;
	while (node_io)
	{
		if (type == IRD && node_io->io_type == IRD)
		{
			ret = 0;
			break ;
		}
		else if ((type == ORD_APP || type == ORD_TRC) \
			&& (node_io->io_type == ORD_APP || node_io->io_type == ORD_TRC))
		{
			ret = 0;
			break ;
		}
		node_io = node_io->next;
	}
	return (ret);
}

int	ft_set_input_redirection(t_io *io, t_node_io *node_io)
{
	int	fd;

	fd = open(node_io->file, O_RDONLY);
	if (redirection_error(node_io->file, fd))
		return (1);
	if (io->infile != NULL && ft_check_last_redir(node_io->next, IRD))
		dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	ft_set_output_redirection(t_io *io, t_node_io *node_io)
{
	int	fd;

	fd = -2;
	if (io->o_mode == 4)
		fd = open(io->redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else if (io->o_mode == 3)
		fd = open(io->redir->file, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (redirection_error(io->redir->file, fd))
		return (1);
	if (ft_check_last_redir(node_io->next, node_io->io_type))
		dup2(fd, STDOUT_FILENO);
	close (fd);
	return (0);
}

static void	ft_close_unused_pipes(t_exec *exec_data)
{
	int	i;

	i = 1;
	while (exec_data->pipes != NULL && exec_data->pipes[i])
	{
		if (i != exec_data->cmd_count)
			close(exec_data->pipes[i][0]);
		if (i != exec_data->cmd_count + 1)
			close(exec_data->pipes[i][1]);
		i++;
	}
}

int	ft_adjust_pipes(t_exec *exec_data, t_node *head)
{
	int		ret;
	t_io	*io;
	int		output_flag;
	int		input_flag;

	output_flag = 0;
	input_flag = 0;
	ret = 0;
	ft_close_unused_pipes(exec_data);
	if (((t_bin *)head->content)->io != NULL && ((t_bin *)head->content)->io->redir != NULL)
	{
		io = ((t_bin *)head->content)->io;
		while (io->redir)
		{
			if (io->redir->io_type == HERE_DOC && io->redir->active_hd)
			{
				ft_exec_here_doc_helper(head);
				input_flag = 1;
			}
			else if (io->redir->io_type == IRD)
			{
				if (ft_set_input_redirection(io, io->redir))
				{
					ret = 1;
					break ;
				}
				input_flag = 1;
			}
			else
			{
				if (ft_set_output_redirection(io, io->redir))
				{
					ret = 1;
					break ;
				}
				output_flag = 1;
			}
			io->redir = io->redir->next;
		}
	}
	if (input_flag == 0 && exec_data->cmd_count != 0)
	{
		dup2(exec_data->pipes[exec_data->cmd_count][0], STDIN_FILENO);
		close(exec_data->pipes[exec_data->cmd_count][0]);
	}
	if (output_flag == 0 && exec_data->cmd_count != exec_data->num_cmds - 1)
	{
		dup2(exec_data->pipes[exec_data->cmd_count + 1][1], STDOUT_FILENO);
		close(exec_data->pipes[exec_data->cmd_count + 1][1]);
	}
	else if (output_flag == 0 && exec_data->cmd_count == 0 && exec_data->num_cmds != 1)
	{
		dup2(exec_data->pipes[1][1], STDOUT_FILENO);
		close(exec_data->pipes[1][1]);
	}
	return (ret);
}
