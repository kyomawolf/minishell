/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 13:53:37 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/16 15:49:28 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

int	cd_main(char **input, t_data *data)
{
	int		i;
	char	*path;

	if (chdir(input[1]) == -1)
	{
		ft_putstring_fd("No such file or directory: ");
		ft_putstring_fd(input[1]);
		write(1, "\n", 1);
		return (-1);
	}
	data->currdir = getcwd(NULL, 0);
	get_prompt(data);
	return (0);
}
