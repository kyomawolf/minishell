/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 16:40:53 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/01 15:21:56 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
//recursive function, parses a list of simple commands; exit condition:
//no new command found
int				check_input(t_node *head);
t_bin			*b_tree_init(t_node *node, int depth);
t_bin			*builder_main(t_node *head);
#endif /*PARSER_H*/