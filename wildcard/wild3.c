/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 21:53:31 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/02 02:15:13 by mstrantz         ###   ########.fr       */
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

char	**inner_part_2(char *string, int *ii, char **ret, int *iii)
{
	int	i;

	i = 0;
	if (string[++i] == WC_END)
	{
		if (*iii != 0)
			ret[++(*ii)] = ft_calloc(2, 1);
		ret[*ii][0] = -1;
		if (string[i + 1] == '\0')
			return (ret);
		ret[++(*ii)] = ft_calloc(ft_strlen(string) + 2, 1);
		(*ii)--;
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

char	**wild_get_seperator(char *string)
{
	int		ii;
	int		iii;
	char	**ret;

	ii = 0;
	iii = 0;
	ret = ft_calloc(ft_strlen(string) + 1, sizeof(char *));
	if (string[0] == '*')
	{
		if (string[1] == WC_END)
		{
			ret[ii] = ft_calloc(2, 1);
			ret[ii][0] = -1;
			ret[++ii] = ft_calloc(ft_strlen(string) + 2, 1);
		}
		else
			ret[ii] = ft_calloc(ft_strlen(string) + 2, 1);
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
