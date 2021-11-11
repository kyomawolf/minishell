/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brackets.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 16:35:29 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/11 17:01:56 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minis.h"
#include "parser.h"
#include "struct.h"

int	brackets_amount(t_data *data, int count, int delimiter)
{
	int	ret;

	ret = 0;
	while (data->input[count] != '\0' && data->input[count] != delimiter)
	{
		if (ft_skip_quotes(data->input, &count))
			return (ret);
		if (data->input[count] == '(')
			ret++;
		count++;
	}
	return (ret);
}

int	brackets_correct(t_data *data, int count, int delimiter)
{
	int	ret;

	ret = 0;
	while (data->input[count] != '\0' && data->input[count] != delimiter)
	{
		if (ft_skip_quotes(data->input, &count))
			return (ret);
		if (data->input[count] == ')')
			ret++;
		if (data->input[count] == '(')
			ret--;
		count++;
	}
	return (ret);
}

int	brackets_next(t_data *data, int count, int delimiter, int mode)
{
	while (data->input[count] != '\0' && data->input[count] != delimiter)
	{
		if (ft_skip_quotes(data->input, &count))
			return (-1);
		if (data->input[count] == ')' && mode)
			return (count);
		if (data->input[count] == '(' && !mode)
			return (count);
		count++;
	}
	return (-1);
}
