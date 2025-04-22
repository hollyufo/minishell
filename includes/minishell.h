#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <dirent.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include "libft.h"

# define PROMPT "minishell> "
# define MAX_PATH 4096

typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
} t_env;

typedef struct s_cmd
{
    char            **args;
    char            *infile;
    char            *outfile;
    int             append;
    struct s_cmd    *next;
} t_cmd;

typedef struct s_shell
{
    t_env   *env;
    t_cmd   *cmd;
    int     exit_status;
    char    *line;
    char    *pwd;
    char    *oldpwd;
} t_shell;

// Shell initialization
void    init_shell(t_shell *shell, char **envp);
void    setup_signals(void);

// Command execution
int     execute_command(t_shell *shell);
int     execute_builtin(t_shell *shell, t_cmd *cmd);
int     execute_external(t_shell *shell, t_cmd *cmd);
int     is_builtin(char *cmd);

// Builtins
int     ft_echo(t_cmd *cmd);
int     ft_cd(t_shell *shell, t_cmd *cmd);
int     ft_pwd(void);
int     ft_export(t_shell *shell, t_cmd *cmd);
int     ft_unset(t_shell *shell, t_cmd *cmd);
int     ft_env(t_shell *shell);
int     ft_exit(t_shell *shell, t_cmd *cmd);

// Environment
t_env   *create_env_list(char **envp);
char    *get_env_value(t_env *env, char *key);
void    update_env(t_shell *shell, char *key, char *value);
void    remove_env(t_shell *shell, char *key);

// Parsing
t_cmd   *parse_command(t_shell *shell, char *line);
void    expand_variables(t_shell *shell, char **str);
void    handle_quotes(char **str);

// Utils
void    free_shell(t_shell *shell);
void    free_cmd(t_cmd *cmd);
void    free_env(t_env *env);
int     error_exit(char *msg, int status);

#endif 