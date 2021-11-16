/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minis.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 13:28:36 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/16 17:11:15 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIS_H
# define MINIS_H

# include "structs.h"
//mainfunction of parser-part -> see parser.h for details regarding the parser
int					par_main(t_data *data);
//free-functions
//frees a list of s_simple_com's
struct s_simple_com	**free_simple_com_list(struct s_simple_com ***tofree,
						int l);

void				input_readline(t_data *data);
void				input_interrupt(int sig);
void				input_attributes_clear(void);
void				input_attributes_add(void);
void				get_prompt(t_data *data);
t_data				*new_t_data(void);
//built in functions
// TODO: cd, echo, env, exit, export, pwd, unset
#endif