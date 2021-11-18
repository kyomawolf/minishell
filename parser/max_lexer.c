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

char	**ft_split_on_set_obey_quoting(char *s, char* set);
int	ft_isalnum(int c);

// STRUCTS

struct s_token
{
	char	*string;
	char	**cmd_arr;
	int		type;
	int		layer;
};

struct s_node
{
	void	*next;
	void	*prev;
	void	*content;
};

// LIBFT FUNCTIONS

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	else
		return (0);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}

char	*ft_strchr(const char *str, int c)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(str);
	while (i <= len)
	{
		if (str[i] == (unsigned char) c)
			return ((char *)&str[i]);
		i++;
	}
	return (NULL);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	if (!dst && !src)
		return (NULL);
	i = 0;
	while (i < n)
	{
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dst);
}

void	*ft_calloc(int nitems, int size)
{
	char	*ptr;
	int		size_in_bytes;
	int		i;

	size_in_bytes = nitems * size;
	ptr = (char *) malloc(size_in_bytes);
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (i < (size_in_bytes))
	{
		ptr[i] = 0;
		i++;
	}
	return ((void *)ptr);
}

char	*ft_substr(char *s, unsigned int start, int len)
{
	char	*substr;
	int	i;

	substr = (char *) malloc(len + 1);
	if (substr == NULL || s == NULL)
		return (NULL);
	if (start >= ft_strlen(s))
	{
		substr[0] = 0;
		return (substr);
	}
	i = 0;
	while (i < len && s[start])
	{
		substr[i] = s[start];
		i++;
		start++;
	}
	substr[i] = '\0';
	return (substr);
}

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
		else if (token[0] == '\'')
			type = QUOTE;
		else if (token[0] == '"')
			type = DQUOTE;
		else if (ft_isalnum(token[0]) || token[0] == '_')
			type = WORD;
	}
	return (type);
}

void	*ft_s_token_create(void *content)
{
	struct s_token	*token;
	char			**cmd_arr;

	token = (struct s_token *)malloc(sizeof(struct s_token));
	if (token == NULL)
		return (NULL);
	token->string = content;
	cmd_arr = ft_split_on_set_obey_quoting(token->string, " \t\n");
	if (cmd_arr == NULL)
		return (NULL);
	token->cmd_arr = cmd_arr;
	token->type = ft_s_token_get_type(content);
	token->layer = 0;
	return (token);
}

void	*ft_s_node_create(void *content)
{
	struct s_node	*node;

	node = (struct s_node *)malloc(sizeof(struct s_node));
	if (node == NULL)
		return (NULL);
	node->next = NULL;
	node->prev = NULL;
	node->content = content;
	return (node);
}

void	*ft_s_node_get_last(void *head)
{
	struct s_node	*current;

	current = head;
	while (current->next != NULL)
		current = current->next;
	return (current);
}

void	ft_s_node_add_back(struct s_node **head, struct s_node *node)
{
	struct s_node	*current;

	if (*head == NULL)
		*head = node;
	else
	{
		current = ft_s_node_get_last(*head);
		node->prev = current;
		current->next = node;
	}
}

void	ft_s_node_free(struct s_node *head)
{
	int				i;
	struct s_node	*temp;

	while (head != NULL)
	{
		i = 0;
		while (((struct s_token *)head->content)->cmd_arr[i] != NULL)
		{
			free (((struct s_token *)head->content)->cmd_arr[i]);
			((struct s_token *)head->content)->cmd_arr[i] = NULL;
			i++;
		}
		free (((struct s_token *)head->content)->cmd_arr);
		((struct s_token *)head->content)->cmd_arr = NULL;
		free(((struct s_token *)head->content)->string);
		((struct s_token *)head->content)->string = NULL;
		temp = head;
		head = head->next;
		free(temp);
		temp = NULL;
	}
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

int	ft_token_get_length(char *input)
{
	int		i;
	char	operator;

	i = 0;
	if (ft_strchr("|&()<>", input[i]))
	{
		operator = input[i];
		if (operator == '(' || operator == ')')
			i++;
		else
		{
			while (input[i] == operator)
				i++;
		}
	}
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

int	ft_quotes_count(char *input)
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
}

/////////////////////////////       added

//space after var_name and '=' sign is not allowed
//not assigned variable gives empty line or new prompt
//no spaces in var_name
//var_name has to start with alpha or '_'
//no special character in var_name

// must handle $?

char	*ft_var_name_is_valid(char *str, int i)
{
	int		i_start;
	char	*var_name;

	i_start = i;
	//check if var_name is ?
	if (str[i] == '?')
	{
		var_name = malloc(2);
		var_name[0] = '?';
		var_name[1] = '\0';
		return (var_name);
	}
	//check first char of var_name: must be alpha or _
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (NULL);
	while (str[i] != '\0' && str[i] != '"' && str[i] != '\'' && str[i] != '$' && str[i] != ' ')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (NULL);
		i++;
	}
	var_name = ft_substr(str, i_start, i - i_start);
	return (var_name);
}

void	ft_check_variables_expansion(char *str, int i)
{
	char	quote;
	char	*var;

	while (str[i] != '\0')
	{
		if (str[i] == '\'')
			ft_skip_quotes(str, &i);
		else if (str[i] == '$')
		{
			var = ft_var_name_is_valid(str, i + 1);
			if (var == NULL)
			{
				printf("invalid var_name\n");
				exit(EXIT_FAILURE); // free and error handling
			}
			printf("var_name: :%s:\n", var);
			free(var);
			var = NULL;
		}
		else if (str[i] == '"')
		{
			quote = str[i++];
			while (str[i] != quote) // && str[i] != '\0'
			{
				if (str[i] == '$' && quote == '"')
				{
					var = ft_var_name_is_valid(str, i + 1);
					if (var == NULL)
					{
						printf("invalid var_name\n");
						exit(EXIT_FAILURE); // free and error handling
					}
					printf("var_name: :%s:\n", var);
					free(var);
					var = NULL;
					// search for variable in env
					//	expand variable & split content if necessary & add tokens
				}
				i++;
			}
		}
		i++;
	}
}

/* {
			quote = str[i++];
			while (str[i] != quote) // && str[i] != '\0'
			{
				if (str[i] == '$' && quote == '"')
				{
					var = ft_var_name_is_valid(str, i + 1);
					if (var == NULL)
					{
						printf("invalid var_name\n");
						exit(EXIT_FAILURE); // free and error handling
					}
					printf("var_name: :%s:\n", var);
					free(var);
					var = NULL;
					// search for variable in env
					//	expand variable & split content if necessary & add tokens
				}
				i++;
			}
} */

void	ft_s_node_expand_variables(struct s_node *head)
{
	int		i;

	while (head != NULL)
	{
		i = 0;
		while (((struct s_token *)head->content)->cmd_arr[i] != NULL)
		{
			if (((struct s_token *)head->content)->type == WORD || ((struct s_token *)head->content)->type == DQUOTE)
			{
				printf("%s\n", ((struct s_token *)head->content)->cmd_arr[i]);
				if (ft_strchr(((struct s_token *)head->content)->cmd_arr[i], '$'))
				{
					ft_check_variables_expansion(((struct s_token *)head->content)->cmd_arr[i], 0);
					// search for variable in env
					//	expand variable & add tokens
				}
			}
			i++;
		}
		head = head->next;
	}
}

/////////////////////////////fin added

struct s_node	*lexer(char *input)
{
	int		i;
	char	*bytes;
	struct s_node	*node;
	struct s_node	*head;
	struct s_token	*token;

	head = NULL;
	i = 0;
	while (input[i] != '\0')
	{
		bytes = ft_get_next_token(input, &i);
		token = ft_s_token_create(bytes);
		node = ft_s_node_create(token);
		ft_s_node_add_back(&head, node);
	}
	return (head);
}

void	ft_s_node_print_content(struct s_node *head)
{
	char	*str;
	int		i;

	while (head != NULL)
	{

		//if (((struct s_token *)head->content)->type == QUOTE || ((struct s_token *)head->content)->type == DQUOTE)
		//str = ft_unquote_string(((struct s_token *)head->content)->string); // type assignment has to be fixed: detection of quotes is wrong!
		//((struct s_token *)head->content)->string = str;
		i = 0;
		while (((struct s_token *)head->content)->cmd_arr[i] != NULL)
		{
			printf("Original :%s:\n", ((struct s_token *)head->content)->cmd_arr[i]);
			str = ft_unquote_string(((struct s_token *)head->content)->cmd_arr[i]);
			((struct s_token *)head->content)->cmd_arr[i] = str;
			printf("Unquoted :%s:\n", ((struct s_token *)head->content)->cmd_arr[i]);
			i++;
		}
		//printf("[%d] :%s:\n", ((struct s_token *)head->content)->type, ((struct s_token *)head->content)->string);
		head = head->next;
	}
}

int	main(int argc, char **argv)//, char **envp)
{
	struct s_node	*head;

	if (argc == 2)
	{
		head = lexer(argv[1]);
		ft_s_node_expand_variables(head);
		//ft_s_node_print_content(head);
		//ft_unquote_string(head->content); in while loop
		//expand variables then wildcards //information if variable was singlequoted?
		//split words
		ft_s_node_free(head);
		//system("leaks test");
		return (0);
	}
	return (1);
}