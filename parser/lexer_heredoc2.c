/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_heredoc2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 01:11:01 by mstrantz          #+#    #+#             */
/*   Updated: 2022/04/04 15:04:38 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "lexer.h"
#include "libft.h"
#include "minis.h"
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

//frees readline output if necessary and sets token->heredoc with input from
// heredoc
static void	ft_lexer_handle_heredoc_input_helper(t_token *token,
			t_node *head, char **line)
{
	if (*line != NULL)
	{
		free(*line);
		*line = NULL;
	}
	if (head != NULL)
		token->heredoc = head;
	return ;
}

static int	ft_lexer_heredoc_input_append(char **line, t_node **head)
{
	t_node	*node;
	int		ret;

	ret = 0;
	node = ft_t_node_create(*line);
	if (node == NULL)
	{
		ft_putstr_fd("minishell: allocation error\n", 2);
		if (*line != NULL)
			free(*line);
		ret = 2;
	}
	ft_t_node_add_back(head, node);
	return (ret);
}

static int	ft_get_heredoc_input(t_node **head, t_token *delimiter, char **line)
{
	size_t	len;

	len = ft_strlen(delimiter->string) + 1;
	while (1)
	{
		#ifndef READLINE
			printf(">");
			fflush(0);
			get_next_line_wrapper(line, 0);
		#endif /*NOT READLINE*/
		#ifdef READLINE
			*line = readline(">");
		#endif /*READLINE*/
		if (*line == NULL || !ft_strncmp(delimiter->string, *line, len))
			break ;
		else
		{
			if (ft_lexer_heredoc_input_append(line, head))
				return (2);
		}
	}
	return (0);
}

static int	ft_lexer_heredoc_post_input(char *line, int temp_fd)
{
	if (line == NULL && errno == 9)
	{
		if (dup2(temp_fd, STDIN_FILENO) == -1)
		{
			ft_putstr_fd(strerror(errno), 2);
			return (2);
		}
	}
	if (close (temp_fd) == -1)
	{
		ft_putstr_fd(strerror(errno), 2);
		return (2);
	}
	if (errno == 9)
	{
		errno = 0;
		return (2);
	}
	return (0);
}

// reads stdin, saves input in list until line is equal to delimiter string
// saves list of input in token member heredoc
int	ft_lexer_handle_heredoc_input(t_token *token, t_token *delimiter)
{
	char	*line;
	t_node	*head;
	int		temp_fd;
	int		ret;

	temp_fd = dup(STDIN_FILENO);
	if (temp_fd == -1)
	{
		ft_putstr_fd(strerror(errno), 2);
		return (2);
	}
	signal(SIGINT, ft_signals_heredoc);
	head = NULL;
	if (ft_get_heredoc_input(&head, delimiter, &line))
		return (2);
	ret = ft_lexer_heredoc_post_input(line, temp_fd);
	if (ret == 2)
		return (2);
	ft_lexer_handle_heredoc_input_helper(token, head, &line);
	return (0);
}
