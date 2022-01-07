/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newwild4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 14:42:05 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/07 15:36:30 by jkasper          ###   ########.fr       */
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
