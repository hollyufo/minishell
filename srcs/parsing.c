#include "../includes/minishell.h"

static int	count_args(char *line)
{
	int	count;
	int	in_quote;
	int	in_dquote;

	count = 0;
	in_quote = 0;
	in_dquote = 0;
	while (*line)
	{
		if (*line == '\'' && !in_dquote)
			in_quote = !in_quote;
		else if (*line == '\"' && !in_quote)
			in_dquote = !in_dquote;
		else if (*line == ' ' && !in_quote && !in_dquote)
			count++;
		line++;
	}
	return (count + 1);
}

static char	**split_args(char *line)
{
	char	**args;
	int		count;
	int		i;
	int		start;
	int		in_quote;
	int		in_dquote;

	count = count_args(line);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	start = 0;
	in_quote = 0;
	in_dquote = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !in_dquote)
			in_quote = !in_quote;
		else if (line[i] == '\"' && !in_quote)
			in_dquote = !in_dquote;
		else if (line[i] == ' ' && !in_quote && !in_dquote)
		{
			args[i] = ft_substr(line, start, i - start);
			start = i + 1;
		}
		i++;
	}
	args[i] = ft_substr(line, start, i - start);
	args[count] = NULL;
	return (args);
}

static void	handle_quotes(char **str)
{
	char	*new;
	int		i;
	int		j;
	int		in_quote;
	int		in_dquote;

	new = malloc(ft_strlen(*str) + 1);
	if (!new)
		return ;
	i = 0;
	j = 0;
	in_quote = 0;
	in_dquote = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '\'' && !in_dquote)
			in_quote = !in_quote;
		else if ((*str)[i] == '\"' && !in_quote)
			in_dquote = !in_dquote;
		else
			new[j++] = (*str)[i];
		i++;
	}
	new[j] = '\0';
	free(*str);
	*str = new;
}

static void	expand_variables(t_shell *shell, char **str)
{
	char	*new;
	char	*var;
	char	*value;
	int		i;
	int		j;

	new = malloc(ft_strlen(*str) + 1);
	if (!new)
		return ;
	i = 0;
	j = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '$' && (*str)[i + 1])
		{
			var = ft_substr(*str, i + 1, ft_strlen(*str) - i - 1);
			value = get_env_value(shell->env, var);
			free(var);
			if (value)
			{
				ft_strlcpy(new + j, value, ft_strlen(value) + 1);
				j += ft_strlen(value);
			}
			i += ft_strlen(var) + 1;
		}
		else
			new[j++] = (*str)[i++];
	}
	new[j] = '\0';
	free(*str);
	*str = new;
}

t_cmd	*parse_command(t_shell *shell, char *line)
{
	t_cmd	*cmd;
	char	**args;
	int		i;

	args = split_args(line);
	if (!args)
		return (NULL);
	i = 0;
	while (args[i])
	{
		handle_quotes(&args[i]);
		expand_variables(shell, &args[i]);
		i++;
	}
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = args;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	return (cmd);
} 