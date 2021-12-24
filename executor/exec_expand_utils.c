/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 20:04:22 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/24 02:29:39 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "exec.h"
#include "lexer.h"

//sets *head to the beginning of the list
int	ft_get_beginning_of_list(t_node *temp, t_node **head)
{
	int	ret;

	*head = temp;
	ret = 0;
	if (temp != NULL)
	{
		ret = 0;
		while ((*head)->prev != NULL)
		{
			*head = (*head)->prev;
		}
	}
	return (ret);
}

// terminates word, and creates token and node element to store the token
int	ft_terminate_word_and_add_to_list(t_expand *exp_data)
{
	char	*str;

	str = ft_t_word_get_str(exp_data->word);
	if (str == NULL)
		return (1);
	if (ft_append_token_helper(str, &(exp_data->list), WORD, -1))
	{
		return (1);
	}
	ft_t_word_init(exp_data->word);
	return (0);
}

// frees char array
void	ft_free_char_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
	arr = NULL;
}
