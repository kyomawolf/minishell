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

int	ft_isalnum(int c);

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

	token = (struct s_token *)malloc(sizeof(struct s_token));
	if (token == NULL)
		return (NULL);
	token->string = content;
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
	struct s_node	*temp;

	while (head != NULL)
	{
		free(((struct s_token *)head->content)->string);
		head->content = NULL;
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
	while (head != NULL)
	{
		printf("[%d] %s\n", ((struct s_token *)head->content)->type, ((struct s_token *)head->content)->string);
		head = head->next;
	}
}

void	ft_s_node_expand_variables(struct s_node * head)
{
	while (head != NULL)
	{
		if (((struct s_token *)head->content)->type == WORD || ((struct s_token *)head->content)->type == DQUOTE)
		{
			if (ft_strchr(((struct s_token *)head->content)->string, '$'))
			{
				// search for variable in env
				//	expand variable & add tokens


			}
		}
		head = head->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	struct s_node	*head;

	if (argc == 2)
	{
		head = lexer(argv[1]);
		ft_s_node_print_content(head);
		//ft_s_node_expand_variables(head);
		//expand variables and wildcards
		//split words
		ft_s_node_free(head);
		//system("leaks test");
		return (0);
	}
	return (1);
}