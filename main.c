/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 18:23:13 by jkasper           #+#    #+#             */
/*   Updated: 2021/12/18 15:25:45 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "minis.h"
#include "libft.h"
#include "struct.h"

void	get_prompt(t_data *data)
{
	int		i;
	int		ii;
	char	*temp;

	i = ft_strlen(data->currdir) - 1;
	temp = malloc(ft_strlen(data->currdir) + 2);
	ii = 0;
	while (data->currdir[i] != '/')
		i--;
	i++;
	while (data->currdir[i] != '\0')
	{
		temp[ii] = data->currdir[i];
		ii++;
		i++;
	}
	temp[ii++] = '-';
	temp[ii++] = '>';
	temp[ii++] = ' ';
	temp[ii] = '\0';
	if (data->prompt != NULL) // was commented
		free(data->prompt); // was commented
	data->prompt = ft_strjoin("minishell@", temp);
	free(temp); // was commented
}

void	main_loop(t_data *data)
{
	t_node *head;
	t_node	*temp;

	head = NULL;
	while (1)
	{
		input_attributes_add();
		input_readline(data);
		input_attributes_clear();
		if (data->input == NULL)
		{
			free_main(data);
			printf("\n\nWARNING: LEAVING MINISHELL\n");
			system("leaks minishell");
			exit(0);
		}
		if (data->input[0] == '\0')
			continue ;
		data->list = ft_lexer(data->input);
		if (data->list == NULL)
			continue ;
		data->tree = builder_main(data->list);
		if (data->tree != NULL)
			traverse_tree_rec(data->tree, &head);
		executor(head, data, 0);
		while (head != NULL)
		{
			temp = head;
			if (head->content != NULL)
			head = head->next;
			free(temp);
			temp = NULL;
		}
		//else
		//	execution_main(data);
		//memory_loop_main(data);
		free_tree(data->tree);
		get_prompt(data);
	}
}

void	main_print_entry(void)
{
	printf("         {\n      {   }\n       }_{ __{\n    .-{   }   }-.\n   (\
	}     {  )\n   |`-.._____..-'|\n   |             ;--.\n   |           \
 (__  \\\n   |             | )  )\n   |             |/  /\n   |        \
     /  /                    \n   |            (  /\n   \\          \
 	 y'\n    `-.._____..-'		 -Minishell v.0.01- \n");
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	if (argc > 1)
	{
		write(2, \
		"Error: Too many arguments. No arugment allowed.", 48);
		return (1);
	}
	data = NULL;
	data = new_t_data(envp);
	(void) argv;
	if (data == NULL)
		return (0);
	main_print_entry();
	main_loop(data);
	//memory_main(data);
}
