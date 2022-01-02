/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 21:47:48 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/02 02:17:42 by mstrantz         ###   ########.fr       */
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

t_node	*formatted_string(char *string)
{
	int		i;
	int		ii;
	char	*ret;
	t_node	*node;

	ret = ft_calloc(1, ft_strlen(string));
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
	node = ft_calloc(1, sizeof(t_node));
	node->content = ft_calloc(1, sizeof(t_token));
	((t_token *)node->content)->string = ret;
	((t_token *)node->content)->type = WORD;
	return (node);
}


/* //debug
void	ft_print_str_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i] && arr[i][0] != '\0')
	{
		printf("[%d]:%s:\n", i, arr[i]);
		i++;
	}
	printf("next\n");
} */

t_node	*wild_main(char *string)
{
	char	**times;
	char	**all_dir;
	char	**selected_dir;

	times = wild_get_seperator(string);
	all_dir = wild_open_dir();
	selected_dir = wild_pattern_match(all_dir, times);
	free_char_array(&times);
	free_char_array(&all_dir);
	if (selected_dir == NULL)
		return (formatted_string(string));
	return (wild_combine(selected_dir));
}
