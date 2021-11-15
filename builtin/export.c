/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 17:21:48 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/15 18:14:50 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	chararr_len(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
		i++;
	return (i);
}

int	export_fill_array(char ***envp, char **new_envp, char *new, int amount)
{
	int	i;

	i = 0;
	while (i < amount)
	{
		new_envp[i] = ft_strdup(*(envp[i]));
		if (new_envp[i] == NULL)
		{
			free_char_arr(new_envp);
			return (-1);
		}
		i++;
	}
	new_envp[i] = ft_strdup(new);
	if (new_envp[i] == NULL)
	{
		free_char_arr(new_envp);
		return (-1);
	}
	return (0);
}

export_check_input(char *str)
{
	
}

int	export_main(char ***envp, char *new)
{
	int		amount;
	int		i;
	char	**new_envp;

	if (!export_check_input(new))
		return (-1);
	amount = chararr_len(*envp);
	new_envp = ft_calloc(amount + 2, sizeof(char *));
	if (new_envp == NULL)
		return (-1);
	if (export_fill_array(envp, new_envp, new, amount) == -1)
		return (-1);
	free_char_arr(envp);
	envp = &new_envp;
	return (0);
}
