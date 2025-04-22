#include "../includes/minishell.h"

int	error_exit(char *msg, int status)
{
	write(2, "minishell: ", 11);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(status);
	return (status);
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->outfile)
		free(cmd->outfile);
	free(cmd);
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->env)
		free_env(shell->env);
	if (shell->cmd)
		free_cmd(shell->cmd);
	if (shell->line)
		free(shell->line);
	if (shell->pwd)
		free(shell->pwd);
	if (shell->oldpwd)
		free(shell->oldpwd);
}

int	execute_external(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	char	**envp;
	int		i;

	if (!cmd->args[0])
		return (0);
	path = get_env_value(shell->env, "PATH");
	if (!path)
		return (error_exit("No such file or directory", 127));
	envp = malloc(sizeof(char *) * (ft_strlen(path) + 1));
	if (!envp)
		return (error_exit("malloc failed", 1));
	i = 0;
	while (path[i])
	{
		envp[i] = ft_strjoin(path[i], "/");
		envp[i] = ft_strjoin(envp[i], cmd->args[0]);
		if (access(envp[i], X_OK) == 0)
		{
			execve(envp[i], cmd->args, NULL);
			free(envp[i]);
			free(envp);
			return (error_exit("execve failed", 1));
		}
		free(envp[i]);
		i++;
	}
	free(envp);
	return (error_exit("command not found", 127));
} 