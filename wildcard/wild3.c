/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 21:53:31 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/16 00:49:59 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#include "libft.h"
#include "minis.h"
#include "structs.h"

char	**inner_part_2(char *string, int ii, char **ret, int iii)
{
	int	i;

	i = 0;
	if (string[++i] == -2)
	{
		if (iii != 0)
			ret[++ii] = ft_calloc(2, 1);
		ret[ii][0] = -1;
		if (string[i + 1] == '\0')
			return (ret);
		ret[++ii] = ft_calloc(ft_strlen(string) + 2, 1);
		iii = 0;
	}
	else
		ret[ii][iii] = '*';
	return (ret);
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
			ret = inner_part_2(string + i++, ii, ret, iii);
			if (ret[ii][iii] == '\0')
			{
				iii = -1;
				if (ret[ii + 1] == NULL)
					break ;
				ii += 2;
			}
		}
		else
			ret[ii][iii] = string[i];
		iii++;
		if (string[i - 1] == '\0')
			break ;
	}
	return (ret);
}

char	**wild_get_seperator(char *string)
{
	int		i;
	int		ii;
	int		iii;
	char	**ret;

	i = 0;
	ii = 0;
	iii = 0;
	ret = ft_calloc(ft_strlen(string) + 1, sizeof(char *));
	if (string[0] == '*')
	{
		if (string[1] == -2)
		{
			ret[ii] = ft_calloc(2, 1);
			ret[ii][0] = -1;
			ret[++ii] = ft_calloc(ft_strlen(string) + 2, 1);
		}
		else
			ret[ii][iii++] = string[i];
		i = 2;
		return (part_2(string, ii, iii, ret));
	}
	else
		ret[ii] = ft_calloc(ft_strlen(string) + 2, 1);
	return (part_2(string, ii, iii, ret));
}

char	**ft_realloc_charpp(char ***old, size_t new_size)
{
	char	**new;
	int		i;
	int		ii;

	new = ft_calloc(new_size, sizeof(char *));
	i = 0;
	while ((*old) != NULL && (*old)[i] != NULL)
	{
		ii = 0;
		new[i] = ft_calloc(ft_strlen((*old)[i]) + 1, 1);
		while ((*old)[i][ii] != '\0')
		{
			new[i][ii] = (*old)[i][ii];
			ii++;
		}
		new[i][ii] = '\0';
		free((*old)[i]);
		i++;
	}
	if ((*old) != NULL)
		free(*old);
	return (new);
}

void	free_char_array2(char ***arr)
{
	int	i;

	i = 0;
	while ((*arr)[i] != NULL)
	{
		free((*arr)[i]);
		i++;
	}
	free(*arr);
}
