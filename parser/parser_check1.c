/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_check1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 15:17:51 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/02 15:17:38 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "minis.h"
#include "libft.h"
#include "parser.h"

int	check_pars(t_node *head)
{
	int	pars;
	int	token;
	int	last_token;

	pars = 0;
	last_token = -1;
	while (head != NULL)
	{
		token = ((t_token *)head->content)->type;
		if (token == CPAR && last_token == OPAR)
			return (1);
		if (token == CPAR)
			pars--;
		if (token == OPAR)
			pars++;
		if (pars < 0)
			return (1);
		last_token = token;
		head = head->next;
	}
	if (pars != 0)
		return (1);
	return (0);
}

int	check_io(t_node *head)
{
	int		sw;
	t_e_op	token;

	sw = 0;
	while (head != NULL)
	{
		token = ((t_token *)head->content)->type;
		if (token == WORD && sw)
			sw = 0;
		else if (token > HERE_DOC && token < QUOTE && !sw)
		{
			sw = 1;
		}
		else if (sw != 0)
			return (1);
		head = head->next;
	}
	return (0);
}

int	check_input(t_node *head)
{
	if (check_pars(head))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("syntax error near unexpected token parenthesis\n", 2);
		return (1);
	}
	if (check_io(head))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("syntax error near unexpected token input/output ", 2);
		ft_putstr_fd("redirection\n", 2);
		return (1);
	}
	if (check_input_part2(head))
		return (1);
	return (0);
}
