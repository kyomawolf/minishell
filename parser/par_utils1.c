/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 16:19:49 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/15 16:37:02 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minis.h"
#include "libft.h"

struct s_simple_com	*utils_copy_simple_com(struct s_simple_com *old)
{
	struct s_simple_com	*new;

	new = malloc(sizeof(struct s_simple_com));
	new->number_arguments = old->number_arguments;
	new->arguments = malloc(new->number_arguments + 1 * sizeof(char *));
	while (i < new->number_arguments)
	{
		new->arguments[i] = ft_strdup(old->arguments[i]);
		i++;
	}
	new->arguments[i] = NULL;
}

struct s_simple_com	**utils_realloc_simple_com_list(struct s_simple_com ***old,
		int old_num, int new_num)
{
	int					i;
	int					ii;
	struct s_simple_com	**new;

	i = 0;
	ii = 0;
	new = ft_calloc(new_num, sizeof(struct s_simple_com *));
	if (new == NULL)
		return (NULL);
	while (i < old_num)
	{
		new[i] = utils_copy_simple_com((*old)[i]);
		if (new[i] == NULL)
			break ;
		i++;
	}
	free_simple_com_list(old, old_num);
	if (new[i] == NULL && i != old_num)
		return (free_simple_com_list(&new, i));
	return (new);
}
