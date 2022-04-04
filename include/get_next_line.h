/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/14 22:37:08 by jkasper           #+#    #+#             */
/*   Updated: 2022/03/19 15:29:50 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include "libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>
# include <fcntl.h>

# define GNL_BUFFER	100

char	*get_next_line(int fd);
int		get_filled(char **str, int fd);
void	get_moved(char **str, size_t i);
int		get_memory(char **old);
char	*e_exit(char **str, char **ret);
char	*s_exit(char **str, char **ret, size_t i, size_t ii);
#endif
