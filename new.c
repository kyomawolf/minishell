/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 15:42:10 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/11 15:52:51 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minis.h"
#include "libft.h"
#include "struct.h"

t_data	*new_t_data(void)
{
	t_data	*ret;

	ret = malloc(sizeof(t_data));
	ret->currdir = NULL;
	ret->currdir = getcwd(NULL, 0);
	ret->allcom = ft_calloc(1, sizeof(struct s_command));
	ret->input = NULL;
	get_prompt(ret);
	return (ret);
}