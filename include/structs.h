/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 15:51:37 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/17 02:05:48 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H

# define STRUCTS_H

# define BUFFER_SIZE  1
# define VAR_UQUOTED -1
# define VAR_SQUOTED -2
# define VAR_DQUOTED -3
# define VAR_END     -4
# define WC_QUOTED   -1
# define WC_END      -2

# include <stdlib.h>

typedef enum e_op
{
	AND,
	OR,
	PIPE,
	OPAR,
	CPAR,
	HERE_DOC,	//				legacy 1
	IRD,		//Input ReDirect no sw
	ORD_APP,	//APP append 	leg 3
	ORD_TRC,	//truncate		leg 4
	QUOTE,
	DQUOTE,
	WORD
}	t_e_op;

typedef enum e_builtin
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	NONE
}	t_e_builtin;

typedef struct s_node
{
	void	*next;
	void	*prev;
	void	*content;
}	t_node;

typedef struct s_io
{
	t_node	*heredoc_node;
	int		quoted_status;
	int		i_count;
	char	**input;
	char	*infile;
	int		o_mode;
	char	**output;
}	t_io;

typedef struct s_simple_com
{
	int		command;
	int		number_arguments;
	char	**arguments;
	int		exit_status;
}	t_simple_com;

typedef struct s_bin
{
	struct s_bin	*parent;
	struct s_bin	**child;
	t_simple_com	*command;
	t_io			*io;
	int				depth;
	enum e_op		control_op;
	int				child_amount;
}	t_bin;

typedef struct s_word
{
	char			*chars;
	size_t			write_head;
	unsigned int	alloc;
	char			type;
	char			status;
}	t_word;

typedef struct s_token
{
	char			*string;
	struct s_node	*args;
	struct s_node	*heredoc;
	char			**cmd_arr;
	int				type;
	int				quote_status;
}	t_token;

typedef struct s_expand
{
	int		i;
	t_node	*list;
	t_word	*word;
}	t_expand;

typedef struct s_exec
{
	int		num_cmds;
	int		cmd_count;
	int		**pipes;
	int		here_doc;
}	t_exec;

typedef struct s_data
{
	char				*input;
	char				*prompt;
	char				*currdir;
	t_node				*envp;
	t_bin				*tree;
	t_node				*list;
}	t_data;

#endif /*STRUCTS_H*/