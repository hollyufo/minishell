#include "../includes/minishell.h"

int	ft_echo(t_cmd *cmd)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	if (cmd->args[1] && !ft_strncmp(cmd->args[1], "-n", 3))
	{
		n_flag = 1;
		i++;
	}
	while (cmd->args[i])
	{
		write(1, cmd->args[i], ft_strlen(cmd->args[i]));
		if (cmd->args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	return (0);
}

int	ft_cd(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	char	*oldpwd;

	if (!cmd->args[1])
		path = get_env_value(shell->env, "HOME");
	else
		path = cmd->args[1];
	oldpwd = getcwd(NULL, 0);
	if (chdir(path) == -1)
	{
		free(oldpwd);
		return (error_exit("No such file or directory", 1));
	}
	update_env(shell, "OLDPWD", oldpwd);
	free(oldpwd);
	path = getcwd(NULL, 0);
	update_env(shell, "PWD", path);
	free(path);
	return (0);
}

int	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (error_exit("getcwd failed", 1));
	write(1, pwd, ft_strlen(pwd));
	write(1, "\n", 1);
	free(pwd);
	return (0);
}

int	ft_export(t_shell *shell, t_cmd *cmd)
{
	int		i;
	char	*equal;

	if (!cmd->args[1])
	{
		// Print all environment variables
		return (0);
	}
	i = 1;
	while (cmd->args[i])
	{
		equal = ft_strchr(cmd->args[i], '=');
		if (equal)
		{
			*equal = '\0';
			update_env(shell, cmd->args[i], equal + 1);
			*equal = '=';
		}
		i++;
	}
	return (0);
}

int	ft_unset(t_shell *shell, t_cmd *cmd)
{
	int	i;

	if (!cmd->args[1])
		return (0);
	i = 1;
	while (cmd->args[i])
	{
		remove_env(shell, cmd->args[i]);
		i++;
	}
	return (0);
}

int	ft_env(t_shell *shell)
{
	t_env	*env;

	env = shell->env;
	while (env)
	{
		if (env->value)
		{
			write(1, env->key, ft_strlen(env->key));
			write(1, "=", 1);
			write(1, env->value, ft_strlen(env->value));
			write(1, "\n", 1);
		}
		env = env->next;
	}
	return (0);
}

int	ft_exit(t_shell *shell, t_cmd *cmd)
{
	int	exit_code;

	if (!cmd->args[1])
		exit_code = shell->exit_status;
	else if (cmd->args[2])
		return (error_exit("too many arguments", 1));
	else
	{
		exit_code = ft_atoi(cmd->args[1]);
		if (exit_code == 0 && cmd->args[1][0] != '0')
			return (error_exit("numeric argument required", 255));
	}
	free_shell(shell);
	exit(exit_code);
}

int	execute_builtin(t_shell *shell, t_cmd *cmd)
{
	if (!ft_strncmp(cmd->args[0], "echo", 5))
		return (ft_echo(cmd));
	else if (!ft_strncmp(cmd->args[0], "cd", 3))
		return (ft_cd(shell, cmd));
	else if (!ft_strncmp(cmd->args[0], "pwd", 4))
		return (ft_pwd());
	else if (!ft_strncmp(cmd->args[0], "export", 7))
		return (ft_export(shell, cmd));
	else if (!ft_strncmp(cmd->args[0], "unset", 6))
		return (ft_unset(shell, cmd));
	else if (!ft_strncmp(cmd->args[0], "env", 4))
		return (ft_env(shell));
	else if (!ft_strncmp(cmd->args[0], "exit", 5))
		return (ft_exit(shell, cmd));
	return (0);
} 