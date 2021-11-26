/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_copy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 19:28:13 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/26 22:23:54 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include "./include/libft.h"
#include "./include/minis.h"
#include <fcntl.h>

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

void	free_char_array(char ***arr)
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

int	wild_sub_match(char *str, char **match)
{
	int		i;
	int		ii;
	int		len;
	char	*temp;

	i = ft_char_arr_len(match) - 1;
	len = ft_strlen(str);
	if (match[i][0] != '*' && ft_strnstr(str, match[i], len) != NULL)
		len -= ft_strlen(match[i]);
	else if (match[i][0] != '*' && match[i][0] != '\0')
		return (0);
	i--;
	while (i >= 0 && match[i][0] != '\0')
	{
		if (i == 0 && match[0][0] != '*')
		{
			ii = 0;
			while (str[ii] != '\0' && match[0][ii] != '\0' \
			&& str[ii] == match[0][ii])
				ii++;
			if (match[0][ii] != '\0')
				return (0);
			return (1);
		}
		temp = ft_strnstr(str, match[i], len);
		if (match[i][0] != '*' && temp == NULL)
			return (0);
		if (temp != NULL)
			len -= str - temp;
		i--;
	}
	return (1);
}

char	**wild_pattern_match(char **all_dir, char **matcher)
{
	int		len;
	int		*ident;
	int		i;
	char	**ret;

	len = 0;
	ident = ft_calloc(ft_char_arr_len(all_dir), sizeof(int));
	i = 0;
	while (all_dir[i] != NULL)
	{
		ident[i] = wild_sub_match(all_dir[i], matcher);
		len += ident[i];
		i++;
	}
	ret = ft_calloc(len + 1, sizeof(char *));
	while (i-- > 0)
	{
		if (ident[i])
			ret[len-- - 1] = ft_strdup(all_dir[i]);
	}
	free(ident);
	return (ret);
}

char	**wild_get_narrows(char *string)
{
	int		i;
	int		ii;
	int		iii;
	int		ign;
	char	**ret;

	i = 0;
	ii = 0;
	iii = 0;
	ign = -1;
	ret = ft_calloc(ii + 2, sizeof(char *));
	if (ret == NULL)
		return (NULL);
	ret[0] = ft_calloc(ft_strlen(string), 1);
	if (ret[0] == NULL)
		return (NULL);
	if (string[i] == '*')
	{
		ret[ii++][0] = '*';
		ret = ft_realloc_charpp(&ret, ii + 2);
		ret[ii] = ft_calloc(ft_strlen(string), 1);
		i++;
	}
	while (string[i] != '\0')
	{
		if (string[i] == '\"')
			ign *= -1;
		else if (string[i] == '*' && ign == -1)
		{
			ret = ft_realloc_charpp(&ret, ii + 2 + 2);
			ret[++ii] = ft_calloc(ft_strlen(string), 1);
			ret[ii + 1] = ft_calloc(ft_strlen(string), 1);
			ret[ii++][0] = '*';
			iii = -1;
		}
		else
			ret[ii][iii] = string[i];
		i++;
		iii++;
	}
	ret[ii + 1] = NULL;
	return (ret);
}

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
	curr->content = (t_token *) ft_calloc(1, sizeof(t_token));
	((t_token *)curr->content)->string = sel_dir[0];
	((t_token *)curr->content)->type = WORD;
	ret = curr;
	i = 1;
	while (sel_dir[i] != NULL)
	{
		curr->next = ft_calloc(1, sizeof(t_node));
		((t_node *)curr->next)->prev = curr;
		curr = curr->next;
		curr->content = (t_token *) ft_calloc(1, sizeof(t_token));
		((t_token *)curr->content)->string = sel_dir[0];
		((t_token *)curr->content)->type = WORD;
		i++;
	}
	free_char_array(&sel_dir);
	return (ret);
}

t_node	*wild_main(char *string)
{
	char	**times;
	char	**all_dir;
	char	**selected_dir;

	times = wild_get_narrows(string);
	all_dir = wild_open_dir();
	selected_dir = wild_pattern_match(all_dir, times);
	free_char_array(&times);
	free_char_array(&all_dir);
	return (wild_combine(selected_dir));
}

int main(int argc, char **argv)
{
	t_node	*node;
	t_node	*temp;
	if (!(argc < 2))
	{
		node = wild_main(argv[1]);
		if (node == NULL)
			return (1);
		temp = node;
		while (temp != NULL)
		{
			printf("%s\n", (char *)temp->content);
			temp = temp->next;
		}
	}
}
