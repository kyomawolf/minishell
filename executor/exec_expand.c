/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 17:29:40 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/16 15:32:40 by mstrantz         ###   ########.fr       */
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
int		ft_t_token_variable_expansion(t_node **head, t_data *data);
void	ft_s_node_print_content(t_node *head);

// terminates word, and creates token and node element to store the token
int	ft_terminate_word_and_add_to_list(t_node **list, t_word **word)
{
	char	*str;

	str = ft_t_word_get_str(*word);
	if (str == NULL)
		return (1);
	if (ft_append_token_helper(str, list, WORD, -1))
		return (1);
	ft_t_word_init(*word);
	return (0);
}

char	*ft_get_var_name_special_cases(char *str, int i)
{
	char	*var_name;

	var_name = NULL;
	if (str[i] == '?')
	{
		var_name = ft_calloc(2, sizeof(char));
		var_name[0] = '?';
		var_name[1] = '\0';
	}
	else if (str[i] == VAR_END)
	{
		var_name = ft_calloc(2, sizeof(char));
		var_name[0] = '$';
		var_name[1] = '\0';
	}
	return (var_name);
}

// gets variable name, checks for valid variable name as well
char	*ft_expander_get_var_name(char *str, int i)
{
	int		i_start;
	char	*var_name;

	i_start = i;
	if (str[i] == '?' || str[i] == VAR_END)
		return (ft_get_var_name_special_cases(str, i));
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
			if (word->write_head != 0)
			{
				ft_terminate_word_and_add_to_list(list, &word);
			}
			continue ;
		}
		ft_t_word_append_char(word, var[1][j]);
		if (var[1][j] == '*')
		{
			ft_t_word_append_char(word, -2);
		}
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

void	ft_set_variable_name_and_value(char ***var, t_data *data)
{
	char	*var_value_copy;

	var_value_copy = NULL;
	if ((var[0][0][0] == '?' || var[0][0][0] == '$') && var[0][0][1] == '\0')
		var_value_copy = ft_strdup(var[0][0]);
	else
	{
		var[0][1] = mini_getenv(data, var[0][0]);
		var_value_copy = ft_strdup(var[0][1]);
	}
	if (var_value_copy == NULL)
	{
		ft_get_variable_name_guard(*var);
		return ;
	}
	var[0][1] = var_value_copy;
	var[0][2] = NULL;
}

// returns char array with variable name and variable value
char	**ft_get_variable_name_and_value(t_node **head, int *i, t_data *data)
{
	t_token	*token;
	char	**var;

	var = ft_calloc(3, sizeof(char *));
	if (var == NULL)
		return (NULL);
	token = (t_token *)(*head)->content;
	var[0] = ft_expander_get_var_name(token->string, *i + 2);
	if (var[0] == NULL)
		return (NULL);
	ft_set_variable_name_and_value(&var, data);
	return (var);
}

// handles how the variable should be expanded according to the quoted state
int	ft_handle_var_expansion(t_node **head, t_expand *exp_data, t_data *data)
{
	t_token	*token;
	char	quoted_status;
	char	**var;
	int		len;

	token = (t_token *)(*head)->content;
	quoted_status = token->string[exp_data->i + 1];
	if (quoted_status == VAR_SQUOTED)
		ft_handle_squoted_var(*head, exp_data->word, &(exp_data->i));
	else
	{
		var = ft_get_variable_name_and_value(head, &(exp_data->i), data);
		if (var == NULL)
			return (1);
		if (quoted_status == VAR_UQUOTED)
			ft_handle_unquoted_var(exp_data->word, &(exp_data->list), var);
		else if (quoted_status == VAR_DQUOTED)
			ft_handle_dquoted_var(exp_data->word, var);
		if (!ft_strncmp(var[0], "$", 2))
			(exp_data->i) = (exp_data->i) - 1;
		len = ft_strlen(var[0]);
		(exp_data->i) = (exp_data->i) + len + 2;
		ft_free_char_array(var);
	}
	return (0);
}

// terminates a word. Addes token to the token list
void	ft_terminate_word(t_word *word, t_node **list)
{
	if (word->write_head != 0)
	{
		ft_terminate_word_and_add_to_list(list, &word);
	}
	if (word->chars != NULL)
	{
		free(word->chars);
		word->chars = NULL;
	}
	free(word);
	word = NULL;
}

void	ft_t_expand_init(t_expand *exp_data)
{
	exp_data->i = 0;
	exp_data->list = NULL;
	exp_data->word = ft_calloc(1, sizeof(t_word));
	ft_t_word_init(exp_data->word);
}

// initializes the variable expansion
t_node	*ft_init_var_expansion(t_node **head, t_data *data)
{
	t_token	*token;
	char	*temp;
	t_expand	exp_data;

	ft_t_expand_init(&exp_data);
	token = (t_token *)(*head)->content;
	temp = token->string;
	while (temp[exp_data.i] != '\0')
	{
		if (temp[exp_data.i] == '$')
		{
			if (ft_handle_var_expansion(head, &exp_data, data))
				return (NULL);
			exp_data.i++;
			if (temp[exp_data.i] == '\0')
				break ;
			continue ;
		}
		ft_t_word_append_char(exp_data.word, temp[exp_data.i++]);
	}
	ft_terminate_word(exp_data.word, &exp_data.list);
	return (exp_data.list);
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
int	ft_t_token_var_expansion_check(t_node **head, t_data *data)
{
	t_token	*token;
	t_node	*list;

	token = (t_token *)(*head)->content;
	if (token->type == WORD && ft_strchr(token->string, '$'))
	{
		list = ft_init_var_expansion(head, data);
		if (list == NULL)
			return (1);
		ft_exchange_tokens(head, list);

	}
	return (0);
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
	int		len;
	int		i;
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

t_node	*wild_combine_2(char **sel_dir)
{
	t_node	*ret;
	t_node	*curr;
	int		i;

	curr = ft_calloc(1, sizeof(t_node));
	curr->content = ft_calloc(1, sizeof(t_token));
	if (sel_dir[0] == NULL)
		return (NULL);
	((t_token *)curr->content)->string = ft_strdup(sel_dir[0]);
	((t_token *)curr->content)->type = WORD;
	ret = curr;
	i = 1;
	while (sel_dir[i] != NULL)
	{
		curr->next = ft_calloc(1, sizeof(t_node));
		((t_node *)curr->next)->prev = curr;
		curr = curr->next;
		curr->content = ft_calloc(1, sizeof(t_token));
		((t_token *)curr->content)->string = ft_strdup(sel_dir[i]);
		((t_token *)curr->content)->type = WORD;
		i++;
	}
	return (ret);
}

int	ft_str_array_var_expansion(char ***str_arr, t_data *data)
{
	t_node	*head_token;

	if (*str_arr != NULL)
	{
		head_token = NULL;
		head_token = wild_combine_2(*str_arr);
		if (head_token == NULL)
			return (0);
		if (ft_t_token_variable_expansion(&head_token, data))
			return (1);
		if (ft_wildcard_expansion(&head_token))
			return (1);
		ft_exchange_str_arr(str_arr, head_token);
	}
	return (0);
}

//Heredoc variable expansion : Beginning
// no wildcard expansion in heredoc

char	*ft_heredoc_get_var_name_special_cases(char *str, int i)
{
	char	*var_name;

	var_name = NULL;
	if (str[i] == '?')
	{
		var_name = ft_calloc(2, sizeof(char));
		var_name[0] = '?';
		var_name[1] = '\0';
	}
	if (!ft_isalnum(str[i]) && str[i] != '_')
	{
		var_name = ft_calloc(2, sizeof(char));
		var_name[0] = '$';
		var_name[1] = '\0';
	}
	return (var_name);
}

char	*ft_heredoc_get_var_name(char *str, int i)
{
	int		i_start;
	char	*var_name;

	i_start = i;
	if (str[i] == '?' || (!ft_isalnum(str[i]) && str[i] != '_'))
		return (ft_heredoc_get_var_name_special_cases(str, i));
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

char	**ft_heredoc_get_variable_name(t_node *node, int i, t_data *data)
{
	char	**var;

	var = ft_calloc(3, sizeof(char *));
	if (var == NULL)
		return (NULL);
	var[0] = ft_heredoc_get_var_name(node->content, i);
	if (var[0] == NULL)
		return (NULL);
	ft_set_variable_name_and_value(&var, data);
	return (var);
}

int	ft_heredoc_handle_var_expansion(t_node *node, t_word *word, int *i, t_data *data)
{
	char	**var;
	int		j;

	var = ft_heredoc_get_variable_name(node, *i + 1, data);
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
	ft_free_char_array(var);
	return (0);
}

int	ft_heredoc_var_expansion_init(t_node **node, t_data *data)
{
	t_word	word;
	int		i;

	ft_t_word_init(&word);
	i = 0;
	while (((char *)(*node)->content)[i] != '\0')
	{
		if (((char *)(*node)->content)[i] == '$')
		{
			if (ft_heredoc_handle_var_expansion(*node, &word, &i, data))
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

void	ft_heredoc_var_expansion_check(t_node **node, t_data *data)
{
	t_node	*temp;

	temp = *node;
	while (*node != NULL)
	{
		if (ft_strchr((*node)->content, '$'))
		{
			ft_heredoc_var_expansion_init(node, data);
		}
		*node = (*node)->next;
	}
	ft_get_beginning_of_list(temp, node);
}

void	ft_t_io_heredoc_var_expansion(t_io *io, t_data *data)
{
	if (io != NULL)
	{
		if (io->quoted_status == VAR_UQUOTED)
		{
			ft_heredoc_var_expansion_check(&(io->heredoc_node), data);
		}
	}
}
//Heredoc variable expansion : End

int	ft_t_bin_var_expansion_check(t_node *head, t_data *data)
{
	t_bin	*tree;

	tree = (t_bin *)head->content;
	if (tree == NULL)
		return (0);
	else
	{
		ft_str_array_var_expansion(&(tree->command->arguments), data);
		if (tree->io != NULL)
		{
			ft_t_io_heredoc_var_expansion(tree->io, data);
			ft_str_array_var_expansion(&(tree->io->input), data);
			ft_str_array_var_expansion(&(tree->io->output), data);

		}
	}
	return (0);
}

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

int	ft_t_token_variable_expansion(t_node **head_token, t_data *data)
{
	t_node	*temp;
	int		ret;

	while (*head_token != NULL)
	{
		if (ft_t_token_var_expansion_check(head_token, data))
			return (1);
		temp = *head_token;
		*head_token = (*head_token)->next;
	}
	ret = ft_get_beginning_of_list(temp, head_token);
	return (ret);
}

int	ft_t_bin_variable_expansion(t_node *head, t_data *data)
{
	while (head != NULL && head->content != NULL)
	{
		if (ft_t_bin_var_expansion_check(head, data))
			return (1);
		head = head->next;
	}
	return (0);
}
