/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 15:17:51 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/23 22:45:45 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "minis.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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

int	check_input(t_node *head)
{
	if (check_pars(head))
		return (printf("syntax error near unexpected token parenthesis\n"));
	if (check_io(head))
		return (printf("syntax error near unexpected token input/output \
		redirection\n"));
	if (check_op(head))
		return (printf("syntax error near unexpected token operator\n"));
	if (check_wordpar(head))
		return (printf("syntax error near unexpected token parenthesis\n"));
	return (0);
}
