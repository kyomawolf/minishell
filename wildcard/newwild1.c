/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newwild1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 14:41:48 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/10 14:43:34 by jkasper          ###   ########.fr       */
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

void	inloop(struct dirent *cur_dir, char ***ret, int *size, char first)
{
	if ((cur_dir->d_name[0] == '.' && first == '.') || \
		cur_dir->d_name[0] != '.')
	{
		*ret = ft_realloc_charpp(ret, *size + 2);
		(*ret)[*size] = ft_strdup(cur_dir->d_name);
		(*ret)[*size][ft_strlen(cur_dir->d_name)] = '\0';
		(*size)++;
	}
}

char	**wild_open_dir(char first)
{
	DIR				*open_dir;
	struct dirent	*cur_dir;
	char			*cwd;
	char			**ret;
	int				size;

	cwd = getcwd(NULL, 0);
	open_dir = opendir(cwd);
	free(cwd);
	if (first != '.')
		cur_dir = readdir(open_dir);
	if (first != '.')
		cur_dir = readdir(open_dir);
	cur_dir = readdir(open_dir);
	ret = NULL;
	size = 0;
	while (cur_dir != NULL)
	{
		inloop(cur_dir, &ret, &size, first);
		cur_dir = readdir(open_dir);
	}
	closedir(open_dir);
	ret[size] = NULL;
	return (ret);
}

t_node	*formatted_string_node(char *ret)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (node == NULL)
	{
		free(ret);
		return (NULL);
	}
	node->content = ft_calloc(1, sizeof(t_token));
	if (node->content == NULL)
	{
		free (ret);
		free (node);
		return (NULL);
	}
	((t_token *)node->content)->string = ret;
	((t_token *)node->content)->type = WORD;
	return (node);
}

t_node	*formatted_string(char *string, char **selected_dir)
{
	int		i;
	int		ii;
	char	*ret;

	ret = ft_calloc(1, ft_strlen(string));
	if (ret == NULL)
		return (NULL);
	i = 0;
	ii = 0;
	while (string[ii] != '\0')
	{
		if (string[ii] == -1 || string[ii] == -2)
		{
			ii++;
			continue ;
		}
		ret[i++] = string[ii++];
	}
	free(selected_dir);
	return (formatted_string_node(ret));
}

t_node	*wild_main(char *string)
{
	char	**matcher;
	char	**all_dir;
	char	**selected_dir;

	matcher = wild_get_seperator(string);
	if (matcher == NULL)
		return (NULL);
	all_dir = wild_open_dir(matcher[0][0]);
	if (all_dir == NULL)
	{
		free_char_array(&matcher);
		return (NULL);
	}
	selected_dir = wild_pattern_match(all_dir, matcher);
	if (selected_dir == NULL)
	{
		free_char_array(&matcher);
		free_char_array(&all_dir);
		return (NULL);
	}
	free_char_array(&matcher);
	free_char_array(&all_dir);
	if (selected_dir[0] == NULL)
		return (formatted_string(string, selected_dir));
	return (wild_combine(selected_dir));
}
