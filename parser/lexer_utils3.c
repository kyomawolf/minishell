/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 15:56:00 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/09 13:30:39 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "libft.h"
#include <stdlib.h>

// initializes struct s_word
int	ft_t_word_init(t_word *word)
{
	if (word)
	{
		word->type = -1;
		word->status = -1;
		word->w_head = 0;
		word->alloc = BUFFER_SIZE;
		word->chars = ft_calloc(sizeof(char), word->alloc);
		if (word->chars == NULL)
		{
			ft_putstr_fd("minishell: allocation error\n", 2);
			return (1);
		}
		return (0);
	}
	ft_putstr_fd("minishell: allocation error\n", 2);
	return (1);
}

//appends character to struct s_word member chars
int	ft_t_word_append_char(t_word *word, char c)
{
	unsigned int	new_alloc;
	char			*new_chars;

	if (word->w_head >= word->alloc)
	{
		new_alloc = word->w_head + word->alloc;
		new_chars = ft_calloc(new_alloc, sizeof(char));
		if (new_chars == NULL)
		{
			ft_putstr_fd("minishell: allocation error\n", 2);
			return (1);
		}
		ft_memcpy(new_chars, word->chars, word->w_head);
		free (word->chars);
		word->chars = NULL;
		word->chars = new_chars;
		word->alloc = new_alloc;
	}
	word->chars[word->w_head] = c;
	word->w_head++;
	return (0);
}

//creates a copy of the terminated string, frees struct s_word member chars
char	*ft_t_word_get_str(t_word *word)
{
	char	*str;

	if (word->w_head <= 0)
	{
		free(word->chars);
		word->chars = NULL;
		return (NULL);
	}
	str = ft_calloc(word->w_head + 1, sizeof(char));
	if (str == NULL)
	{
		ft_putstr_fd("minishell: allocation error\n", 2);
		free(word->chars);
		word->chars = NULL;
		return (NULL);
	}
	ft_memcpy(str, word->chars, word->w_head);
	str[word->w_head] = '\0';
	free (word->chars);
	word->chars = NULL;
	return (str);
}

//skips a set of characters in an inputstring
void	ft_skip_set(char *input, int *pos, char *set)
{
	while (ft_strchr(set, input[*pos]) && input[*pos] != '\0')
		(*pos)++;
}
