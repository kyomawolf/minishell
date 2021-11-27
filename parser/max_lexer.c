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
#include <readline/readline.h>
#include "../include/structs.h"
#include "../include/libft.h"

void	*ft_t_token_create(void *content)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->string = content;
	token->args = NULL;
	token->cmd_arr = NULL;
	token->heredoc = NULL;
	token->type = -1;
	token->quote_status = -1;
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

	while (head != NULL)
	{
		token = (t_token *)head->content;
		free(token->string);
		if (token->heredoc != NULL)
		{
			while (token->heredoc != NULL)
			{
				free (token->heredoc->content);
				token->heredoc->content = NULL;
				token->heredoc = token->heredoc->next;
				free(token->heredoc->prev);
				token->heredoc->prev = NULL;
			}
			free(token->heredoc->prev);
			token->heredoc->prev = NULL;
		}
		token->string = NULL;
		free(token);
		token = NULL;
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

// initializes struct s_word
int	ft_t_word_init(t_word *word)
{
	word->type = -1;
	word->status = -1;
	word->write_head = 0;
	word->alloc = BUFFER_SIZE;
	word->chars = malloc(sizeof(char) * word->alloc);
	if (word->chars == NULL)
		return (1);
	return (0);
}

//appends character to struct s_word member chars
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

//creates a copy of the terminated string, frees struct s_word member chars
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

void	ft_s_node_print_content(t_node *head)
{
	t_token	*token;

	while (head != NULL)
	{
		token = (t_token *)head->content;
		printf("[%d][%c] :%s:\n", token->type, token->quote_status, token->string);
		head = head->next;
	}
}

//changes quotation status
int	ft_change_status(char *input, int *i, char *status, t_word *word)
{
	int	ret;

	ret = 0;
	if (*status == -1 && (input[*i] == '"' || input[*i] == '\''))
	{
		*status = input[*i];
		word->type = *status;
		word->status = *status;
		(*i)++;
		ret = 1;
	}
	else if (input[*i] == *status)
	{
		*status = -1;
		word->status = -1;
		(*i)++;
		ret = 1;
	}
	return (ret);
}

//defines type of the token (operator or word)
char	ft_set_type(t_word *word)
{
	char	ret;

	ret = WORD;
	if (word->type == -1)
	{
		if (word->chars[0] == '&' && word->chars[1] == '&' &&
			word->write_head == 2)
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

//creates t_token * and t_node * and adds t_node * to the back of the list
int	ft_append_token_helper(char *str, t_node **head, int type, int status)
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
	token->quote_status = status;
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

//gets the type of the token, does the appendage and if necessary inits
//a new t_word *
int	ft_append_token(t_word *word, t_node **head, int end)
{
	char	*str;
	int		type;
	int		status;

	status = word->type;
	type = ft_set_type(word);
	str = ft_t_word_get_str(word);
	if (str == NULL)
		return (1);
	if (ft_append_token_helper(str, head, type, status))
		return (1);
	if (end == 0)
		ft_t_word_init(word);
	return (0);
}

// terminates_token on unquoted metacharacter, inits appendage of token
int	ft_terminate_token(char *input, int *i, t_word *word, t_node **head)
{
	int		ret;

	ret = 0;
	if (word->status == -1 && word->write_head != 0)
	{
		if (ft_strchr(" \t\n", input[*i]))
			ret = 2;
		else if (ft_strchr("<>|&()", input[*i]))
			ret = 1;
		if (ret > 0)
			ft_append_token(word, head, 0);
		if (ret == 2)
			ft_skip_set(input, i, " \t\n");
	}
	else if (word->status == -1 && word->write_head == 0)
		ft_skip_set(input, i, " \t\n");
	return (ret);
}

//handles dquoted variable appendage
int	ft_variable_dquoted(char *var_value, t_word *word)
{
	int	j;

	if (word->status == '"')
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

//handles unquoted variable appendage
// var_value may be splitted in several words
int	ft_variable_unquoted(char *var_value, t_word *word, t_node **head)
{
	int	j;

	if (word->status == -1)
	{
		j = 0;
		while (var_value[j] != '\0')
		{
			if (ft_whitespaces(var_value[j]))
			{
				ft_skip_set(var_value, &j, " \t\n");
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

//checks if var_name is valid and returns string with var_name
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
	while (!ft_strchr(" \t\n\"'$|&()<>", str[i]) && str[i] != '\0')
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
int	ft_append_variable(char *input, int *i, t_word *word, t_node **head)
{
	char	*var_name;
	char	*var_value;
	t_node	*last_node;

	last_node = ft_s_node_get_last(*head);
	if (((t_token *)last_node->content)->type == HERE_DOC)
	{
		ft_t_word_append_char(word, input[*i]);
		return (0);
	}
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
	if (ft_variable_dquoted(var_value, word) == -1)
		return (-1);
	if (ft_variable_unquoted(var_value, word, head) == -1)
		return (-1);
	return (0);
}

// skips set of chars, gives information if characters were skipped
int	ft_skip_chars(char *input, int *pos, char *chars)
{
	int	ret;
	int	temp;

	ret = 0;
	temp = *pos;
	while (ft_strchr(chars, input[*pos]) && input[*pos] != '\0')
		(*pos)++;
	if (temp != *pos)
		ret = 1;
	return (ret);
}

//checks if token has type of a redirection
int	ft_is_type_redirection(t_node *node)
{
	t_token	*token;
	int	ret;

	ret = 0;
	token = (t_token *)node->content;
	if (token->type == HERE_DOC)
		ret = 1;
	else if (token->type == IRD)
		ret = 1;
	else if (token->type == ORD_APP)
		ret = 1;
	else if (token->type == ORD_TRC)
		ret = 1;
	return (ret);
}

//handles wildcard appandage
int	ft_append_wildcard(char *input, int *i, t_word *word)//, t_node **head)
{
/* 	t_node	*last_node;

	last_node = ft_s_node_get_last(*head);
	if (ft_is_type_redirection(last_node))
	{
		printf("ambigious redirection\n");
		return (-1);
	} */
	if (input[*i] == '*' && (word->status == '\'' || word->status == '"'))
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

//manages appendage of characters
void	ft_append(char *input, int *i, t_word *word, t_node **head)
{
	char	c;

	if (word->status == '\'' && input[*i] != '*')
		ft_t_word_append_char(word, input[*i]);
	else if (input[*i] == '$')
		ft_append_variable(input, i, word, head);
	else if (input[*i] == '*')
		ft_append_wildcard(input, i, word);									//, head);
	else if (word->status == -1 && (input[*i] == '(' || input[*i] == ')'))
	{
		ft_t_word_append_char(word, input[*i]);
		ft_terminate_token(input, i, word, head);
	}
	else if (word->status == -1 && ft_strchr("<>|&", input[*i]))
	{
		c = input[*i];
		while (input[*i] == c)
			ft_t_word_append_char(word, input[(*i)++]);
		(*i)--;
		ft_terminate_token(input, i, word, head);
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
	ft_skip_set(input, &i, " \t\n");
	while (input[i] != '\0')
	{
		if (ft_terminate_token(input, &i, &word, &head))
			continue ;
		if (ft_change_status(input, &i, &status, &word))
			continue ;
		ft_append(input, &i, &word, &head);
		i++;
	}
	if (word.write_head != 0)
		ft_append_token(&word, &head, 1);
	return (head);
}

// checks if operator has valid amount of characters
int	ft_operator_is_valid(t_node *head)
{
	t_token	*token;
	int		ret;

	ret = 0;
	while (head != NULL)
	{
		token = (t_token *)head->content;
		if (token->type != WORD && ft_strlen(token->string) > 2)
		{
			ret = 1;
			break ;
		}
		head = head->next;
	}
	return (ret);
}

int	ft_handle_heredoc_input(t_node *head, t_token *token, t_token *delimiter)
{
	char	*input;
	size_t	len;
	t_node	*node;
	t_node	*head2;

	(void)head;
	(void)token;
	head2 = NULL;
	node = NULL;
	len = ft_strlen(delimiter->string);
	input = "";
	while (ft_strncmp(delimiter->string, input, len))
	{
		input = readline(">");
		if (input != NULL && ft_strncmp(delimiter->string, input, len))
		{
			// expand variable if necessary
			node = ft_t_node_create(input);
			if (node == NULL)
				return (1);
			ft_t_node_add_back(&head2, node);
		}
	}
	while (head2 != NULL)
	{
		printf(":%s:\n", head2->content);
		head2 = head2->next;
	}
	return (0);
}

int	ft_heredoc(t_node *head)
{
	t_token	*token;
	t_token	*delimiter;

	while (head != NULL)
	{
		token = ((t_token *)head->content);
		if (token->type == HERE_DOC)
		{
			delimiter = ((t_token *)((t_node *)head->next)->content);
			if (delimiter->type != WORD)
				return (1);
			else
				ft_handle_heredoc_input(head, token, delimiter);
		}
		head = head->next;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_node	*head;
	int		ret;

	ret = 1;
	if (argc == 2)
	{
		head = ft_lexer_v2(argv[1]);
		if (ft_operator_is_valid(head))
			printf("invalid operator\n");
		else
		{
			ft_heredoc(head);
			ft_s_node_print_content(head);
			//system("leaks lexer");
			ret = 0;
		}
		ft_s_node_free(head);
	}
	return (ret);
}


//todos
/*
** wildcard expansion function + includes of t_nodes
** if t_node->prev is redirection operator: error if wc expandsion results in more than one t_token *
** here_doc
*/