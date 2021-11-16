/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkasper <jkasper@student.42Heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 19:28:13 by jkasper           #+#    #+#             */
/*   Updated: 2021/11/16 19:38:34 by jkasper          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

int	main(void)
{
	struct stat	t_struct;

	stat(getcwd(NULL, 0), &t_struct);
	printf("ID of containing device:  [%jx]\n",
			(uintmax_t) t_struct.st_dev);

	
	printf("I-node number:            %ju\n", (uintmax_t) t_struct.st_ino);

           printf("Mode:                     %jo (octal)\n",
                   (uintmax_t) t_struct.st_mode);

           printf("Link count:               %ju\n", (uintmax_t) t_struct.st_nlink);
           printf("Ownership:                UID=%ju   GID=%ju\n",
                   (uintmax_t) t_struct.st_uid, (uintmax_t) t_struct.st_gid);

           printf("Preferred I/O block size: %jd bytes\n",
                   (intmax_t) t_struct.st_blksize);
           printf("File size:                %jd bytes\n",
                   (intmax_t) t_struct.st_size);
           printf("Blocks allocated:         %jd\n",
                   (intmax_t) t_struct.st_blocks);

           printf("Last status change:       %s", ctime(&t_struct.st_ctime));
           printf("Last file access:         %s", ctime(&t_struct.st_atime));
           printf("Last file modification:   %s", ctime(&t_struct.st_mtime));
}
