#include "../include/minishell.h"
int g_exit_code = 0;


// but lier un dossier du path avec la commande taper par l utilisateur.
static char *join_dir_cmd(char *path_directorie, char *cmd)
{
    char *tmp;
    char *full;

    tmp = ft_strjoin(path_directorie, "/");
    if (!tmp)
        return (NULL);
    full = ft_strjoin(tmp, cmd);
    free(tmp);
    return (full);
}

//but chercher dans chaque dossier du path si le binaire existe et peut etre execute
char *get_valid_exec_path(char **path_split, char *cmd)
{
    int   i;
    char *full_path;

    i = 0;
    while (path_split && path_split[i])
    {
        full_path = join_dir_cmd(path_split[i], cmd);
        if (!full_path)
        {
            free_split(path_split);
            return (NULL);
        }
        if (access(full_path, X_OK) == 0)
        {
            free_split(path_split);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    free_split(path_split);
    return (NULL);
}

// envoi la chaine path decoupe a get_valid_exec_path
char *get_path_to_cmd(char *cmd, const char *path_var)
{
    char **path_split;

    if (!path_var || !*path_var)
        return (NULL);
    path_split = ft_split(path_var, ':');
    if (!path_split)
        return (NULL);
    return (get_valid_exec_path(path_split, cmd));
}

void handle_command(t_command *cmd, t_env *env)
{
    char        *path;
    pid_t       pid;
    char        *cmd_name;
    const char  *path_var;
    char        **envp_child;
    int         status;

    if (!cmd || !cmd->args || !cmd->args[0])
        return;
    cmd_name = cmd->args[0];
    if (ft_strchr(cmd_name, '/'))
    {
        struct stat st;

        if (access(cmd_name, F_OK) != 0)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd_name, 2);
            ft_putendl_fd(": No such file or directory", 2);
            g_exit_code = 127;
            return;
        }
        if (stat(cmd_name, &st) == 0 && S_ISDIR(st.st_mode))
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd_name, 2);
            ft_putendl_fd(": Is a directory", 2);
            g_exit_code = 126;
            return;
        }
        if (access(cmd_name, X_OK) != 0)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd_name, 2);
            ft_putendl_fd(": Permission denied", 2);
            g_exit_code = 126;
            return;
        }
        path = cmd_name;
    }
    else
    {
        path_var = env_get(env, "PATH");
        path = get_path_to_cmd(cmd->args[0], path_var);
        if (!path)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd_name, 2);
            ft_putendl_fd(": command not found", 2);
            g_exit_code = 127;
            return;
        }
    }
    envp_child = env_to_envp(env);
    if (!envp_child)
    {
        ft_putstr_fd("minishell: env alloc error\n", 2);
        if (path != cmd_name)
            free(path);
        g_exit_code = 1;
        return;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("minishell");
        if (path != cmd_name)
            free(path);
        free_envp_array(envp_child);
        g_exit_code = 1;
        return;
    }
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        execve(path, cmd->args, envp_child);
        perror("minishell");
        free_envp_array(envp_child);
        if (path != cmd_name)
            free(path);
        _exit(126);
    }
    free_envp_array(envp_child);
    if (path != cmd_name)
        free(path);
    status = 0;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        g_exit_code = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        g_exit_code = 128 + WTERMSIG(status);
}


/*
char *get_env(char *nam, char **env)
{
	int i = 0;
	size_t len = ft_strlen(nam);

	while (env && env[i])
	{
		if (ft_strncmp(env[i], nam, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}
*/
