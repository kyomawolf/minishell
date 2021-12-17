/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minis.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 13:28:36 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/17 02:17:33 by mstrantz         ###   ########.fr       */
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
void				free_main(t_data *data);
void				free_tree(t_bin *tree);
void				free_io(t_io *io);
void				free_simplecommand(t_simple_com *command);

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
t_node				*ft_get_token_list(char *input);
t_bin				*builder_main(t_node *list);
void				free_char_array(char ***arr);
void				free_t_node_content_list(t_node *head);

char				*mini_getenv(t_data *data, char *var_name);

char				**list_to_array(t_node *node);

int					pipe_count_main(t_bin *tree, int i);

void				*ft_s_node_create(void *content);
void				ft_s_node_add_back(struct s_node **head, \
										struct s_node *node);
int					ft_s_node_iter(struct s_node *head);
//temporay
void				traverse_tree(t_bin *tree, t_node **head);
void				traverse_tree_rec(t_bin *tree, t_node **head);
void				ft_t_node_free(t_node *head);
void				ft_t_node_free_heredoc(t_token *token, t_node *temp);
int					ft_t_bin_variable_expansion(t_node *head, t_data *data);
t_node				*wild_combine(char **sel_dir);
int					ft_wildcard_expansion(t_node **head);
int					ft_get_beginning_of_list(t_node *temp, t_node **head);
void				executor(t_node *head, t_data *data);
int					ft_execute(t_node *head, t_data *data);
void				ft_t_exec_init(t_exec *exec_data, t_node *head);
int					ft_open_pipes(t_exec *exec_data);
void				ft_parent_close_used_pipes(t_exec *exec_data);
int					ft_parent_waitpid(t_exec *exec_data, pid_t *pid);
void				ft_exec_here_doc(t_exec *exec_data, t_node *head);
void				builtin_check(char **cmd_arr, t_data *data);

//wildcard
char				**wild_pattern_match(char **all_dir, char **matcher);
char				**wild_get_seperator(char *string);
char				**ft_realloc_charpp(char ***old, size_t new_size);
void				free_char_array2(char ***arr);
#endif