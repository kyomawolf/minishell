/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 18:23:37 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/04 18:04:15 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include "../include/structs.h"
#include "../lib/libft/libft.h"

char	*ft_t_word_get_str(t_word *word);
void	*ft_t_token_create(void *content);
void	*ft_t_node_create(void *content);
int		ft_append_token_helper(char *str, t_node **head, int type, int status);
void	*ft_t_node_get_last(void *head);
void	ft_t_node_add_back(t_node **head, t_node *node);
void	ft_skip_set(char *input, int *pos, char *set);
int		ft_t_word_init(t_word *word);
int		ft_t_word_append_char(t_word *word, char c);

// terminates word, and creates token and node element to store the token
int	ft_terminate_word_and_add_to_list(t_node **list, t_word *word)
{
	char	*str;

	str = ft_t_word_get_str(word);
	if (str == NULL)
		return (1);
	if (ft_append_token_helper(str, list, WORD, -1))
		return (1);
	ft_t_word_init(word);
	return (0);
}

// gets variable name, checks for valid variable name as well
char	*ft_get_var_name2(char *str, int i)
{
	int		i_start;
	char	*var_name;

	i_start = i;
	if (str[i] == '?')
	{
		var_name = malloc(2);
		var_name[0] = '?';
		var_name[1] = '\0';
		return (var_name);
	}
	if (str[i] == VAR_END)
	{
		var_name = malloc (2);
		var_name[0] = '$';
		var_name[1] = '\0';
		return (var_name);
	}
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (NULL);
	while (str[i] != VAR_END)
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (NULL);
		i++;
	}
	var_name = ft_substr(str, i_start, i - i_start);
	return (var_name);
}

// appends chars to word in case of a single quoted variable
void	ft_handle_squoted_var(t_node *head, t_word *word, int *i)
{
	t_token	*token;

	token = (t_token *)head->content;
	ft_t_word_append_char(word, token->string[*i]);
	*i = *i + 2;
	while (token->string[*i] != VAR_END)
	{
		ft_t_word_append_char(word, token->string[*i]);
		(*i)++;
	}
}

// appends chars to word in case of an unquoted variable
//  creates new word and token if necessary
void	ft_handle_unquoted_var(t_word *word, t_node **list, \
			char **var)
{
	int	j;

	j = 0;
	while (var[1][j] != '\0')
	{
		if (ft_whitespaces(var[1][j]))
		{
			ft_skip_set(var[1], &j, " \t\n");
			ft_terminate_word_and_add_to_list(list, word);
			continue ;
		}
		ft_t_word_append_char(word, var[1][j]);
		if (var[1][j] == '*')
			ft_t_word_append_char(word, -2);
		j++;
	}
}

// appends chars to word in case of a double quoted variable
void	ft_handle_dquoted_var(t_word *word, char **var)
{
	int	j;

	j = 0;
	while (var[1][j] != '\0')
	{
		if (var[1][j] == '*')
		{
			ft_t_word_append_char(word, var[1][j]);
			ft_t_word_append_char(word, -1);
		}
		else
			ft_t_word_append_char(word, var[1][j]);
		j++;
	}
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

// guards against failed var_name in var_value conversion
void	ft_get_variable_name_guard(char **var)
{
	free(var[0]);
	var[0] = NULL;
	free(var);
	var = NULL;
}

// returns char array with variable name and variable value
char	**ft_get_variable_name(t_node *head, int *i)
{
	t_token	*token;
	char	**var;
	char	*var_value_copy;

	var = malloc(sizeof(char *) * 3);
	if (var == NULL)
		return (NULL);
	token = (t_token *)head->content;
	var[0] = ft_get_var_name2(token->string, *i + 2);
	if (var[0] == NULL)
		return (NULL);
	if ((var[0][0] == '?' || var[0][0] == '$') && var[0][1] == '\0')
	{
		var_value_copy = ft_strdup(var[0]);
		if (var_value_copy == NULL)
		{
			ft_get_variable_name_guard(var);
			return (NULL);
		}
		var[1] = var_value_copy;
	}
	else
	{
		var[1] = getenv(var[0]);
		var_value_copy = ft_strdup(var[1]);
		if (var_value_copy == NULL)
		{
			ft_get_variable_name_guard(var);
			return (NULL);
		}
		var[1] = var_value_copy;
	}
	var[2] = NULL;
	return (var);
}

// handles how the variable should be expanded according to the quoted state
int	ft_handle_var_expansion(t_node *head, t_word *word, t_node **list, int *i)
{
	t_token	*token;
	char	quoted_status;
	char	**var;
	int		len;

	token = (t_token *)head->content;
	quoted_status = token->string[(*i) + 1];
	if (quoted_status == VAR_SQUOTED)
		ft_handle_squoted_var(head, word, i);
	else
	{
		var = ft_get_variable_name(head, i);
		if (var == NULL)
			return (1);
		if (quoted_status == VAR_UQUOTED)
			ft_handle_unquoted_var(word, list, var);
		else if (quoted_status == VAR_DQUOTED)
			ft_handle_dquoted_var(word, var);
		if (!ft_strncmp(var[0],"$", 2))
			*i = *i - 1;
		len = ft_strlen(var[0]);
		*i = *i + len + 2;
		ft_free_char_array(var);
	}
	return (0);
}

// terminates a word. Addes token to the token list
void	ft_terminate_word(t_word *word, t_node **list)
{
	if (word->write_head != 0)
		ft_terminate_word_and_add_to_list(list, word);
	if (word->chars != NULL)
	{
		free(word->chars);
		word->chars = NULL;
	}
}

// initializes the variable expansion
t_node	*ft_init_var_expansion(t_node *head)
{
	t_token	*token;
	int		i;
	t_word	word;
	t_node	*list;

	list = NULL;
	ft_t_word_init(&word);
	token = (t_token *)head->content;
	i = 0;
	while (token->string[i] != '\0')
	{
		if (token->string[i] == '$')
		{
			if (ft_handle_var_expansion(head, &word, &list, &i))
				return (NULL);
			i++;
			continue ;
		}
		ft_t_word_append_char(&word, token->string[i++]);
	}
	ft_terminate_word(&word, &list);
	return (list);
}

// exchanges token with information about var_expansion with expaned tokens
void	ft_exchange_tokens(t_node **head, t_node *list)
{
	t_node	*last_in_list;

	if ((*head)->prev != NULL)
		((t_node *)(*head)->prev)->next = list;
	last_in_list = ft_t_node_get_last(list);
	if ((*head)->next != NULL)
		((t_node *)(*head)->next)->prev = last_in_list;
	list->prev = (*head)->prev;
	last_in_list->next = (*head)->next;
	free(((t_token *)(*head)->content)->string);
	((t_token *)(*head)->content)->string = NULL;
	free((t_token *)(*head)->content);
	(*head)->content = NULL;
	free(*head);
	*head = last_in_list;
}

//loops through string and searches for char $.
// if necessary calls variable expansion initialization function
//  and calls function to exchange the old token with the new ones.
int	ft_check_for_var_expansion(t_node **head)
{
	t_token	*token;
	t_node	*list;
	int		i;

	token = (t_token *)(*head)->content;
	if (token->type == WORD)
	{
		i = 0;
		while (token->string[i] != '\0')
		{
			if (token->string[i] == '$')
			{
				list = ft_init_var_expansion(*head);
				if (list == NULL)
					return (1);
				ft_exchange_tokens(head, list);
				break ;
			}
			i++;
		}
	}
	return (0);
}

//sets *head to the beginning of the list
void	ft_get_beginning_of_list(t_node *temp, t_node **head)
{
	*head = temp;
	while ((*head)->prev != NULL)
		*head = (*head)->prev;
}

//loops through list of token and checks if variable expansion is necessary
int	ft_var_expansion(t_node **head)
{
	t_node	*temp;

	while (*head != NULL)
	{
		if (ft_check_for_var_expansion(head))
			return (1);
		temp = *head;
		*head = (*head)->next;
	}
	ft_get_beginning_of_list(temp, head);
	return (0);
}
