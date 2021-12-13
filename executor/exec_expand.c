/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 17:29:40 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/13 21:58:02 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include "../include/structs.h"
#include "../lib/libft/libft.h"
#include "../include/minis.h"

char	*ft_t_word_get_str(t_word *word);
void	*ft_t_token_create(void *content);
void	*ft_t_node_create(void *content);
int		ft_append_token_helper(char *str, t_node **head, int type, int status);
void	*ft_t_node_get_last(void *head);
void	ft_t_node_add_back(t_node **head, t_node *node);
void	ft_skip_set(char *input, int *pos, char *set);
int		ft_t_word_init(t_word *word);
int		ft_t_word_append_char(t_word *word, char c);
int	ft_var_expansion2(t_node **head);
int	ft_check_for_var_expansion(t_node **head);

// terminates word, and creates token and node element to store the token
int	ft_terminate_word_and_add_to_list1(t_node **list, t_word *word)
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
char	*ft_get_var_name21(char *str, int i)
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
void	ft_handle_squoted_var1(t_node *head, t_word *word, int *i)
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
void	ft_handle_unquoted_var1(t_word *word, t_node **list, \
			char **var)
{
	int	j;

	j = 0;
	while (var[1][j] != '\0')
	{
		if (ft_whitespaces(var[1][j]))
		{
			ft_skip_set(var[1], &j, " \t\n");
			ft_terminate_word_and_add_to_list1(list, word);
			continue ;
		}
		ft_t_word_append_char(word, var[1][j]);
		if (var[1][j] == '*')
			ft_t_word_append_char(word, -2);
		j++;
	}
}

// appends chars to word in case of a double quoted variable
void	ft_handle_dquoted_var1(t_word *word, char **var)
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
void	ft_free_char_array1(char **arr)
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
void	ft_get_variable_name_guard1(char **var)
{
	free(var[0]);
	var[0] = NULL;
	free(var);
	var = NULL;
}

// returns char array with variable name and variable value
char	**ft_get_variable_name1(t_node *head, int *i)
{
	t_token	*token;
	char	**var;
	char	*var_value_copy;

	var = malloc(sizeof(char *) * 3);
	if (var == NULL)
		return (NULL);
	token = (t_token *)head->content;
	var[0] = ft_get_var_name21(token->string, *i + 2);
	if (var[0] == NULL)
		return (NULL);
	if ((var[0][0] == '?' || var[0][0] == '$') && var[0][1] == '\0')
	{
		var_value_copy = ft_strdup(var[0]);
		if (var_value_copy == NULL)
		{
			ft_get_variable_name_guard1(var);
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
			ft_get_variable_name_guard1(var);
			return (NULL);
		}
		var[1] = var_value_copy;
	}
	var[2] = NULL;
	return (var);
}

// handles how the variable should be expanded according to the quoted state
int	ft_handle_var_expansion1(t_node *head, t_word *word, t_node **list, int *i)
{
	t_token	*token;
	char	quoted_status;
	char	**var;
	int		len;

	token = (t_token *)head->content;
	quoted_status = token->string[(*i) + 1];
	if (quoted_status == VAR_SQUOTED)
		ft_handle_squoted_var1(head, word, i);
	else
	{
		var = ft_get_variable_name1(head, i);
		if (var == NULL)
			return (1);
		if (quoted_status == VAR_UQUOTED)
			ft_handle_unquoted_var1(word, list, var);
		else if (quoted_status == VAR_DQUOTED)
			ft_handle_dquoted_var1(word, var);
		if (!ft_strncmp(var[0],"$", 2))
			*i = *i - 1;
		len = ft_strlen(var[0]);
		*i = *i + len + 2;
		ft_free_char_array1(var);
	}
	return (0);
}

// terminates a word. Addes token to the token list
void	ft_terminate_word1(t_word *word, t_node **list)
{
	if (word->write_head != 0)
		ft_terminate_word_and_add_to_list1(list, word);
	if (word->chars != NULL)
	{
		free(word->chars);
		word->chars = NULL;
	}
}

// initializes the variable expansion
t_node	*ft_init_var_expansion1(t_node *head)
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
			if (ft_handle_var_expansion1(head, &word, &list, &i))
				return (NULL);
			i++;
			continue ;
		}
		ft_t_word_append_char(&word, token->string[i++]);
	}
	ft_terminate_word1(&word, &list);
	return (list);
}

// exchanges token with information about var_expansion with expaned tokens
void	ft_exchange_tokens1(t_node **head, t_node *list)
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

///////////////////////////////////////////////////////////////////////////////

int	ft_t_node_size(t_node *head)
{
	int	len;

	len = 0;
	while (head != NULL)
	{
		head = head->next;
		len++;
	}
	return (len);
}

void	ft_exchange_str_arr(char ***str_arr, t_node *head)
{
	int	len;
	int	i;
	t_node	*temp;

	len = ft_t_node_size(head);
	*str_arr = ft_calloc(len + 1, sizeof(char *));
	i = 0;
	while (head != NULL)
	{
		(*str_arr)[i] = ((t_token *)head->content)->string;
		free((t_token *)head->content);
		head->content = NULL;
		temp = head;
		head = head->next;
		free(temp);
		temp = NULL;
		i++;
	}
	(*str_arr)[i] = NULL;
}

int	ft_str_array_var_expansion(char ***str_arr)
{
	t_node	*head;

	if (*str_arr != NULL)
	{
		head = NULL;
		head = wild_combine(*str_arr);
		if (ft_var_expansion2(&head))
			return (1);
		if (ft_wildcard_expansion(&head))
			return (1);
		ft_exchange_str_arr(str_arr, head);
	}
	return (0);
}

char	*ft_heredoc_get_var_name(char *str, int i)
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
	if (!ft_isalnum(str[i]) && str[i] != '_')
	{
		var_name = malloc (2);
		var_name[0] = '$';
		var_name[1] = '\0';
		return (var_name);
	}
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (NULL);
	while (ft_isalnum(str[i]) || str[i] == '_')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (NULL);
		i++;
	}
	var_name = ft_substr(str, i_start, i - i_start);
	return (var_name);
}

char **ft_heredoc_get_variable_name(t_node *node, int i)
{
	char	**var;
	char	*var_value_copy;

	var = malloc(sizeof(char *) * 3);
	if (var == NULL)
		return (NULL);
	var[0] = ft_heredoc_get_var_name(node->content, i);
	if (var[0] == NULL)
		return (NULL);
	if ((var[0][0] == '?' || var[0][0] == '$') && var[0][1] == '\0')
	{
		var_value_copy = ft_strdup(var[0]);
		if (var_value_copy == NULL)
		{
			ft_get_variable_name_guard1(var);
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
			ft_get_variable_name_guard1(var);
			return (NULL);
		}
		var[1] = var_value_copy;
	}
	var[2] = NULL;
	return (var);
}

int	ft_heredoc_handle_var_expansion(t_node *node, t_word *word, int *i)
{
	char	**var;
	int		j;

	var = ft_heredoc_get_variable_name(node, *i + 1);
	if (var == NULL)
		return (1);
	j = 0;
	while (var[1][j] != '\0')
	{
		ft_t_word_append_char(word, var[1][j]);
		j++;
	}
	j = 0;
	while (var[0][j++] != '\0')
		(*i)++;
	ft_free_char_array1(var);
	return (0);
}

int	ft_heredoc_uqvar_expansion(t_node **node)
{
	t_word	word;
	int		i;

	ft_t_word_init(&word);
	i = 0;
	while (((char *)(*node)->content)[i] != '\0')
	{
		if (((char *)(*node)->content)[i] == '$')
		{
			if (ft_heredoc_handle_var_expansion(*node, &word, &i))
				return (1);
			i++;
			continue ;
		}
		ft_t_word_append_char(&word, ((char *)(*node)->content)[i++]);
	}
	free((*node)->content);
	(*node)->content = ft_t_word_get_str(&word);
	return (0);
}

void	ft_heredoc_uqvar_check_expansion(t_node **node)
{
	t_node	*temp;

	temp = *node;
	while (*node != NULL)
	{
		if (ft_strchr((*node)->content, '$'))
		{
			ft_heredoc_uqvar_expansion(node);
		}
		*node = (*node)->next;
	}
	ft_get_beginning_of_list(temp, node);
}
void	ft_heredoc_var_expansion(t_io *io)
{
	if (io != NULL)
	{
		if (io->quoted_status == VAR_UQUOTED)
		{
			ft_heredoc_uqvar_check_expansion(&(io->heredoc_node));
		}
	}
}

int	ft_check_for_var_expansion1(t_node *head)
{
	t_bin	*tree;

	tree = (t_bin *)head->content;
	if (tree == NULL)
		return (0);
	else
	{
		ft_str_array_var_expansion(&(tree->command->arguments));
		if (tree->io != NULL)
		{
			ft_heredoc_var_expansion(tree->io);
			ft_str_array_var_expansion(&(tree->io->input));
			ft_str_array_var_expansion(&(tree->io->output));
		}
	}
	return (0);
}

//loops through list of t_bin and checks if variable expansion is necessary
int	ft_var_expansion1(t_node *head)
{
	while (head != NULL)
	{
		if (ft_check_for_var_expansion1(head))
			return (1);
		head = head->next;
	}
	return (0);
}

int	ft_var_expansion2(t_node **head)
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