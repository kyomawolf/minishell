/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 13:46:52 by mstrantz          #+#    #+#             */
/*   Updated: 2021/11/16 16:06:39 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "../include/structs.h"
#include "../include/libft.h"

#define AND 0
#define OR	1
#define PIPE 2
#define OPAR 3
#define CPAR 4
#define HERE_DOC 5
#define IRD 6
#define ORD_APP 7
#define ORD_TRC 8
#define QUOTE 9
#define DQUOTE 10
#define WORD 11

#define BUFFER_SIZE 1

//
// LEXER FUNCTIONS
//

// check if token->string is valid on a later step?
// what if quotes appear not at the beginning of the token->string?

int	ft_s_token_get_type(void *content)
{
	char	*token;
	int		type;
	int		len;

	type = -1;
	if (content != NULL)
	{
		token = content;
		len = ft_strlen(token);
		if (token[0] == '&' && token[1] == '&' && len == 2)
			type = AND;
		else if (token[0] == '|' && token[1] == '|' && len == 2)
			type = OR;
		else if (token[0] == '|' && token[1] == '\0')
			type = PIPE;
		else if (token[0] == '(')
			type = OPAR;
		else if (token[0] == ')')
			type = CPAR;
		else if (token[0] == '<' && token[1] == '<' && len == 2)
			type = HERE_DOC;
		else if (token[0] == '<' && token[1] == '\0')
			type = IRD;
		else if (token[0] == '>' && token[1] == '>' && len == 2)
			type = ORD_APP;
		else if (token[0] == '>' && token[1] == '\0')
			type = ORD_TRC;
		else if (ft_strchr(token, '\'')) // what if quotes appear on a later char?
			type = QUOTE;
		else if (token[0] == '"') // what if quotes appear on a later char?
			type = DQUOTE;
		else if (ft_isalnum(token[0]) || token[0] == '_') // what if quotes appear on a later char?
			type = WORD;
	}
	return (type);
}

void	*ft_t_token_create(void *content)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->string = content;
	token->args = NULL;
	token->cmd_arr = NULL;
	token->type = -1;//ft_s_token_get_type(content);
	return (token);
}

void	*ft_t_node_create(void *content)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (node == NULL)
		return (NULL);
	node->next = NULL;
	node->prev = NULL;
	node->content = content;
	return (node);
}

void	*ft_s_node_get_last(void *head)
{
	while (((t_node *)head)->next != NULL)
		head = ((t_node *)head)->next;
	return (head);
}

void	ft_t_node_add_back(t_node **head, t_node *node)
{
	t_node	*current;

	if (*head == NULL)
		*head = node;
	else
	{
		current = ft_s_node_get_last(*head);
		node->prev = current;
		current->next = node;
	}
}

void	ft_s_node_free(t_node *head)
{
	t_token	*token;
	t_node	*temp;
	//t_node	*temp_args;

	while (head != NULL)
	{
		token = (t_token *)head->content;
		free(token->string);
		token->string = NULL;
		/* while (token->args != NULL)
		{
			free(token->args->content);
			token->args->content = NULL;
			temp_args = token->args;
			token->args = token->args->next;
			free(temp_args);
			temp_args = NULL;
		} */
		free(token);
		temp = head;
		head = head->next;
		free(temp);
		temp = NULL;
	}
}
// skips set of characters specified in char *set
void	ft_skip_set(char *input, int *pos, char *set)
{
	while (ft_strchr(set, input[*pos]) && input[*pos] != '\0')
		(*pos)++;
}

void	ft_skip_whitespace(char *input, int *pos)
{
	while (ft_strchr(" \t\n", input[*pos]) && input[*pos] != '\0')
		(*pos)++;
}

void	ft_skip_quotes(char *input, int *pos)
{
	char	quote;

	quote = input[(*pos)++];
	while (input[*pos] != quote && input[*pos] != '\0')
		(*pos)++;
	if (input[*pos] == '\0')
		exit(EXIT_FAILURE); // FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE ERROR HANDLING ERROR HANDLING ERROR HANDLING
}

void	ft_operator_length(char *input, int *i)
{
	char	operator;

	operator = input[*i];
	if (operator == '(' || operator == ')')
		(*i)++;
	else
	{
		while (input[*i] == operator)
			(*i)++;
	}
}

// counts the length of the token. For allocation reasons.
int	ft_token_get_length(char *input)
{
	int		i;

	i = 0;
	if (ft_strchr("|&()<>", input[i]))
		ft_operator_length(input, &i);
	else
	{
		while (!ft_strchr("|&()<>", input[i]) && input[i] != '\0')
		{
			if ((input[i] == '\'' || input[i] == '"'))
				ft_skip_quotes(input, &i);
			i++;
		}
		while (ft_strchr(" \t\n", input[i - 1]))
			i--;
	}
	return (i);
}

// interates through input string and returns the next token string.
// divides input string in wordstrings (cmd + flags + arguments or filename)
// and (control- or redirection-) operators
char	*ft_get_next_token(char *input, int *i)
{
	int	len;
	char	*bytes;

	ft_skip_whitespace(input, i);
	len = ft_token_get_length(input + (*i));
	bytes = (char *)malloc(sizeof(char) * len + 1);
	if (bytes == NULL)
		return (NULL);
	ft_memcpy(bytes, input + (*i), len);
	bytes[len] = '\0';
	(*i) += len;
	return (bytes);
}

/*
** cmd_string_handling
*/

//space after var_name and '=' sign is not allowed
//not assigned variable gives empty line or new prompt
//no spaces in var_name
//var_name has to start with alpha or '_'
//no special character in var_name

// must handle $?

// initializes struct s_word word
int	ft_t_word_init(t_word *word)
{
	word->type = -1;
	word->write_head = 0;
	word->alloc = BUFFER_SIZE;
	word->chars = malloc(sizeof(char) * word->alloc);
	if (word->chars == NULL)
		return (1);
	return (0);
}

//appends character to struct s_word chars buffer
int	ft_t_word_append_char(t_word *word, char c)
{
	unsigned int	new_alloc;
	char			*new_chars;

	if (word->write_head >= word->alloc)
	{
		new_alloc = word->write_head + word->alloc;
		new_chars = malloc(sizeof(char) * new_alloc);
		if (new_chars == NULL)
			return (1);
		ft_memcpy(new_chars, word->chars, word->write_head);
		free (word->chars);
		word->chars = NULL;
		word->chars = new_chars;
		word->alloc = new_alloc;
	}
	word->chars[word->write_head] = c;
	word->write_head++;
	return (0);
}

//creates a copy of the terminated word, frees struct s_word word->chars buffer
char	*ft_t_word_get_str(t_word *word)
{
	char	*str;

	if (word->write_head <= 0)
	{
		free(word->chars);
		return (NULL);
	}
	str = malloc(sizeof(char) * word->write_head + 1);
	if (str == NULL)
	{
		free(word->chars);
		return (NULL);
	}
	ft_memcpy(str, word->chars, word->write_head);
	str[word->write_head] = '\0';
	free (word->chars);
	return (str);
}

//creates copy of terminated word, saves it in a new node and adds node to list
int	ft_t_token_add_word_helper(t_token *token, t_word *word)
{
	int		ret;
	char	*str;
	struct s_node	*node;

	ret = 0;
	str = ft_t_word_get_str(word);
	if (str == NULL)
		ret = -1;
	node = ft_t_node_create(str);
	if (node == NULL)
	{
		free (str);
		str = NULL;
		ret = -1;
	}
	ft_t_node_add_back(&(token->args), node);
	return (ret);
}
// checks if var_name is valid. Returns var_name.
char	*ft_var_name_is_valid(char *str, int i)
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
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (NULL);
	while (str[i] != '\0' && str[i] != '"' && str[i] != '\'' && str[i] != '$' &&
		str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (NULL);
		i++;
	}
	var_name = ft_substr(str, i_start, i - i_start);
	return (var_name);
}

//dquoted $ -> var_value == one word
int ft_t_word_variable_dquoted(char *var_value, char status, t_word *word)
{
	int	j;

	if (status == '"')
	{
		j = 0;
		while (var_value[j] != '\0')
		{
			if (var_value[j] == '*')
			{
				ft_skip_set(var_value, &j, "*");
				ft_t_word_append_char(word, '"');
				ft_t_word_append_char(word, '*');
				ft_t_word_append_char(word, '"');
				continue ;
			}
			else if (ft_t_word_append_char(word, var_value[j]))
				return (-1);
			j++;
		}
	}
	return (0);
}

//unquoted $ -> var_value may contain several words
int	ft_t_word_variable_unquoted(t_token *token, char *var_value, char status,
		t_word *word)
{
	int	j;

	if (status == -1)
	{
		j = 0;
		while (var_value[j] != '\0')
		{
			if (ft_whitespaces(var_value[j]))
			{
				ft_skip_whitespace(var_value, &j);
				if (ft_t_token_add_word_helper(token, word) == -1)
					return (-1);
				ft_t_word_init(word);
				continue ;
			}
			else if (ft_t_word_append_char(word, var_value[j]))
				return (-1);
			j++;
		}
	}
	return (0);
}

// is called if variable is found: checks for
// valid var_name, gets var_value, checks for wildcard character
// and appends chars to word and terminates word if necessary
int	ft_t_word_handle_variable(t_token *token, t_word *word, char status, int *i)
{
	char	*var_name;
	char	*var_value;

	var_name = ft_var_name_is_valid(token->string, *i + 1);
	if (var_name == NULL)
		return (-1);
	var_value = getenv(var_name);
	if (var_value == NULL)
	{
		free(var_name);
		return (-1);
	}
	*i += ft_strlen(var_name);
	free(var_name);
	if (ft_t_word_variable_dquoted(var_value, status, word) == -1)
		return (-1);
	if (ft_t_word_variable_unquoted(token, var_value, status, word) == -1)
		return (-1);
	return (0);
}

int	ft_t_token_add_word(t_token *token, char status, t_word *word, int *i)
{
	if ((token->string[*i] == ' ' || token->string[*i] == '\t' ||
		token->string[*i] == '\n') && status == -1)
	{
		if (ft_t_token_add_word_helper(token, word) == -1)
			return (-1);
		ft_t_word_init(word);
		ft_skip_whitespace(token->string, i);
		return (1);
	}
	return (0);
}
//changes status of quotation
int	ft_cmdstring_change_status(t_token *token, char *status, int *i)
{
	int	ret;

	ret = 0;
	if (*status == -1 && (token->string[*i] == '"' ||
		token->string[*i] == '\''))
	{
		*status = token->string[*i];
		(*i)++;
		ret = 1;
	}
	else if (token->string[*i] == *status)
	{
		*status = -1;
		(*i)++;
		ret = 1;
	}
	return (ret);
}

int	ft_t_word_append_wildcard(t_token *token, char status, int *i, t_word *word)
{
	if (token->string[*i] == '*' && (status == '\'' || status == '"'))
	{
		ft_skip_set(token->string, i, "*");
		if (ft_t_word_append_char(word, '"') == -1)
			return (-1);
		if (ft_t_word_append_char(word, '*') == -1)
			return (-1);
		if (ft_t_word_append_char(word, '"') == -1)
			return (-1);
		return (1);
	}
	return (0);
}

// manages wildcard, char and variable appendage
int	ft_cmdstring_append_char(t_token *token, char status, int *i, t_word *word)
{
	if (status == '\'' || token->string[*i] != '$')
	{
		if (ft_t_word_append_wildcard(token , status, i, word))
			return (1);
		else if (ft_t_word_append_char(word, token->string[*i]))
		{
			free(word->chars);
			return (-1);
		}
	}
	else if (token->string[*i] == '$')
	{
		if (ft_t_word_handle_variable(token, word, status, i) == -1)
		{
			free(word->chars);
			return (-1);
		}
	}
	return (0);
}

// breaks down cmdstrings and filenames into:
// single words, unquotes, expands variables and wildcards
int	ft_handle_cmdstring(t_token *token)
{
	int				i;
	char			status;
	t_word			word;
	int				ret;

	status = -1;
	ft_t_word_init(&word);
	i = 0;
	while (token->string[i] != '\0')
	{
		if (ft_t_token_add_word(token, status, &word, &i))
			continue ; // error not handled
		else if (ft_cmdstring_change_status(token, &status, &i))
			continue ; // error not handled
		ret = ft_cmdstring_append_char(token, status, &i, &word);
		if (ret == 1)
			continue ; // error not handled
		else if (ret == -1)
			return (ret);
		i++;
	}
	if (ft_t_token_add_word_helper(token, &word) == -1)
		return (-1);
	return (1);
}

// checks nodes->token for type and calls function to split string
// (cmd plus flags and arguments OR filenames) into words and puts them in a
// argument list
// prints content of argument list
void	ft_handle_nodes(t_node *head)
{
	t_token	*token;
	t_node	*node;

	while (head != NULL)
	{
		token = (t_token *)head->content;
		if (token->type == WORD || token->type == DQUOTE ||
			token->type == QUOTE)
			ft_handle_cmdstring(token);
		node = token->args;
		while (node != NULL)
		{
			printf("word :%s:\n", node->content);
			node = node->next;
		}
		head = head->next;
	}
}

// lexer: breaks input string from readline into:
// cmd_strings, filenames, control operator or redirection operators
t_node	*lexer(char *input)
{
	int		i;
	char	*bytes;
	t_node	*node;
	t_node	*head;
	t_token	*token;

	head = NULL;
	i = 0;
	while (input[i] != '\0')
	{
		bytes = ft_get_next_token(input, &i);
		if (bytes == NULL)
			return (NULL);

		token = ft_t_token_create(bytes);
		if (token == NULL)
			return (NULL);
		node = ft_t_node_create(token);
		if (node == NULL)
			return (NULL);
		ft_t_node_add_back(&head, node);
	}
	return (head);
}

void	ft_s_node_print_content(t_node *head)
{
	t_token	*token;

	while (head != NULL)
	{
		token = (t_token *)head->content;
		printf("[%d] :%s:\n", token->type, token->string);
		head = head->next;
	}
}

int	ft_change_status(char *input, int *i, char *status, t_word *word)
{
	int	ret;

	ret = 0;
	if (*status == -1 && (input[*i] == '"' || input[*i] == '\''))
	{
		*status = input[*i];
		word->type = *status;
		(*i)++;
		ret = 1;
	}
	else if (input[*i] == *status)
	{
		*status = -1;
		(*i)++;
		ret = 1;
	}
	return (ret);
}

char	ft_set_type(t_word *word)
{
	char	ret;

	ret = WORD;
	if (word->type == -1)
	{
		if (word->chars[0] == '&' && word->chars[1] == '&')
			ret = AND;
		else if (word->chars[0] == '|' && word->write_head == 1)
			ret = PIPE;
		else if (word->chars[0] == '|' && word->chars[1] == '|')
			ret = OR;
		else if (word->chars[0] == '(' && word->write_head == 1)
			ret = OPAR;
		else if (word->chars[0] == ')' && word->write_head == 1)
			ret = CPAR;
		else if (word->chars[0] == '<' && word->write_head == 1)
			ret = IRD;
		else if (word->chars[0] == '<' && word->chars[1] == '<')
			ret = HERE_DOC;
		else if (word->chars[0] == '>' && word->write_head == 1)
			ret = ORD_TRC;
		else if (word->chars[0] == '>' && word->chars[1] == '>')
			ret = ORD_APP;
	}
	return (ret);
}

int	ft_append_token_helper(char *str, t_node **head, int type)
{
	t_token	*token;
	t_node	*node;

	token = ft_t_token_create(str);
	if (token == NULL)
	{
		free (str);
		str = NULL;
		return (1);
	}
	token->type = type;
	node = ft_t_node_create(token);
	if (node == NULL)
	{
		free(str);
		str = NULL;
		free(token);
		token = NULL;
		return (1);
	}
	ft_t_node_add_back(head, node);
	return (0);
}

int	ft_append_token(t_word *word, t_node **head, int end)
{
	char	*str;
	int		type;

	type = ft_set_type(word);
	str = ft_t_word_get_str(word);
	if (str == NULL)
		return (1);
	if (ft_append_token_helper(str, head, type))
		return (1);
	if (end == 0)
		ft_t_word_init(word);
	return (0);
}

int	ft_terminate_token(char *input, int *i, char *status, t_word *word, t_node **head)
{
	int		ret;

	ret = 0;
	if (*status == -1 && word->write_head != 0)
	{
		if (ft_strchr(" \t\n", input[*i]))
			ret = 2;
		else if (ft_strchr("<>|&()", input[*i]))
			ret = 1;
		if (ret > 0)
			ft_append_token(word, head, 0);
		if (ret == 2)
			ft_skip_whitespace(input, i);
	}
	else if (*status == -1 && word->write_head == 0)
		ft_skip_whitespace(input, i);
	return (ret);
}

//dquoted $ -> var_value == one word
int ft_variable_dquoted(char *var_value, char status, t_word *word)
{
	int	j;

	if (status == '"')
	{
		j = 0;
		while (var_value[j] != '\0')
		{
			if (var_value[j] == '*')
			{
				ft_skip_set(var_value, &j, "*");
				ft_t_word_append_char(word, '"');
				ft_t_word_append_char(word, '*');
				ft_t_word_append_char(word, '"');
				continue ;
			}
			else if (ft_t_word_append_char(word, var_value[j]))
				return (-1);
			j++;
		}
	}
	return (0);
}

//unquoted $ -> var_value may contain several words
int	ft_variable_unquoted(char *var_value, char status, t_word *word, t_node **head)
{
	int	j;

	if (status == -1)
	{
		j = 0;
		while (var_value[j] != '\0')
		{
			if (ft_whitespaces(var_value[j]))
			{
				ft_skip_whitespace(var_value, &j);
				if (ft_append_token(word, head, 0))
					return (-1);
				ft_t_word_init(word);
				continue ;
			}
			else if (ft_t_word_append_char(word, var_value[j]))
				return (-1);
			j++;
		}
	}
	return (0);
}

char	*ft_get_var_name(char *str, int i)
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
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (NULL);
	while (!ft_strchr(" \t\n\"'$|&()<>", str[i]) &&str[i] != '\0')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (NULL);
		i++;
	}
	var_name = ft_substr(str, i_start, i - i_start);
	return (var_name);
}

// is called if variable is found: checks for
// valid var_name, gets var_value, checks for wildcard character
// and appends chars to word and terminates word if necessary
int	ft_append_variable(char *input, int *i, char *status, t_word *word, t_node **head)
{
	char	*var_name;
	char	*var_value;

	var_name = ft_get_var_name(input, *i + 1);
	if (var_name == NULL)
		return (-1);
	var_value = getenv(var_name);
	if (var_value == NULL)
	{
		free(var_name);
		return (-1);
	}
	*i += ft_strlen(var_name);
	free(var_name);
	if (ft_variable_dquoted(var_value, *status, word) == -1)
		return (-1);
	if (ft_variable_unquoted(var_value, *status, word, head) == -1)
		return (-1);
	return (0);
}

int	ft_skip_chars(char *input, int *pos, char *chars)
{
	int ret;
	int	temp;

	ret = 0;
	temp = *pos;
	while (ft_strchr(chars, input[*pos]) && input[*pos] != '\0')
		(*pos)++;
	if (temp != *pos)
		ret = 1;
	return (ret);
}

int	ft_append_wildcard(char *input, int *i, char status, t_word *word)
{
	if (input[*i] == '*' && (status == '\'' || status == '"'))
	{
		if (ft_skip_chars(input, i, "*") == 1)
			(*i)--;
		if (ft_t_word_append_char(word, '"') == -1)
			return (-1);
		if (ft_t_word_append_char(word, '*') == -1)
			return (-1);
		if (ft_t_word_append_char(word, '"') == -1)
			return (-1);
		return (1);
	}
	else
	{
		if (ft_t_word_append_char(word, '*') == -1)
			return (-1);
	}
	return (0);
}

void	ft_append(char *input, int *i, char *status, t_word *word, t_node **head)
{
	char	c;

	if (*status == '\'' && input[*i] != '*')
		ft_t_word_append_char(word, input[*i]);
	else if (input[*i] == '$')
		ft_append_variable(input, i, status, word, head);
	else if (input[*i] == '*')
		ft_append_wildcard(input, i, *status, word);
	else if (*status == -1 && (input[*i] == '(' || input[*i] == ')'))
	{
		ft_t_word_append_char(word, input[*i]);
		ft_terminate_token(input, i, status, word, head);
	}
	else if (*status == -1 && ft_strchr("<>|&", input[*i]))
	{
		c = input[*i];
		while (input[*i] == c)
			ft_t_word_append_char(word, input[(*i)++]);
		(*i)--;
		ft_terminate_token(input, i, status, word, head);
	}
	else
		ft_t_word_append_char(word, input[*i]);
}

t_node	*ft_lexer_v2(char *input)
{
	int		i;
	char	status;
	t_word	word;
	t_node	*head;

	ft_t_word_init(&word);
	status = -1;
	i = 0;
	ft_skip_whitespace(input, &i);
	while (input[i] != '\0')
	{
		if (ft_terminate_token(input, &i, &status, &word, &head))
			continue ;
		if (ft_change_status(input, &i, &status, &word))
			continue ;
		ft_append(input, &i, &status, &word, &head);
		i++;
	}
	ft_append_token(&word, &head, 1);
	return (head);
}

int	main(int argc, char **argv)
{
	t_node	*head;

	if (argc == 2)
	{
		//head = lexer(argv[1]);
		head = ft_lexer_v2(argv[1]);
		//ft_handle_nodes(head);
		ft_s_node_print_content(head);
		//create cmd_arr (char **)
		ft_s_node_free(head);
		system("leaks lexer");
		return (0);
	}
	return (1);
}
