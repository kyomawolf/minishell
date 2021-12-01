/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 15:17:51 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/01 15:27:18 by jkasper          ###   ########.fr       */
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
		if (token > HERE_DOC && token < QUOTE && !sw)
			sw += 1;
		else if (token == WORD)
			sw -= 1;
		else
			return (1);
		head = head->next;
	}
	if (sw != 0)
		return (1);
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

	err = check_pars(head);
	if (err != -1)
		return (err);
	err = check_io(head);
	if (err != -1)
		return (err);
	err = check_op(head);
	write(1, "\nOK\n", 4);
	return (err);
}
