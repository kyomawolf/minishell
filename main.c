#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

void	main_loop(struct somestruct *data)
{
	while (1)
	{
		data->input = readline(data->output);
		add_history(data->input);
		if (!ft_strncmp(data->input, "exit\0", 5))
			return ;
		if (!parser_main(data))
			error_main(data);
		else
			execution_main(data);
		memory_loop_main(data);
	}
}

int	main(void)
{
	struct somestruct	*data;

	data = NULL;
	data = init_main();
	if (data == NULL)
		return (0);
	main_loop(data);
	memory_main(data);
}
