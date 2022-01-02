/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 16:40:53 by jkasper           #+#    #+#             */
/*   Updated: 2022/01/02 16:09:36 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

int		check_input(t_node *head);
int		check_input_part2(t_node *head);
t_bin	*b_tree_init(t_node **node, int depth);
t_bin	*builder_main(t_node *head);
int		count_children(t_node *node);
t_bin	*add_com(t_node **ori_node, t_bin *parent);
t_e_op	add_last_operator(t_node *node);
t_bin	*bracket_check(t_node **ori_node, t_bin *parent);
t_node	*add_io(t_bin *tree, t_node *node);
t_node	*add_io_heredoc(t_bin *tree, t_node *node);
t_node	*add_words(t_simple_com *command, t_node *node);
int		count_words(t_node *node);
t_bin	*add_node(t_bin *par, t_node *node);
void	move_bracket(t_node **ori_node);
#endif /*PARSER_H*/