/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/14 22:37:18 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/19 17:24:11 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h" 

int	get_filled(char **str, int fd)
{
	ssize_t	i;

	i = 0;
	if (*str == NULL)
	{
		*str = malloc(GNL_BUFFER + 2);
		(*str)[GNL_BUFFER + 1] = '1';
	}
	if (*str == NULL)
		return (1);
	i = read(fd, (*str), GNL_BUFFER);
	if (i >= 0)
		(*str)[i] = '\0';
	if (((*str)[GNL_BUFFER + 1] == '0' && (*str)[0] != '\0') || i == -1)
		return (1);
	(*str)[GNL_BUFFER + 1] = '1';
	if (i < GNL_BUFFER)
		(*str)[GNL_BUFFER + 1] = '0';
	return (0);
}

void	get_moved(char **str, size_t i)
{
	size_t	ii;

	ii = i;
	(*str)[i - ii] = (*str)[i];
	while ((*str)[i] != '\0')
	{
		(*str)[i - ii] = (*str)[i];
		i++;
	}
	(*str)[i - ii] = '\0';
}

int	get_memory(char **old)
{
	char	*new;
	size_t	i;

	if (*old == NULL)
		return (0);
	i = 0;
	while ((*old)[i] != '\0')
		i++;
	new = malloc(i + 1 + GNL_BUFFER);
	if (new == NULL)
	{
		free(*old);
		return (0);
	}
	i++;
	while (i-- > 0)
		new[i] = (*old)[i];
	if (*old != NULL)
		free(*old);
	*old = new;
	return (1);
}

char	*e_exit(char **str, char **ret)
{
	if (*str != NULL)
		free(*str);
	if (*ret != NULL)
		free(*ret);
	*str = NULL;
	*ret = NULL;
	return (NULL);
}

char	*s_exit(char **str, char **ret, size_t i, size_t ii)
{
	if ((*ret)[ii] != '\n')
	{
		if (*str != NULL)
			free(*str);
		(*ret)[ii] = '\0';
		*str = NULL;
		if ((*ret)[0] == '\0')
		{
			free(*ret);
			return (NULL);
		}
		return (*ret);
	}
	(*ret)[ii + 1] = '\0';
	get_moved(str, i);
	return (*ret);
}
