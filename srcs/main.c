#include "../includes/minishell.h"


static void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	handle_sigquit(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_redisplay();
}

void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = handle_sigquit;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	init_shell(t_shell *shell, char **envp)
{
	shell->env = create_env_list(envp);
	shell->cmd = NULL;
	shell->exit_status = 0;
	shell->line = NULL;
	shell->pwd = getcwd(NULL, 0);
	shell->oldpwd = NULL;
	setup_signals();
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	while (1)
	{
		shell.line = readline(PROMPT);
		if (!shell.line)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (*shell.line)
		{
			add_history(shell.line);
			shell.cmd = parse_command(&shell, shell.line);
			if (shell.cmd)
			{
				execute_command(&shell);
				free_cmd(shell.cmd);
				shell.cmd = NULL;
			}
		}
		free(shell.line);
		shell.line = NULL;
	}
	free_shell(&shell);
	return (0);
} 