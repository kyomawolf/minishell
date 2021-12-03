/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 15:17:51 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/03 22:50:14 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
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
			head = head->next;
			continue ;
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
		if (token == OPAR && last_token > CPAR)
			return (1);
		else if (last_token == CPAR && token > CPAR)
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
		else if (token == CPAR || token == WORD)
			sw = 0;
		else
			sw = 2;
		head = head->next;
	}
	if (sw == -1 || sw == 1)
		return (1);
	return (0);
}

int	check_input(t_node *head)
{
	int	err;

	err = 0;
	err += check_pars(head);
	write(1, "passed pars\n", 12);
	err += check_io(head);
	write(1, "passed io\n", 10);
	err += check_op(head);
	write(1, "passed ops\n", 11);
	err += check_wordpar(head);
	write(1, "OK\n", 3);
	return (err);
}
