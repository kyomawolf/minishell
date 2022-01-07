/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newwild3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 14:42:01 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/07 17:13:36 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#include "libft.h"
#include "minis.h"
#include "struct.h"

int	counting_stars(char *string)
{
	int	ret;
	int	i;

	ret = 1;
	i = 0;
	while (string[i] != '\0')
	{
		if (string[i] == '*')
			ret += 2;
		i++;
	}
	return (ret);
}

char	**inner_part_2(char *string, int *ii, char **ret, int *iii)
{
	int	i;

	i = 0;
	if (string[++i] == WC_END)
	{
		if (*iii != 0)
			++(*ii);
		ret[*ii][0] = -1;
		if (string[i + 1] == '\0')
			return (ret);
		*iii = 0;
	}
	else if (string[i] == WC_QUOTED)
		ret[*ii][*iii] = '*';
	return (ret);
}

int	part_2_reset_indices(char **ret, int *ii, int *iii)
{
	if (ret[*ii][*iii] == '\0')
	{
		*iii = -1;
		if (ret[(*ii) + 1] == NULL)
			return (1);
		(*ii) += 2;
	}
	else if (ret[*ii][*iii] == -1)
	{
		*iii = -1;
		(*ii)++;
	}
	return (0);
}

char	**part_2(char *string, int ii, int iii, char **ret)
{
	int	i;

	i = -1;
	if (ii != 0)
		i = 1;
	while (string[++i] != '\0')
	{
		if (string[i] == '*')
		{
			ret = inner_part_2(string + i++, &ii, ret, &iii);
			if (part_2_reset_indices(ret, &ii, &iii))
				break ;
		}
		else
			ret[ii][iii] = string[i];
		iii++;
		if (i > 0 && string[i - 1] == '\0')
			break ;
	}
	return (ret);
}

//regarding the line:
//	ret[i] = ft_calloc((ft_strlen(string) - size / 2) + 1);
//I substract the hidden characters in the string after the *[-1|-2]
char	**wild_get_seperator(char *string)
{
	int		i;
	int		ii;
	int		size;
	char	**ret;

	i = 0;
	ii = 0;
	size = counting_stars(string);
	ret = ft_calloc(size + 1, sizeof(char *));
	if (ret == NULL)
		return (NULL);
	while (i < size)
	{
		ret[i] = ft_calloc((ft_strlen(string) - size / 2) + 1, 1);
		i++;
	}
	i = 0;
	if (string[0] == '*')
	{
		if (string[1] == WC_END)
			ret[i++][0] = -1;
	}
	return (part_2(string, i, ii, ret));
}
