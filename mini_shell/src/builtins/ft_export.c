#include "../../include/minishell.h"

static int	print_env_simple(t_env *env)
{
	t_env	*cur;

	cur = env;
	while (cur)
	{
		if (cur->value)
		{
			ft_putstr_fd(cur->name, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(cur->value, 1);
			ft_putstr_fd("\n", 1);
		}
		cur = cur->next;
	}
	return (0);
}

static int	split_name_and_value(const char *arg, char **out_name, char **out_val)
{
	size_t	i;

	*out_name = NULL;
	*out_val = NULL;
	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	*out_name = (char *)malloc(i + 1);
	if (!*out_name)
		return (0);
	ft_memcpy(*out_name, arg, i);
	(*out_name)[i] = '\0';
	if (arg[i] == '=')
	{
		*out_val = ft_strdup(arg + i + 1);
		if (!*out_val)
			return (free(*out_name), *out_name = NULL, 0);
	}
	return (1);
}

static int	export_one_var(const char *arg, t_env **env)
{
	char	*name;
	char	*value;
	int		ret;

	if (!split_name_and_value(arg, &name, &value))
		return (ft_putstr_fd("minishell: export: alloc error\n", 2), 1);
	if (!is_valid_name(name))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd((char *)arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (free(name), free(value), 1);
	}
	ret = env_set(env, name, value);
	free(name);
	free(value);
	if (ret == 2)
		return (ft_putstr_fd("minishell: export: alloc error\n", 2), 1);
	return (0);
}

int	ft_export(char **argv, t_env **env)
{
	int	i;
	int	exit_code;

	if (!env)
		return (1);
	if (!argv || !argv[1])
		return (print_env_simple(*env));
	exit_code = 0;
	i = 1;
	while (argv[i])
	{
		if (export_one_var(argv[i], env))
			exit_code = 1;
		i++;
	}
	return (exit_code);
}

