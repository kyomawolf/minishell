/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newwild1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 14:41:48 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/07 15:32:52 by jkasper          ###   ########.fr       */
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

char	**wild_open_dir(void)
{
	DIR				*open_dir;
	struct dirent	*cur_dir;
	char			*cwd;
	char			**ret;
	int				size;

	cwd = getcwd(NULL, 0);
	open_dir = opendir(cwd);
	free(cwd);
	cur_dir = readdir(open_dir);
	cur_dir = readdir(open_dir);
	cur_dir = readdir(open_dir);
	ret = NULL;
	size = 0;
	while (cur_dir != NULL)
	{
		ret = ft_realloc_charpp(&ret, size + 2);
		ret[size] = ft_strdup(cur_dir->d_name);
		ret[size][ft_strlen(cur_dir->d_name)] = '\0';
		cur_dir = readdir(open_dir);
		size++;
	}
	closedir(open_dir);
	ret[size] = NULL;
	return (ret);
}

t_node	*wild_combine(char **sel_dir)
{
	t_node	*ret;
	t_node	*curr;
	int		i;

	curr = ft_calloc(1, sizeof(t_node));
	curr->content = ft_calloc(1, sizeof(t_token));
	((t_token *)curr->content)->string = sel_dir[0];
	((t_token *)curr->content)->type = WORD;
	ret = curr;
	i = 1;
	while (sel_dir[i] != NULL)
	{
		curr->next = ft_calloc(1, sizeof(t_node));
		((t_node *)curr->next)->prev = curr;
		curr = curr->next;
		curr->content = ft_calloc(1, sizeof(t_token));
		((t_token *)curr->content)->string = sel_dir[i];
		((t_token *)curr->content)->type = WORD;
		i++;
	}
	free(sel_dir);
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
	all_dir = wild_open_dir();
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
