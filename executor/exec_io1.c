/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_io1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 19:13:00 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/23 22:53:23 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "exec.h"
#include "libft.h"
#include <unistd.h>
#include <fcntl.h>

static int	ft_set_input_redirection(t_exec *exec_data, t_node *head)
{
	int	fd;

	if (((t_bin *)head->content)->io != NULL \
		&& ((t_bin *)head->content)->io->infile != NULL)
	{
		fd = open(((t_bin *)head->content)->io->infile, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell:", 2);
			ft_putstr_fd(((t_bin *)head->content)->io->infile, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			//ft_free_input(exec_data, EXIT);
			return (1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (exec_data->cmd_count != 0)
	{
		dup2(exec_data->pipes[exec_data->cmd_count][0], STDIN_FILENO);
		close(exec_data->pipes[exec_data->cmd_count][0]);
	}
	return (0);
}

int	ft_set_outout_redirection_helper(t_node *head)
{
	int	i;
	int	fd;

	fd = -2;
	i = 0;
	while (((t_bin *)head->content)->io->output[i] != NULL)
	{
		if (((t_bin *)head->content)->io->o_mode == 4)
		{
			fd = open(((t_bin *)head->content)->io->output[i], \
			O_WRONLY | O_CREAT | O_TRUNC, 0777);
		}
		else if (((t_bin *)head->content)->io->o_mode == 3)
		{
			fd = open(((t_bin *)head->content)->io->output[i], \
			O_WRONLY | O_CREAT | O_APPEND, 0777);
		}
		if (fd == -1)
		{
				ft_putstr_fd("minishell:", 2);
				ft_putstr_fd(((t_bin *)head->content)->io->output[i], 2);
				ft_putstr_fd(": Permission denied\n", 2);
				return (1);
		}
		//close "unused files"
		if (((t_bin *)head->content)->io->output[i + 1] == NULL)
			dup2(fd, STDOUT_FILENO);
		close (fd);
		i++;
	}
	return (0);
}

static int	ft_set_output_redirection(t_exec *exec_data, t_node *head)
{
	if (((t_bin *)head->content)->io != NULL \
		&& ((t_bin *)head->content)->io->output != NULL)
	{
		if (ft_set_outout_redirection_helper(head))
			return (1);
	}
	else if (exec_data->cmd_count != exec_data->num_cmds - 1)
	{
		dup2(exec_data->pipes[exec_data->cmd_count + 1][1], STDOUT_FILENO);
		close(exec_data->pipes[exec_data->cmd_count + 1][1]);
	}
	else if (exec_data->cmd_count == 0 && exec_data->num_cmds != 1)
	{
		dup2(exec_data->pipes[1][1], STDOUT_FILENO);
		close(exec_data->pipes[1][1]);
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
	int	ret;

	ret = 0;
	ft_close_unused_pipes(exec_data);
	ft_t_exec_heredoc_check(head, exec_data);
	if (exec_data->here_doc == 1)
	{
		if (ft_exec_here_doc(exec_data, head))
			ret = 1;
	}
	else
	{
		if (ft_set_input_redirection(exec_data, head))
			ret = 1;
		if (ret != 1 && ft_set_output_redirection(exec_data, head))
			ret = 1;
	}
	return (ret);
}