/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 16:40:53 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/06 17:10:08 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
//recursive function, parses a list of simple commands; exit condition:
//no new command found
int				check_input(t_node *head);
t_bin			*b_tree_init(t_node **node, int depth);
t_bin			*builder_main(t_node *head);
int				count_children(t_node *node);
t_bin			*add_com(t_node **ori_node, t_bin *parent);
t_e_op			add_last_operator(t_node *node);
#endif /*PARSER_H*/