/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstrantz <mstrantz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 17:25:23 by mstrantz          #+#    #+#             */
/*   Updated: 2021/12/15 17:07:12 by mstrantz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "minis.h"
#include "libft.h"
#include "struct.h"

void	*ft_t_node_create(void *content);
void	*ft_t_node_get_last(void *head);
void	ft_t_node_add_back(t_node **head, t_node *node);
void	ft_t_node_free(t_node *head);

//only for printing
void	ft_print_nodes(t_node *head)
{
	t_node	*temp;
	char	**temp_char;
	int		i;
	int		j, k;

	while (head != NULL)
	{
		if (((t_bin *)head->content) == NULL)
			printf("next\n");
		else if (((t_bin *)head->content)->command == NULL)
			printf("command null\n");
		else if (((t_bin *)head->content)->command->arguments == NULL)
			printf("arguments null \n");
		else
		{
			i = 0;
			while (((t_bin *)head->content)->command->arguments[i] != NULL)
			{
				printf("%s\n", ((t_bin *)head->content)->command->arguments[i++]);
			}
			if ((t_bin *)head->content != NULL &&
				((t_bin *)head->content)->io != NULL &&
				((t_bin *)head->content)->io->heredoc_node != NULL)
			{
				temp = ((t_bin *)head->content)->io->heredoc_node;
				while (temp != NULL)
				{
					printf("Heredoc content :%s:\n", temp->content);
					temp = temp->next;
				}
			}
			if ((t_bin *)head->content != NULL &&
				((t_bin *)head->content)->io != NULL &&
				((t_bin *)head->content)->io->input != NULL)
			{
				j = 0;
				temp_char = ((t_bin *)head->content)->io->input;
				while (temp_char[j] != NULL)
				{
					printf("io input :%s:\n", temp_char[j]);
					j++;
				}
			}
			if ((t_bin *)head->content != NULL &&
				((t_bin *)head->content)->io != NULL &&
				((t_bin *)head->content)->io->output != NULL)
			{
				k = 0;
				temp_char = ((t_bin *)head->content)->io->output;
				while (temp_char[k] != NULL)
				{
					printf("io input :%s:\n", temp_char[k]);
					k++;
				}
			}
		}
		head = head->next;
	}
}

void	traverse_tree_rec(t_bin *tree, t_node **head)
{
	int	i;

	if (tree->child == NULL)
	{
		if (tree->command != NULL && tree->command->arguments != NULL)
		{
			if (head != NULL && tree->control_op != PIPE)
				ft_t_node_add_back(head, ft_t_node_create(NULL));
			ft_t_node_add_back(head, ft_t_node_create(tree));
		}
		return ;
	}
	i = 0;
	while (tree->child[i] != NULL)
	{
		traverse_tree_rec(tree->child[i], head);
		i++;
	}
}

void	traverse_tree(t_bin *tree, t_node **head)
{
	traverse_tree_rec(tree, head);
	ft_print_nodes(*head);
	ft_t_bin_variable_expansion(*head);
	//print_binary_tree(tree, 3);
	ft_print_nodes(*head);
}
