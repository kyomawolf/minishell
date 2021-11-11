/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 13:09:28 by mstrantz          #+#    #+#             */
/*   Updated: 2021/11/11 16:04:07 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// metacharacter: 					space, tab, newline, ‘|’, ‘&’, ‘(’, ‘)’, ‘<’, or ‘>’
// controll operator (token): 		newline, ‘||’, ‘&&’, ‘|’, ‘(’, or ‘)’
// redirection operator (token):	'<', '>', '<<' or '>>'
// word	(token): 					(no unquoted metacharacters)

#include <stdio.h>

//Count tokens part (characters between parenthesis and quotes counted as one token)

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

void	ft_skip_non_operator_metachars(char *input, int *i)
{
	while (ft_strchr(" \t\n", input[*i]) && input[*i] != '\0')
		(*i)++;
}

void	ft_skip_non_operator_chars(char *input, int *i, int *counter)
{
	int	temp;

	temp = *i;
	while (!ft_strchr(" \t\n|&()<>", input[*i]) && input[*i] != '\0')
		(*i)++;
	if (temp != *i)
		(*counter)++;
}

int	ft_skip_operator_chars(char *input, int *i, int *counter, char *chars)
{
	int		idx;
	int		temp;
	int		op_count;

	idx = 0;
	while (chars[idx] != '\0')
	{
		op_count = 0;
		temp = *i;
		while (input[*i] == chars[idx])
		{
			op_count++;
			(*i)++;
		}
		if (op_count > 2 || (op_count != 0 && input[*i] == '\0'))
			return (1);
		if (temp != *i)
			(*counter)++;
		idx++;
	}
	return (0);
}

int	ft_skip_quotes_wCounter(char *input, int *i, int *counter)
{
	int		temp;
	char	quote;

	if ((input[*i] == '\'' || input[*i] == '"') && input[*i] != '\0')
	{
		temp = *i;
		quote = input[(*i)++];
		while (input[*i] != quote && input[*i] != '\0')
			(*i)++;
		if (input[*i] == '\0' && input[(*i) - 1] != quote)
			return (1);
		if (temp != *i)
			(*counter)++;
	}
	return (0);
}

int	ft_skip_parentheses(char *input, int *i, int *counter)
{
	int		n_open_par;
	int		n_close_par;

	if (input[*i] == '(' && input[*i] != '\0')
	{
		n_open_par = 1;
		n_close_par = 0;
		(*i)++;
		while (input[*i] != ')' && n_close_par != n_open_par && input[*i] != '\0')
		{
			if (input[*i] == '(')
				n_open_par++;
			else if (input[*i] == ')')
				n_close_par++;
			(*i)++;
		}
		if (input[*i] == '\0' && n_open_par != n_close_par)
			return (1);
		(*counter) += n_open_par;
	}
	return (0);
}

int	ft_count_token(char *input, int *i)
{
	int	counter;

	counter = 0;
	while (input[*i] != '\0')
	{
		ft_skip_non_operator_metachars(input, i);
		ft_skip_non_operator_chars(input, i, &counter);
		if (ft_skip_operator_chars(input, i, &counter, "|&<>"))// what about newline? for here_doc?
			return (-1);
		if (ft_skip_quotes_wCounter(input, i, &counter))
			return (-1);
		if (ft_skip_parentheses(input, i, &counter))
			return (-1);
	}
	return (counter);
}

//
//	Command count: several commands must be separated by a control operator! #control_operator + 1 == #commands
//
//	Separation of command and its arguments: delimiter has to be one or more whitespace metacharacters
//	Redirection operators are directly connected to one command (?)


int	ft_skip_quotes(char *input, int *i)
{
	char	quote;

	quote = input[(*i)];
	(*i)++;
	while (input[(*i)] != quote && input[(*i)] != '\0')
		(*i)++;
	if (input[(*i)] == '\0')
		return (1);
	return (0);
}

// Count	parentheses layers

int	ft_count_hierarchy(char *input)
{
	int				i;
	unsigned int	c_open_par;
	unsigned int	c_closed_par;

	c_open_par = 0;
	c_closed_par = 0;
	i = 0;
	while (input[i] != '\0')
	{
		if ((input[i] == '\'' || input[i] == '"'))
		{
			if (ft_skip_quotes(input, &i))
				return (-1);
		}
		if (input[i] == '(')
			c_open_par++;
		else if (input[i] == ')')
			c_closed_par++;
		if (c_closed_par > c_open_par)
			return (-1);
		i++;
	}
	if (c_open_par != c_closed_par)
		return (-1);
	return (c_open_par);
}

int	ft_count_commands(char *input)
{
	int	c_cmds;
	int	i;
	int	c_operator;

	c_cmds = 0;
	i = 0;
	while (input[i] != '\0')
	{
		if (!ft_strchr("&|", input[i]))
			c_operator = 0;
		if (input[i] == '&' && input[i + 1] == '&')
		{
			c_operator++;
			c_cmds++;
		}
		else if (input[i] == '|' && input[i + 1] == '|')
		{
			c_operator++;
			c_cmds++;
		}
		i++;
	}
	return (0);
}

int	ft_parser(char * input)
{
	//int	n_token;
	//int	i;

	printf("Hierarchy %d\n", ft_count_hierarchy(input));
	//i = 0;
	/* n_token = ft_count_token(input, &i);
	if (n_token == -1)
		return (1);
	else
		printf("%d Tokens counted\n", n_token); */
	return (0);
}

int	main(int argc, char **argv)
{

	if (argc == 2)
	{
		if (ft_parser(argv[1]))
			return (1);
		return (0);
	}
	return (1);
}
