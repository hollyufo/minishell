#include "../includes/minishell.h"

t_env	*create_env_list(char **envp)
{
	t_env	*env;
	t_env	*new;
	char	*equal;
	int		i;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		new = malloc(sizeof(t_env));
		if (!new)
			return (NULL);
		equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			*equal = '\0';
			new->key = ft_strdup(envp[i]);
			new->value = ft_strdup(equal + 1);
			*equal = '=';
		}
		else
		{
			new->key = ft_strdup(envp[i]);
			new->value = NULL;
		}
		new->next = env;
		env = new;
		i++;
	}
	return (env);
}

char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	update_env(t_shell *shell, char *key, char *value)
{
	t_env	*env;
	t_env	*new;

	env = shell->env;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			free(env->value);
			env->value = ft_strdup(value);
			return ;
		}
		env = env->next;
	}
	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = shell->env;
	shell->env = new;
}

void	remove_env(t_shell *shell, char *key)
{
	t_env	*env;
	t_env	*prev;

	env = shell->env;
	prev = NULL;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			if (prev)
				prev->next = env->next;
			else
				shell->env = env->next;
			free(env->key);
			free(env->value);
			free(env);
			return ;
		}
		prev = env;
		env = env->next;
	}
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
} 