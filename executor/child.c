/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 01:39:08 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/14 17:07:06 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minis.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "struct.h"
#include "libft.h"

/*
	explaining: flag
	flag is set bitwise:
	pipe one			pipe two
	1 2 | 4 8  			16 32 | 64 128	  
	0 0 | 0 0 			0  0  | 0  0
	|					   |
	read from			write to
	which fd			which fd
	in setflag(int pipeone, int pipetwo)
	0 1 | 2 3			4  5  | 6  7
	special cases:
	0 -> beginning of the pipe
	15 -> end of the pipe
*/

int	setflag(int fp, int sp)
{
	int	ret;

	ret = 0;
	if (fp == 0)
		ret += 1;
	if (fp == 1)
		ret += 2;
	if (fp == 2)
		ret += 4;
	if (fp == 3)
		ret += 8;
	if (sp == 4)
		ret += 16;
	if (sp == 5)
		ret += 32;
	if (sp == 6)
		ret += 64;
	if (sp == 7)
		ret += 128;
	return (ret);
}

int	**child_setpipes(int **a_pipes, int flag)
{
	return (a_pipes);
}

int	child_process(t_data *data, int **a_pipes, t_bin *tree, int flag)
{
	int	fd;

	a_pipes = child_setpipes(a_pipes, flag);//TODO sets first two pipes to close and dup the pipes acc to the flag
	tree->command->exit_status = execve(tree->command->arguments[0], &(tree->command->arguments[1]), NULL);
	close(apipes[0][0]);
	close(apipes[0][1]);
	exit(tree->command->exit_status);
}

int	child_init(t_data *data, int **a_pipes, t_node *tree_list, pid_t children[2])
{
	a_pipes[0][0] = ft_calloc(1, sizeof(int));
	a_pipes[0][1] = ft_calloc(1, sizeof(int));
	a_pipes[1][0] = ft_calloc(1, sizeof(int));
	a_pipes[1][1] = ft_calloc(1, sizeof(int));
	pipe(a_pipes[0]);
	pipe(a_pipes[1]);
	children[0] = fork();
	flag = setflag_first();
	if (!child_1)
		child_process(data, a_pipes, tree_list->content, setflag(0, 0));
	children[1] = fork();
	tree_list = tree_list->next;
	if (!child_2)
		child_process(data, a_pipes, tree_list->content, setflag(2, 5));
}

int	child_piped(t_data *data, t_node *tree_list)
{
	pid_t	children[2];
	int		**a_pipes;
	int		*buffer;
	int		io;
	int		ch;

	a_pipes = ft_calloc(3, sizeof(int *));
	child_init(data, a_pipes, tree_list, children);
	io = 4;
	ch = 0;
	tree_list = tree_list->next;
	while (tree_list->content != NULL)
	{
		waitpid(children[ch % 2]);
		create_pipes(a_pipes, io);//TODO
		children[ch % 2] = fork();
		if (!children[ch % 2])
		{
			child_process(data, a_pipes, tree_list->content, setflag(io, io + 1));//sort of like this NOT thought through
		}
		tree_list = tree_list->next;
		//pipe,pipe2->c1,c2->pipe3->c3,c4->pipe4->c5
		io += 2;
		ch++;
		if (io > 6)//collect case > 7 in setpipes
			io = 0;
	}
}

int	child_exec(t_data *data, t_node *tree_list)
{
	int		i;
	t_node	*ori;

	i = 0;
	ori = tree_list;
	while (tree_list != NULL && tree_list->content != NULL)
	{
		tree_list = tree_list->next;
		i++;
	}
	if (i > 1)
		child_piped(data, ori);
	else
		...;
}
