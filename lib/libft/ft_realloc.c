/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/24 17:17:17 by jkasper           #+#    #+#             */
/*   Updated: 2021/07/24 19:21:29 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static void	*ft_relong(size_t newsize, size_t size, void *old)
{
	long	*new;
	size_t	count;

	count = 0;
	new = malloc(newsize * sizeof(long));
	while (count < size)
	{
		new[count] = ((long *)old)[count];
		count++;
	}
	return (new);
}

static void	*ft_rechar(size_t newsize, size_t size, void *old)
{
	char	*new;
	size_t	count;

	count = 0;
	new = malloc(newsize * sizeof(char));
	while (count < size)
	{
		new[count] = ((char *)old)[count];
		count++;
	}
	return (new);
}

static void	*ft_reint(size_t newsize, size_t size, void *old)
{
	int		*new;
	size_t	count;

	count = 0;
	new = malloc(newsize * sizeof(int));
	while (count < size)
	{
		new[count] = ((int *)old)[count];
		count++;
	}
	return (new);
}

void	*ft_realloc(void *old, size_t newsize, size_t size, int i)
{
	void		*new;

	new = NULL;
	if (i == e_type_CHAR)
		new = ft_rechar(newsize, size, old);
	else if (i == e_type_INT)
		new = ft_reint(newsize, size, old);
	else if (i == e_type_LONG)
		new = ft_relong(newsize, size, old);
	free(old);
	return (new);
}
