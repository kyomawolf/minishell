/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unused_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 19:03:23 by mstrantz          #+#    #+#             */
/*   Updated: 2021/11/25 19:08:37 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/*
//breaks down cmdstrings and filenames into single words, unquotes, expands variables and wildcards
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
		//add word to list
		if ((token->string[i] == ' ' || token->string[i] == '\t' || token->string[i] == '\n') && status !=  '\'' && status != '"')
		{
			if (ft_t_token_add_word_helper(token, &word) == -1)
				return (-1);
			ft_t_word_init(&word);
			ft_skip_whitespace(token->string, &i);
			continue ;
		}
		//set status
		else if (status == -1 && (token->string[i] == '"' || token->string[i] == '\''))
		{
			status = token->string[i];
			i++;
			continue ;
		}
		//unset status
		else if (token->string[i] == status)
		{
			status = -1;
			i++;
			continue ;
		}
		//append char as literal (includes single quoted $)
		else if (status == '\'' || token->string[i] != '$')
		{
			if (token->string[i] == '*' && (status == '\'' || status == '"'))
			{
				ft_skip_set(token->string, &i, "*");
				ft_t_word_append_char(&word, '"');
				ft_t_word_append_char(&word, '*');
				ft_t_word_append_char(&word, '"');
				continue ;
			}
			else if (ft_t_word_append_char(&word, token->string[i]))
			{
				free(word.chars);
				return (-1);
			}
		}
		// found variable in dquoted or unquoted state
		else if (token->string[i] == '$')
		{
			if (ft_t_word_handle_variable(token, &word, status, &i) == -1)
			{
				free(word.chars);
				return (-1);
			}
		}
		i++;
	}
	if (ft_t_token_add_word_helper(token, &word) == -1)
		return (-1);
	return (1);
}
 */

//
// unused utils
//

/* int	ft_quotes_count(char *input)
{
	int		i;
	int		counter;

	counter = 0;
	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			ft_skip_quotes(input, &i);
			counter = counter + 2;
		}
		i++;
	}
	return (counter);
}

char	*memcpy_skip_quotes(char *dst, char *src)
{
	int		i;
	int		j;
	char	quote;

	if (!dst && !src)
		return (NULL);
	i = 0;
	j = 0;
	quote = -1;
	while (src[i] != '\0')
	{
		if ((src[i] == '\'' || src[i] == '"') && quote == -1)
		{
			quote = src[i++];
			continue ;
		}
		else if (src[i] == quote)
		{
			quote = -1;
			i++;
			continue ;
		}
		dst[j++] = src[i++];
	}
	dst[j] = '\0';
	return (dst);
}

char	*ft_unquote_string(char *str)
{
	char	*new_string;
	int		len;

	len = ft_strlen(str);
	len = len - ft_quotes_count(str);
	new_string = (char *)malloc(sizeof(char) * len + 1);
	memcpy_skip_quotes(new_string, str);
	free (str);
	return (new_string);
}

static int	ft_get_num_str(char *s, char *set)
{
	int		found;
	int		num_str;
	int		i;

	found = 0;
	num_str = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			ft_skip_quotes(s, &i);
		if (ft_strchr(set, s[i]))
			found = 0;
		else if (found == 0)
		{
			num_str++;
			found = 1;
		}
		i++;
	}
	return (num_str);
}

static char	*ft_get_substr(char *s, char* set, int *pos)
{
	int		i;
	char	*substr;

	while (ft_strchr(set, s[*pos]))
		(*pos)++;
	i = *pos;
	while (!ft_strchr(set, s[i]) && s[i] != '\0')
	{
		if (s[i] == '\'' || s[i] == '"')
			ft_skip_quotes(s, &i);
		i++;
	}
	substr = ft_substr(s, *pos, i - *pos);
	if (substr == NULL)
		return (NULL);
	*pos = i + 1;
	return (substr);
}

char	**ft_split_on_set_obey_quoting(char *s, char* set)
{
	int		num_str;
	char	**ret;
	int		i;
	char	*substr;
	int		pos;

	if (s == NULL)
		return (NULL);
	num_str = ft_get_num_str(s, set);
	ret = ft_calloc(sizeof(char *), num_str + 1);
	if (ret == NULL)
		return (NULL);
	i = 0;
	pos = 0;
	while (i < num_str)
	{
		substr = ft_get_substr(s, set, &pos);
		if (substr == NULL)
			return (NULL);
		ret[i] = substr;
		i++;
	}
	ret[num_str] = NULL;
	return (ret);
} */