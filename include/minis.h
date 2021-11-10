/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minis.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 13:28:36 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/10 15:39:47 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIS_H
# define MINIS_H

struct s_simple_com
{
	int		number_arguments;
	char	**arguments;
	int		exit_status;
};

struct s_groups
{
	int	begin_com;
	int	end_com;
	int	group_id;
};

struct s_command
{
	struct s_groups		**group_list;
	int					number_commands;
	int					curr_command;
	struct s_simple_com	**command_list;
	char				*in_file;
	char				*out_file;
	char				*err_file;
};

typedef struct s_data
{
	struct s_command	*allcom;
	char				*input;
	char				*prompt;
	char				*currdir;
}	t_data;

//mainfunction of parser-part -> see parser.h for details regarding the parser
int					par_main(t_data *data);
//free-functions
//frees a list of s_simple_com's
struct s_simple_com	**free_simple_com_list(struct s_simple_com ***tofree,
						int l);

void				input_readline(t_data *data);
void				input_attributes_clear(void);
void				input_attributes_add(void);
#endif