/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 15:51:37 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/27 00:48:37 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H

# define STRUCTS_H

#define BUFFER_SIZE 1

typedef enum e_op
{
	AND,
	OR,
	PIPE,
	OPAR,
	CPAR,
	HERE_DOC,
	IRD,		//Input ReDirect no sw
	ORD_APP,	//APP append
	ORD_TRC,	//truncate
	QUOTE,
	DQUOTE,
	WORD
}	t_e_op;

typedef struct s_io
{
	int		i_mode;
	char	**input;
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

struct s_groups
{
	int	begin_com;
	int	end_com;
	int	group_id;
};

struct s_command
{
	int				number_groups;
	int				curr_group;
	struct s_groups	**group_list;
	int				number_commands;
	int				curr_command;
	t_simple_com	**command_list;
	char			*in_file;
	char			*out_file;
	char			*err_file;
};

typedef struct s_data
{
	struct s_command	*allcom;
	char				*input;
	char				*prompt;
	char				*currdir;
	char				**envp;
}	t_data;

typedef struct s_bin
{
	struct s_bin	*parent;
	struct s_bin	**child;
	t_simple_com	*command;
	t_io			*command_io;
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

typedef struct s_node
{
	void	*next;
	void	*prev;
	void	*content;
}	t_node;

#endif /*STRUCTS_H*/