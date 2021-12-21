#include "minis.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "struct.h"
#include <string.h>

char	*strjoin(char const *s1, char const *s2)
{
	size_t	lens1;
	size_t	lens2;
	char	*s3;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	lens1 = strlen(s1);
	lens2 = strlen(s2);
	s3 = calloc((lens1 + lens2 + 1), 1);
	if (s3 == NULL)
		return (NULL);
	memcpy(s3, s1, lens1);
	strlcat(s3, s2, lens1 + lens2 + 1);
	return (s3);
}

int	**child_setpipes(int **a_pipes, int flag)
{
	//set 0 and 15
	return (a_pipes);
}


int	child_process(int **a_pipes, char *command, int flag)
{
	int	fd;
	int	ret;

	a_pipes = child_setpipes(a_pipes, flag);//TODO sets first two pipes to close and dup the pipes acc to the flag
	ret = execve(command, NULL, NULL);
	close(a_pipes[0][0]);
	close(a_pipes[0][1]);
	exit(ret);
}

int	child_init(int **a_pipes, t_node *tree_list, pid_t children[2])
{
	a_pipes[0] = calloc(2, sizeof(int));
	a_pipes[1] = calloc(2, sizeof(int));
	pipe(a_pipes[0]);
	pipe(a_pipes[1]);
	children[0] = fork();
	if (!children[0])
		child_process(a_pipes, tree_list->content, setflag(0, 0));
	children[1] = fork();
	tree_list = tree_list->next;
	if (!children[1])
		child_process(a_pipes, tree_list->content, setflag(2, 5));
}

void	create_pipes(int **a_pipes, int io)
{
	close(a_pipes[io][0]);
	close(a_pipes[io][1]);
	pipe(a_pipes[io]);
}

int	child_piped(t_node *tree_list)
{
	pid_t	children[2];
	int		**a_pipes;
	int		buffer;
	int		ch;

	a_pipes = calloc(3, sizeof(int *));
	child_init(a_pipes, tree_list, children);
	ch = 0;
	tree_list = tree_list->next;
	while (tree_list->content != NULL)
	{
		children[ch % 2] = fork();
		if (!children[ch % 2])
		{
			child_process(a_pipes, tree_list->content, setflag(ch));//sort of like this NOT thought through
		}
		tree_list = tree_list->next;
		//pipe,pipe2->c1,c2->pipe3->c3,c4->pipe4->c5
	}
	while (i < count)
	{

		create_pipes(a_pipes, ch % 2);
		ch++;
	}
	return (buffer);
}

int	child_exec(t_node *tree_list)
{
	int		i;
	t_node	*ori;

	i = 0;
	ori = tree_list;
	while (tree_list != NULL && tree_list->content != NULL)
	{
		tree_list = tree_list->next;
		i++;
	}
	//if (i > 1)
	return (child_piped(ori));
	//else
	...;
}

int	main(int argc, char **argv)
{
	int		i;
	t_node	*node;
	t_node	*head;

	node = calloc(1, sizeof(t_node));
	node->content = strjoin(argv[1], argv[2]);
	head = node;
	i = 3;
	while (i < argc)
	{
		node->next = calloc(1, sizeof(t_node));
		node->content = strjoin(argv[1], argv[i]);
		((t_node *)node->next)->prev = node;
		node = node->next;
		i++;
	}
	child_exec(head);
}
