/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 16:40:53 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/08 17:25:06 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
//recursive function, parses a list of simple commands; exit condition:
//no new command found
int					par_commands(t_data *data);

//utils:

//reallocates a list of s_simple_com
struct s_simple_com	**utils_realloc_simple_com_list(struct s_simple_com ***old,
						int old_num, int new_num);
#endif /*PARSER_H*/