/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 21:49:00 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/23 22:54:27 by mstrantz         ###   ########.fr       */
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

int	wild_comp_end(char *matcher, char *dir)
{
	int	i;
	int	ii;

	i = ft_strlen(matcher) - 1;
	ii = ft_strlen(dir) - 1;
	while (i >= 0)
	{
		if (ii < 0 || matcher[i--] != dir[ii--])
			return (1);
	}
	return (0);
}

int	wild_sub_sub(char *dir, char **matcher, int i, int ii)
{
	while (matcher[ii + 1] != NULL)
	{
		if (matcher[ii][0] != -1)
		{
			if (ft_strnstr(dir + i, matcher[ii], ft_strlen(dir) - i) == NULL)
				return (0);
		}
		if (matcher[ii][0] != -1)
			i += ft_strnstr(dir + i, matcher[ii], ft_strlen(dir) - i) + 1 - dir;
		ii++;
	}
	if (matcher[ii][0] == -1)
		return (1);
	if (ft_strnstr(dir + i, matcher[ii], ft_strlen(dir) - i) == NULL)
		return (0);
	if (wild_comp_end(matcher[ii], dir))
		return (0);
	return (1);
}

int	wild_sub_match(char *dir, char **matcher)
{
	int	i;
	int	ii;

	i = 0;
	ii = 0;
	if (matcher[0][0] == -1 && matcher[1][0] == '\0')
		return (1);
	if (matcher[ii][0] != -1)
	{
		if (dir - ft_strnstr(dir + i, matcher[ii], ft_strlen(dir) - i) != 0)
			return (0);
	}
	else
		ii++;
	return (wild_sub_sub(dir, matcher, i, ii));
}

char	**wild_select(char **all_dir, int *ident, int i, int len)
{
	char	**ret;

	ret = ft_calloc(len + 1, sizeof(char *));
	while (i-- > 0)
	{
		if (ident[i])
		{
			ret[len - 1] = ft_strdup(all_dir[i]);
			len--;
		}
	}
	free(ident);
	return (ret);
}

char	**wild_pattern_match(char **all_dir, char **matcher)
{
	int		len;
	int		*ident;
	int		i;
	int		sw;

	len = 0;
	ident = ft_calloc(ft_char_arr_len(all_dir), sizeof(int));
	i = 0;
	sw = 0;
	while (all_dir[i] != NULL)
	{
		ident[i] = wild_sub_match(all_dir[i], matcher);
		len += ident[i];
		if (ident[i++] != 0)
			sw = 1;
	}
	if (!sw)
		return (NULL);
	return (wild_select(all_dir, ident, i, len));
}
