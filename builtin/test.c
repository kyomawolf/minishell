#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	main()
{
	char	*some;

	some = malloc(12);
	printf("cwd: %s\n", getcwd(NULL, 0));
	chdir("..");
	system("ls");
	system("env");
}