/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 16:40:53 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/11 16:49:12 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
//recursive function, parses a list of simple commands; exit condition:
//no new command found
int				par_commands(t_data *data);

//utils:

//reallocates a list of s_simple_com
t_simple_com	**utils_realloc_simple_com_list(struct s_simple_com ***old,
					int old_num, int new_num);
int				brackets_next(t_data *data, int count, int delimiter, int mode);
int				brackets_correct(t_data *data, int count, int delimiter);
int				brackets_amount(t_data *data, int count, int delimiter);
#endif /*PARSER_H*/