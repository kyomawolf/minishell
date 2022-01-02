/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_check2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/02 15:10:03 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/02 15:43:31 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "libft.h"
#include "minis.h"

int	check_wordpar(t_node *head)
{
	t_e_op	token;
	t_e_op	last_token;

	last_token = ((t_token *)head->content)->type;
	head = head->next;
	while (head != NULL)
	{
		token = ((t_token *)head->content)->type;
		if (token == OPAR && last_token > CPAR && last_token != HERE_DOC)
			return (1);
		else if (last_token == CPAR && token > CPAR && last_token != HERE_DOC)
			return (1);
		head = head->next;
		last_token = token;
	}
	return (0);
}

int	check_op(t_node *head)
{
	int		sw;
	t_e_op	token;

	sw = -1;
	while (head != NULL)
	{
		token = ((t_token *)head->content)->type;
		if (token < OPAR && (sw == 1 || sw == 2))
			return (1);
		if (token < OPAR && sw == -1)
			return (1);
		if (token < OPAR && sw == 0)
			sw = 1;
		else if (token == CPAR || token == WORD || token == HERE_DOC)
			sw = 0;
		else if (token < OPAR)
			sw = 2;
		head = head->next;
	}
	if (sw == -1 || sw == 1)
		return (1);
	return (0);
}

int	check_input_part2(t_node *head)
{
	if (check_op(head))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("syntax error near unexpected token operator\n", 2);
		return (1);
	}
	if (check_wordpar(head))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("syntax error near unexpected token parenthesis\n", 2);
		return (1);
	}
	return (0);
}
