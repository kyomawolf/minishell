/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 19:28:13 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/18 12:46:14 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

int		wild_check(t_token *token);

void	wild_replace(char ***arr, char **matches, int start);

char	**wild_pattern_match(char **all_dir, char **matcher);

char	**wild_get_narrows(char *string)
{
	int		i;
	int		ii;
	char	**ret;

	i = 0;
	ii = 0;
	ret = ft_calloc(2, sizeof(char *));
	while (string[i] != '\0')
	{
		if (string[i] == '*')
		{
			ii++;
			ret = re_alloc_charpp(ret, ii + 2);
		}
		else
			ret[ii] = string[i];
		i++;
	}
	return (ret);
}

char	**wild_open_dir(void)
{
	DIR			*open_dir;
	dirent		*cur_dir;
	char		*cwd;
	char		**ret;
	int			size;

	cwd = getcwd(NULL, 0);
	open_dir = opendir(cwd);
	free(cwd);
	cur_dir = readdir(open_dir);
	ret = NULL;
	size = 1;
	while (cur_dir != NULL)
	{
		ret = re_alloc_charpp(ret, size);
		ret[size - 1] = ft_strdup(cur_dir->d_name);
		free(cur_dir);
		cur_dir = readdir(open_dir);
		size++;
	}
	free(open_dir);
	return (ret);
}

void	wild_replace(char ***arr, char **sel_dir, int start)
{
	int		ii;
	int		iii;
	char	**new;

	new = ft_calloc(char_arr_len(*arr) + char_arr_len(sel_dir) + 1, sizeof(char *));//..,todo
	iii = 0;
	ii = 0;
	while (iii < start)
	{
		new[iii] = ft_strdup(*arr[iii]);
		iii++;
	}
	while (sel_dir[ii] != NULL)
	{
		new[iii] == ft_strdup(sel_dir[ii]);
		ii++;
		iii++;
	}
	while (*arr[start] != NULL)
		new[iii++] = ft_strdup((*arr)[start++]);
	free_char_array(*arr);
	*arr = new;
}

void	wild_start_expand(char ***arr, int i)
{
	char	**times;
	char	**all_dir;
	char	**selected_dir;

	times = wild_get_narrows((*arr)[i]);
	alldir = wild_open_dir();
	selected_dir = wild_pattern_match(alldir, times);//todo
	wild_replace(arr, selected_dir, i);
	free_char_array(&times);
	free_char_array(&all_dir);
	free_char_array(&selected_dir);
}

int	wild_main(t_node **head)
{
	t_node	*temp;

	temp = *head;
	while (temp != NULL)
	{
		wild_check(temp->content);
		temp = temp->next;
	}
}
