/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 18:39:41 by mstrantz          #+#    #+#             */
/*   Updated: 2022/01/08 21:54:36 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "struct.h"
//exec_utils.c
char		ft_tolower2(unsigned int i, char c);
void		wait_child(int sig);
void		ft_t_expand_init(t_expand *exp_data);
void		ft_signals(void);
void		child_signal(int signal);

//exec_builtin.c
void		builtin_check_child(char **cmd_arr, t_data *data, t_node *head, \
								t_exec *exec_data);
t_e_builtin	builtin_check(t_node *head);
int			ft_builtin_exec_init(t_e_builtin builtin, t_node *head, \
				t_data *data, t_exec *exec_data);

//exec_child.c
void		ft_child_process(t_node *pl, t_data *data, t_exec *exec_data, \
								t_node **ori_head);

//exec_io1.c
int			ft_adjust_pipes(t_exec *exec_data, t_node *head);
int			ft_check_last_redir(t_node_io *node_io, int type);
int			ft_set_input_redirection(t_io *io, t_node_io *node_io);
int			ft_set_output_redirection(t_io *io, t_node_io *node_io);
int			ft_set_outout_redirection_helper(t_node *head);

//exec_io2.c
int			redirection_error(char *filename, int fd);
void		ft_t_exec_heredoc_check(t_node *head, t_exec *exec_data);
int			ft_exec_here_doc(t_exec *exec_data, t_node *head);
void		ft_exec_here_doc_helper(t_node *head);

//exec_io_adjust_pipes.c
void		adjust_pipes_helper(t_exec *exec_data, int i_flag, int o_flag);
int			adjust_redirections(t_node *head, int *inp_flag, int *outp_flag);

//exec_executor.c
int			ft_execute(t_node *pl, t_data *data, t_node **ori_head);

//exec_expand_wildcard.c
int			ft_wildcard_expansion(t_node **head);

//exec_expand1.c
int			ft_t_bin_variable_expansion(t_node *head, t_data *data);
int			ft_t_token_variable_expansion(t_node **head_token, t_data *data);

//exec_expand2.c
int			ft_str_array_var_expansion(char ***str_arr, t_data *data);

//exec_expand3.c
int			ft_exchange_tokens_var(t_node ***head, t_expand *exp_data, \
									t_node **temp_ori);
void		ft_exchange_tokens_helper(t_node **head, t_node *list);
void		ft_exchange_tokens_free_exp_data_word(t_expand *exp_data);
t_node		*ft_init_var_expansion(t_node **head, t_data *data, \
									t_expand *exp_data);
void		ft_t_expand_init(t_expand *exp_data);

//exec_expand4.c
int			ft_handle_var_expansion(t_node **head, t_expand *exp_data, \
									 t_data *data);
void		ft_set_variable_name_and_value(char ***var, t_data *data);
void		ft_get_variable_name_guard(char **var);
int			ft_init_handle_var_expansion(t_node **head, t_data *data, \
									t_expand *exp_data, char *temp);
//exec_expand5.c
void		ft_handle_dquoted_var(t_word *word, char **var);
void		ft_handle_unquoted_var(t_expand *exp_data, char **var);
//void		ft_handle_unquoted_var(t_word *word, t_node **list, char **var);
void		ft_handle_squoted_var(t_node *head, t_word *word, int *i);
char		*ft_expander_get_var_name(char *str, int i);
char		*ft_get_var_name_special_cases(char *str, int i);

//exec_expand_heredoc1.c
void		ft_t_io_heredoc_var_expansion(t_io *io, t_data *data);

//exec_expand_heredoc2.c
int			ft_heredoc_handle_var_expansion(t_node *node, t_word *word, \
											int *i, t_data *data);

//exec_expand_io1.c
void		ft_t_io_redir_expansion(t_io *io, t_data *data);

//exec_expand_io2.c
int			ft_redir_handle_var_expansion(t_node_io *node, t_word *word, \
											int *i,	t_data *data);

//exec_expand_utils.c
int			ft_get_beginning_of_list(t_node *temp, t_node **head);
int			ft_terminate_word_and_add_to_list(t_expand *exp_data);
//int			ft_terminate_word_and_add_to_list(t_node **list, t_word **word);
void		ft_free_char_array(char **arr);

//exec_main.c
void		executor(t_node **head, t_data *data, int es);
void		change_env_exit_status(t_data *data, int es);

// exec_pipeline
t_node		*create_execution_pipeline(t_node **head, t_data *data, \
										int *last_depth);
void		ft_free_pipeline(t_node **pipeline);
void		traverse_tree_rec(t_bin *tree, t_node **head);

// exec_parent
int			ft_open_pipes(t_exec *exec_data);
int			ft_parent_waitpid(t_exec *exec_data);
void		ft_exec_data_free_pipes(t_exec *exec_data);

#endif /* EXEC_H */