/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 17:35:50 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/23 22:46:08 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "struct.h"

//lexer_utils1.c
void	*ft_t_token_create(void *content);
void	*ft_t_node_create(void *content);
void	*ft_t_node_get_last(void *head);
void	ft_t_node_add_back(t_node **head, t_node *node);
int		ft_t_node_size(t_node *head);

//lexer_utils2.c
void	ft_t_node_free(t_node *head);
//void	ft_t_node_free_heredoc(t_token *token, t_node *temp);
void	ft_t_node_detach_and_free(t_node *to_detach);

//lexer_utils3.c
int		ft_t_word_init(t_word *word);
int		ft_t_word_append_char(t_word *word, char c);
char	*ft_t_word_get_str(t_word *word);
void	ft_skip_set(char *input, int *pos, char *set);

//lexer_utils4.c
void	ft_s_node_print_content(t_node *head);
//int	ft_skip_chars(char *input, int *pos, char *chars);
//int	ft_is_type_redirection(t_node *node);

//lexer_append1.c
int		ft_append(char *input, int *i, t_word *word, t_node **head);

//lexer_append2.c
int		ft_append_variable(char *input, int *i, t_word *word, t_node **head);

//lexer_append3.c
int		ft_terminate_token(char *input, int *i, t_word *word, t_node **head);
int		ft_append_token(t_word *word, t_node **head, int end);
int		ft_append_token_helper(char *str, t_node **head, int type, int status);
char	ft_set_type(t_word *word);
void	ft_t_token_add_empty_string(t_word *word, t_node **head, int *i);

//lexer_heredoc.c
int		ft_lexer_heredoc(t_node *head);

//lexer_tokenlist.c
t_node	*ft_get_token_list(char *input);

//lexer_main.c
t_node	*ft_lexer(char *input);

#endif /*LEXER_H*/
