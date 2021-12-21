/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_io1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 19:13:00 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/21 22:30:45 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "exec.h"
#include <unistd.h>
#include <fcntl.h>

static void	ft_set_input_redirection(t_exec *exec_data, t_node *head)
{
	int	fd;

	//IRD if IO true
	if (((t_bin *)head->content)->io != NULL \
		&& ((t_bin *)head->content)->io->infile != NULL)
	{
		fd = open(((t_bin *)head->content)->io->infile, O_RDONLY);
		/* if (fd == -1)
			ft_free_input(exec_data, EXIT); */
		dup2(fd, STDIN_FILENO);
		close(fd);
	} // IRD  NOT first cmd
	else if (exec_data->cmd_count != 0)
	{
		dup2(exec_data->pipes[exec_data->cmd_count][0], STDIN_FILENO);
		//added close
		close(exec_data->pipes[exec_data->cmd_count][0]);
	}
	// else read from stdin
}

void	ft_set_outout_redirection_helper(t_node *head)
{
	int	i;
	int	fd;

	i = 0;
	while (((t_bin *)head->content)->io->output[i] != NULL)
	{
		//open/create files in accordance to the given mode (trunc/append)
		if (((t_bin *)head->content)->io->o_mode == 4) //mode is TRUNC)
		{
			fd = open(((t_bin *)head->content)->io->output[i], \
			O_WRONLY | O_CREAT | O_TRUNC, 0777);
			//guard if fd == -1
		}
		else if (((t_bin *)head->content)->io->o_mode == 3) //if mode is APPEND
		{
			fd = open(((t_bin *)head->content)->io->output[i], \
			O_WRONLY | O_CREAT | O_APPEND, 0777);
			//guard
		}
		/* if (fd == -1)
			free and exit */
		//close "unused files"
		if (((t_bin *)head->content)->io->output[i + 1] == NULL)
		{
		//redirect stdout to outfile (last file in output[])
			dup2(fd, STDOUT_FILENO);
		}
		close (fd);
		i++;
	}
}

static void	ft_set_output_redirection(t_exec *exec_data, t_node *head)
{
	//ORD if IO is true
	if (((t_bin *)head->content)->io != NULL \
		&& ((t_bin *)head->content)->io->output != NULL)
		ft_set_outout_redirection_helper(head);
	// ORD pipe: more than 1 cmd and not last cmd
	else if (exec_data->cmd_count != exec_data->num_cmds - 1)
	{
		dup2(exec_data->pipes[exec_data->cmd_count + 1][1], STDOUT_FILENO);
		//added close
		close(exec_data->pipes[exec_data->cmd_count + 1][1]);
	} //first cmd, but not last
	else if (exec_data->cmd_count == 0 && exec_data->num_cmds != 1)
	{
		dup2(exec_data->pipes[1][1], STDOUT_FILENO);
		close(exec_data->pipes[1][1]);
	}
	//else output in stdout: last cmd
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

void	ft_adjust_pipes(t_exec *exec_data, t_node *head)
{
	ft_close_unused_pipes(exec_data);
	ft_t_exec_heredoc_check(head, exec_data);
	if (exec_data->here_doc == 1)
		ft_exec_here_doc(exec_data, head);
	else
	{
		ft_set_input_redirection(exec_data, head);
		ft_set_output_redirection(exec_data, head);
	}
}
