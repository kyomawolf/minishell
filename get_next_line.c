/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/14 22:37:06 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/19 15:29:53 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	char		*ret;
	static char	*str = NULL;
	size_t		i;
	size_t		ii;

	ret = malloc(GNL_BUFFER + 1);
	ret[0] = '\0';
	i = 0;
	ii = 0;
	while (1)
	{
		if (str == NULL || str[i] == '\0')
		{
			if (get_filled(&str, fd) || !get_memory(&ret))
				return (e_exit(&str, &ret));
			if (str[0] == '\0')
				return (s_exit(&str, &ret, i + 1, ii));
			i = 0;
		}
		ret[ii] = str[i];
		if (ret[ii] == '\n' || ret[ii] == '\0')
			return (s_exit(&str, &ret, i + 1, ii));
		i++;
		ii++;
	}
}
