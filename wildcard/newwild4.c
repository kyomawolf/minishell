/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newwild4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 14:42:05 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/10 14:42:49 by jkasper          ###   ########.fr       */
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

void	*freetili(char **arr)
{
	int	i;

	i = 0;
	while (arr != NULL && arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	if (arr != NULL)
		free (arr);
	return (NULL);
}

char	**ft_realloc_charpp(char ***old, size_t new_size)
{
	char	**new;
	int		i;
	int		ii;

	new = ft_calloc(new_size, sizeof(char *));
	if (new == NULL)
		return (NULL);
	i = 0;
	while ((*old) != NULL && (*old)[i] != NULL)
	{
		ii = 0;
		new[i] = ft_calloc(ft_strlen((*old)[i]) + 1, 1);
		if (new[i] == NULL)
			return (freetili(new));
		while ((*old)[i][ii] != '\0')
		{
			new[i][ii] = (*old)[i][ii];
			ii++;
		}
		new[i][ii] = '\0';
		i++;
	}
	if (*old != NULL)
		freetili(*old);
	return (new);
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
