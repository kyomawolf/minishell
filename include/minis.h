/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minis.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 13:28:36 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/11 13:42:52 by mstrantz         ###   ########.fr       */
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
t_data				*new_t_data(char **envp);
//built in functions
// TODO: cd, echo, env, exit, export, pwd, unset
char				**ft_realloc_charpp(char ***old, size_t new_size);
t_node				*wild_main(char *string);
void				print_binary_tree(t_bin *prin, int dep);
void				free_t_node_list(t_node *head);
t_node				*ft_lexer(char *input);
t_node				*ft_lexer_v2(char *string);
t_bin				*builder_main(t_node *list);
void				free_char_array(char ***arr);

int					pipe_count_main(t_bin *tree, int i);

void				*ft_s_node_create(void *content);
void				ft_s_node_add_back(struct s_node **head, \
										struct s_node *node);
//temporay
void	traverse_tree(t_bin *tree, t_node **head);
void	traverse_tree_rec(t_bin *tree, t_node **head);
void	ft_t_node_free(t_node *head);
#endif