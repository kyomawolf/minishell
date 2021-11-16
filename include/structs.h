/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 15:51:37 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/16 17:08:34 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H

# define STRUCTS_H

enum e_op
{
	NONE,
	PIPE,
	AND,
	OR
};

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
}	t_data;

typedef struct s_bin
{
	struct s_bin	*parent;
	struct s_bin	**child;
	t_simple_com	*command;
	enum e_op		control_op;
	int				child_amount;
}	t_bin;

#endif /*STRUCTS_H*/