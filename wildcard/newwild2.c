/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newwild2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 14:41:54 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/08 14:02:05 by mstrantz         ###   ########.fr       */
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

// if dir runs out of chars before matcher does: return 1 -> no match
// same if chars of matcher and dir are not the same
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

// returns 1 if pattern matches, 0 if not.
// loops through the remaining matchers
//  first if:  no wc as first char in matcher: checks pattern
//   second if: shifts index of dir according the matching pattern
//  1st if out of while: if last matcher is plan * : return 1
//  2nd if out of while: pattern not found, return 0
//  3rd if out of while: checks rest of strings for matching pattern
int	wild_sub_sub(char *dir, char **matcher, int i, int ii)
{
	while (matcher[ii + 1] != NULL && matcher[ii + 1][0] != '\0')
	{
		if (matcher[ii][0] != -1)
		{
			if (ft_strnstr(dir + i, matcher[ii], ft_strlen(dir) - i) == NULL)
				return (0);
		}
		if (matcher[ii][0] != -1)
			i = ft_strnstr(dir + i, matcher[ii], ft_strlen(dir) - i) + 1 - dir;
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

// returns 1 to keep file selected and 0 to unselect.
//  first if condition: checks for plane *. expands to everything given
//  second if condition: returns 0 if pattern of dir is not matching
//   with matcher[ii]
//  else: wc * found: calls wild_sub_sub with next matcher
int	wild_sub_match(char *dir, char **matcher)
{
	int	ii;

	ii = 0;
	if (matcher[0][0] == -1 && matcher[1][0] == '\0')
		return (1);
	if (matcher[ii][0] != -1)
	{
		if (dir - ft_strnstr(dir, matcher[ii], ft_strlen(dir)) != 0)
			return (0);
	}
	else
		ii++;
	return (wild_sub_sub(dir, matcher, 0, ii));
}

char	**wild_select(char **all_dir, int *ident, int i, int len)
{
	char	**ret;

	ret = ft_calloc(len + 1, sizeof(char *));
	if (ret == NULL)
		i = 0;
	while (i-- > 0)
	{
		if (ident[i])
		{
			ret[len - 1] = ft_strdup(all_dir[i]);
			if (ret[len - 1] == NULL)
			{
				free(ident);
				freetili(ret);
				break ;
			}
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

	len = 0;
	ident = ft_calloc(ft_char_arr_len(all_dir), sizeof(int));
	if (ident == NULL)
		return (NULL);
	i = 0;
	while (all_dir[i] != NULL)
	{
		ident[i] = wild_sub_match(all_dir[i], matcher);
		len += ident[i++];
	}
	return (wild_select(all_dir, ident, i, len));
}
