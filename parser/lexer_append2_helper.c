/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_append2_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 00:30:52 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/08 01:40:30 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static char	*ft_lexer_get_var_name_special(char *str, int i)
{
	char	*var_name;

	if (str[i] == '?')
	{
		var_name = malloc(2);
		if (var_name == NULL)
			return (NULL);
		var_name[0] = '?';
		var_name[1] = '\0';
		return (var_name);
	}
	return ((void *) 1);
}

//checks if var_name is valid and returns allocated string with var_name
char	*ft_lexer_get_var_name(char *str, int i)
{
	int		i_start;
	char	*var_name;
	int		flag;

	i_start = i;
	var_name = ft_lexer_get_var_name_special(str, i);
	if (var_name == NULL)
		return (NULL);
	else if (var_name != (void *) 1)
		return (var_name);
	flag = 1;
	if (ft_isdigit(str[i]))
		flag = 0;
	if (ft_isalnum(str[i] || str[i] == '_'))
		i++;
	if (flag)
	{
		while (ft_isalnum(str[i]) || str[i] == '_')
			i++;
	}
	var_name = ft_substr(str, i_start, i - i_start);
	return (var_name);
}
