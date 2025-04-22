#include "../includes/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strncmp(cmd, "echo", 5) || !ft_strncmp(cmd, "cd", 3) ||
		!ft_strncmp(cmd, "pwd", 4) || !ft_strncmp(cmd, "export", 7) ||
		!ft_strncmp(cmd, "unset", 6) || !ft_strncmp(cmd, "env", 4) ||
		!ft_strncmp(cmd, "exit", 5))
		return (1);
	return (0);
}

static int	handle_redirections(t_cmd *cmd)
{
	int	fd;

	if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd == -1)
			return (error_exit("No such file or directory", 1));
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (cmd->outfile)
	{
		if (cmd->append)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (error_exit("Permission denied", 1));
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (0);
}

static int	execute_single_command(t_shell *shell, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	if (handle_redirections(cmd))
		return (1);
	if (is_builtin(cmd->args[0]))
		return (execute_builtin(shell, cmd));
	pid = fork();
	if (pid == 0)
	{
		execute_external(shell, cmd);
		exit(1);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	return (0);
}

static int	execute_pipeline(t_shell *shell, t_cmd *cmd)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	if (pipe(fd) == -1)
		return (error_exit("pipe failed", 1));
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execute_single_command(shell, cmd);
		exit(shell->exit_status);
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	return (0);
}

int	execute_command(t_shell *shell)
{
	t_cmd	*cmd;
	int		stdin_fd;
	int		stdout_fd;

	cmd = shell->cmd;
	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	while (cmd)
	{
		if (cmd->next)
		{
			if (execute_pipeline(shell, cmd))
				break ;
		}
		else
		{
			if (execute_single_command(shell, cmd))
				break ;
		}
		cmd = cmd->next;
	}
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdin_fd);
	close(stdout_fd);
	return (0);
} 