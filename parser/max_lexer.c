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
#include "../lib/libft/libft.h"

int	ft_append_token(t_word *word, t_node **head, int end);
int	ft_var_expansion(t_node **head);
int	ft_wildcard_expansion(t_node **head);

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

void	*ft_t_node_get_last(void *head)
{
	if (head != NULL)
	{
		while (((t_node *)head)->next != NULL)
			head = ((t_node *)head)->next;
	}
	return (head);
}

void	ft_t_node_add_back(t_node **head, t_node *node)
{
	t_node	*last;

	if (*head == NULL)
		*head = node;
	else
	{
		last = ft_t_node_get_last(*head);
		node->prev = last;
		last->next = node;
	}
}

void	ft_t_node_detach_and_free(t_node *to_detach)
{
	((t_node *)to_detach->prev)->next = to_detach->next;
	if ((t_node *)to_detach->next != NULL)
		((t_node *)to_detach->next)->prev = to_detach->prev;
	if (to_detach->content != NULL)
	{
		free(((t_token *)to_detach->content)->string);
		((t_token *)to_detach->content)->string = NULL;
		free(to_detach->content);
		to_detach->content = NULL;
	}
	to_detach->next = NULL;
	to_detach->prev = NULL;
	free(to_detach);
	to_detach = NULL;
}

void	ft_t_node_free(t_node *head)
{
	t_token	*token;
	t_node	*temp;

	temp = NULL;
	token = NULL;
	while (head != NULL)
	{
		token = (t_token *)head->content;
		free(token->string);
		token->string = NULL;
		if (token->heredoc != NULL)
		{
			while (token->heredoc != NULL)
			{
				free (token->heredoc->content);
				token->heredoc->content = NULL;
				temp = token->heredoc;
				token->heredoc = token->heredoc->next;
				free (temp);
				temp = NULL;
			}
		}
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
		word->chars = NULL;
		return (NULL);
	}
	str = malloc(sizeof(char) * word->write_head + 1);
	if (str == NULL)
	{
		free(word->chars);
		word->chars = NULL;
		return (NULL);
	}
	ft_memcpy(str, word->chars, word->write_head);
	str[word->write_head] = '\0';
	free (word->chars);
	word->chars = NULL;
	return (str);
}

void	ft_s_node_print_content(t_node *head)
{
	t_token	*token;
	t_node	*temp;

	while (head != NULL)
	{
		token = (t_token *)head->content;
		printf("[%d][%c] :%s:\n", token->type, token->quote_status, token->string);
		if (token->heredoc != NULL)
		{
			temp = token->heredoc;
			while (temp != NULL)
			{
				printf(":%s:\n", temp->content);
				temp = temp->next;
			}
		}
		head = head->next;
	}
}


void	ft_t_token_add_empty_string(t_word *word, t_node **head, int *i)
{
	ft_t_word_append_char(word, '\0');
	ft_append_token(word, head, 0);
	(*i) = (*i) + 2;
}

//changes quotation status
int	ft_change_status(char *input, int *i, t_word *word, t_node **head)
{
	int	ret;

	ret = 0;
	if (word->status == -1 && (input[*i] == '"' || input[*i] == '\''))
	{
		word->status = input[*i];
		if (input[*i + 1] == word->status && word->write_head == 0 &&
			ft_strlen(input) > (size_t)(*i + 1) && ft_strchr(" \t\n", input[*i + 2]))
			ft_t_token_add_empty_string(word, head, i);
		else
		{
			word->type = word->status;
			//word->status = word->status;
			(*i)++;
		}
		ret = 1;
	}
	else if (input[*i] == word->status)
	{
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
		if (word->chars[0] == '&' && word->chars[1] == '&') //&& word->write_head == 2)
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
	int		token_type;
	int		quote_status;

	quote_status = word->type;
	token_type = ft_set_type(word);
	str = ft_t_word_get_str(word);
	if (str == NULL)
		return (1);
	if (ft_append_token_helper(str, head, token_type, quote_status))
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

/* //handles dquoted variable appendage
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
} */

/* //handles unquoted variable appendage
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
} */

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
	/* if (!ft_isalpha(str[i]) && str[i] != '_')
		return (malloc()); // NULL */
	while (ft_isalnum(str[i]) || str[i] == '_')//(!ft_strchr(" \t\n\"'$|&()<>", str[i]) && str[i] != '\0')
	{
		/* if (!ft_isalnum(str[i]) && str[i] != '_')
			break ;//return (NULL); */
		i++;
	}
	var_name = ft_substr(str, i_start, i - i_start);
	printf("[%d]var_name = :%s:\n",__LINE__, var_name);
	return (var_name);
}

// is called if variable is found: checks for
// valid var_name, gets var_value, checks for wildcard character
// and appends chars to word and terminates word if necessary

//new: status of var is stored in char after $.
int	ft_append_variable(char *input, int *i, t_word *word, t_node **head)
{
	char	*var_name;
	//char	*var_value;
	t_node	*last_node;
	int		len;

	last_node = ft_t_node_get_last(*head);
	if (last_node != NULL && ((t_token *)last_node->content)->type == HERE_DOC)
	{
		ft_t_word_append_char(word, input[*i]);
		return (0);
	}
	ft_t_word_append_char(word, input[*i]);
	if (word->status == -1) //expandsion in executor!
		ft_t_word_append_char(word, VAR_UQUOTED);
	else if (word->status == '"')
		ft_t_word_append_char(word, VAR_DQUOTED);
	else if (word->status == '\'')
		ft_t_word_append_char(word, VAR_SQUOTED);
	var_name = ft_get_var_name(input, *i + 1);
	if (var_name == NULL)
		return (-1);
	len = ft_strlen(var_name);
	while (len > 0)
	{
		ft_t_word_append_char(word, input[++(*i)]);
		len--;
	}
	free(var_name);
	var_name = NULL;
	ft_t_word_append_char(word, VAR_END);
		/* var_value = getenv(var_name);
		if (var_value == NULL)
		{
			free(var_name);
			var_name = NULL;
			return (-1);
		} */
		/* *i += ft_strlen(var_name);
		free(var_name);
		var_name = NULL;
		if (ft_variable_dquoted(var_value, word) == -1)
			return (-1);
		if (ft_variable_unquoted(var_value, word, head) == -1)
			return (-1); */
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

	last_node = ft_t_node_get_last(*head);
	if (ft_is_type_redirection(last_node))
	{
		printf("ambigious redirection\n");
		return (-1);
	} */
	if (input[*i] == '*' && (word->status == '\'' || word->status == '"'))
	{
		/* if (ft_skip_chars(input, i, "*") == 1)
			(*i)--; */
		/* if (ft_t_word_append_char(word, '"') == -1)
			return (-1); */
		if (ft_t_word_append_char(word, '*') == -1)
			return (-1);
		if (ft_t_word_append_char(word, -1) == -1)
			return (-1);
		return (1);
	}
	else
	{
		if (ft_t_word_append_char(word, '*') == -1)
			return (-1);
		if (ft_t_word_append_char(word, -2) == -1)
			return (-1);
	}
	return (0);
}

//manages appendage of characters
void	ft_append(char *input, int *i, t_word *word, t_node **head)
{
	char	c;

	if (word->status == '\'' && input[*i] != '*' && input[*i] != '$')
	{
		ft_t_word_append_char(word, input[*i]);
		/* if (input[*i] == '$')
			ft_t_word_append_char(word, '\''); */
	}
	else if (input[*i] == '$')
	{
		if(ft_append_variable(input, i, word, head) == -1)
		{
			printf("invalid var_name\n");
			exit(EXIT_FAILURE);
		}
	}
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
	t_word	word;
	t_node	*head;

	head = NULL;
	ft_t_word_init(&word);
	i = 0;
	ft_skip_set(input, &i, " \t\n");
	while (input[i] != '\0')
	{
		if (ft_terminate_token(input, &i, &word, &head))
			continue ;
		if (ft_change_status(input, &i, &word, &head))
			continue ;
		ft_append(input, &i, &word, &head);
		i++;
	}
	if (word.write_head != 0)
		ft_append_token(&word, &head, 1);
	if (word.chars != NULL)
	{
		free(word.chars);
		word.chars = NULL;
	}
	if (word.status == '\'' || word.status == '"')
	{
		ft_t_node_free(head);
		printf("missing quote\n");
		exit(EXIT_FAILURE);
	}
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

// reads stdin, saves input in list until line is equal to delimiter string
// saves list of input in token member heredoc
int	ft_handle_heredoc_input(t_token *token, t_token *delimiter)
{
	char	*line;
	size_t	len;
	t_node	*node;
	t_node	*head;

	head = NULL;
	node = NULL;
	len = ft_strlen(delimiter->string) + 1;
	while (1)
	{
		line = readline(">");
		if (line == NULL || !ft_strncmp(delimiter->string, line, len))
			break ;
		else
		{
			node = ft_t_node_create(line);
			if (node == NULL)
				return (1);
			ft_t_node_add_back(&head, node);
		}
	}
	if (line != NULL)
	{
		free(line);
		line = NULL;
	}
	if (head != NULL)
		token->heredoc = head;
	return (0);
}

//searches list for token with type heredoc. If next list elements content is of type word
// the function ft_handle_heredoc_input is called. The quoted status of the "delimiter" token
// is copied to the here_doc token and the delimiter token is removed from the list.
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
			{
				ft_handle_heredoc_input(token, delimiter);
				token->quote_status = delimiter->quote_status;
				ft_t_node_detach_and_free(head->next);
			}
		}
		head = head->next;
	}
	return (0);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//								PARSER										  //

int	ft_is_type_control_op(t_node *head)
{
	int		ret;
	t_token	*token;

	ret = 0;
	token = (t_token *)head->content;
	if (token->type == AND || token->type == OR || token->type == PIPE)
		ret = 1;
	return (ret);
}

int	ft_is_type_operator(t_node *head)
{
	int		ret;

	ret = 0;
	if (ft_is_type_redirection(head) || ft_is_type_control_op(head))
		ret = 1;
	return (ret);
}

// if controll operator is last token: reads stdin for input, lexes and adds to list
int	ft_ask_for_cmd(t_node *head)
{
	char 	*input;
	t_node	*head_append;

	head_append = ft_t_node_get_last(head);
	if (((t_token *)head_append->content)->type == HERE_DOC)
		return (0);
	input = readline(">");
	if (input != NULL)
	{
		head_append = ft_lexer_v2(input);
		ft_t_node_add_back(&head, head_append);
		free(input);
		input = NULL;
		return (0);
	}
	return (1);

}

// checks for valid syntax (currently without parentheses)
int	ft_parser(t_node *head)
{
	int	start;
	t_token	*current;
	t_token *last;

	start = 1;
	last = NULL;
	while (head != NULL)
	{
		current = (t_token *)head->content;
		//first token in pipeline must be word of redirop or par
		if (start == 1 && ft_is_type_control_op(head))
			return (1);
		//after operator token (exept here_doc (delimiter was already removed))
		// there must be a token type word
		else  if (last != NULL && ft_is_type_operator(head->prev) &&
			current->type != WORD && last->type != HERE_DOC)
			return (1);
		// no operator as last token (PAR not handled right now!!!!!)
		else if (ft_is_type_operator(head) && head->next == NULL)
		{
			if (ft_ask_for_cmd(head))
				return (1);
		}
		start = 0;
		head = head->next;
		last = current;
	}
	return (0);
}

t_node	*ft_lexer(char *input)
{
	t_node	*head;
	int		ret;

	ret = 1;
	head = ft_lexer_v2(input);
	//ft_s_node_print_content(head);
	if (ft_operator_is_valid(head))
	{
		printf("invalid token\n");
		ft_t_node_free(head);
		return (NULL);
	}
	else
	{
		ft_heredoc(head);
		ret = 0;
		/* if (ft_parser(head))
		{
			printf("parser error\n");
			ft_t_node_free(head);
			return (NULL);
		} */
		if (ft_var_expansion(&head))
		{
			printf("invalid var_name\n");
			ft_t_node_free(head);
			return (NULL);
		}
		if (ft_wildcard_expansion(&head))
		{
			printf("Error while wc expansion\n");
			ft_t_node_free(head);
			return (NULL);
		}
		ft_s_node_print_content(head);
	}
	//ft_t_node_free(head);
	//system("leaks a.out");
	return (head);
}

/* int	main(int argc, char **argv)
{
	t_node	*head;
	int		ret;

	ret = 1;
	if (argc == 2)
	{
		head = ft_lexer_v2(argv);
		if (ft_operator_is_valid(head))
			printf("invalid token\n");
		else
		{
			ft_heredoc(head);
			ret = 0;
			if (ft_parser(head))
			{
				ret = 1;
				printf("parser error\n");
			}
			if (ft_var_expansion(&head))
			{
				printf("invalid var_name\n");
				return (NULL);
			}
			if (ft_wildcard_expansion(&head))
			{
				printf("Error while wc expansion\n");
				return (1);
			}
			ft_s_node_print_content(head);
		}
		return (head);
		ft_t_node_free(head);
		//system("leaks a.out");
	}
	return (NULL);
}
*/

//todos
/*
** single $ sign is not handled correct
** if t_node->prev is redirection operator: error if wc expandsion results in more than one t_token *
** build tree, handle parentheses
*/