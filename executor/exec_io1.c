/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_io1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 19:13:00 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/09 18:50:20 by mstrantz         ###   ########.fr       */
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

int	ft_set_output_redirection(t_io *io, t_node_io *node_io)
{
	int	fd;

	fd = -1;
	if (io->o_mode == 4 || io->o_mode == 3)
	{
		if (io->o_mode == 4)
			fd = open(node_io->file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		else if (io->o_mode == 3)
			fd = open(node_io->file, O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (redirection_error(node_io->file, fd))
			return (1);
		if (ft_check_last_redir(node_io->next, node_io->io_type))
			dup2(fd, STDOUT_FILENO);
		close (fd);
	}
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
	int		output_flag;
	int		input_flag;

	output_flag = 0;
	input_flag = 0;
	ret = 0;
	ft_close_unused_pipes(exec_data);
	if (((t_bin *)head->content)->io != NULL \
		&& ((t_bin *)head->content)->io->redir != NULL)
	{
		if (adjust_redirections(head, &input_flag, &output_flag))
			ret = 1;
	}
	adjust_pipes_helper(exec_data, input_flag, output_flag);
	return (ret);
}
