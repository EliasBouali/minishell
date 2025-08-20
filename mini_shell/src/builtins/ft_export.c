#include "../../include/minishell.h"

int is_valid_name(char *s)
{
    int i;

    if (!s || s[0] == '\0')
        return 0;
    if (!((s[0] >= 'A' && s[0] <= 'Z') ||
          (s[0] >= 'a' && s[0] <= 'z') ||
          s[0] == '_'))
        return 0;
    i = 1;
    while (s[i])
    {
        if (!((s[i] >= 'A' && s[i] <= 'Z') ||
              (s[i] >= 'a' && s[i] <= 'z') ||
              (s[i] >= '0' && s[i] <= '9') ||
              s[i] == '_'))
            return 0;
        i++;
    }
    return 1;
}

int split_name_value(const char *arg, char **name_out, char **value_out)
{
	size_t i;
	size_t j;
	char   *name;
	char   *value;

	if (!arg || !name_out || !value_out)
		return (0);
	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;

	if (arg[i] == '=')
	{
		name = (char *)malloc(i + 1);
		if (!name)
			return (0);
		j = 0;
		while (j < i)
		{
			name[j] = arg[j];
			j++;
		}
		name[i] = '\0';
		value = ft_strdup(arg + i + 1);
		if (!value)
		{
			free(name);
			return (0);
		}
		*name_out = name;
		*value_out = value;
		return (1);
	}
	name = ft_strdup(arg);
	if (!name)
		return (0);
	*name_out = name;
	*value_out = NULL;
	return (1);
}

int ft_export(char **argv)
{
    int   i;
    int   exit_code;
    char *name;
    char *value;

    if (!argv || !argv[1])
        return ft_env(argv);
    exit_code = 0;
    i = 1;
    while (argv[i])
    {
        if (!split_name_value(argv[i], &name, &value))
        {
            ft_putstr_fd("minishell: export: allocation error\n", 2);
            exit_code = 1;
            i++;
            continue;
        }
        if (!is_valid_name(name))
        {
            ft_putstr_fd("minishell: export: `", 2);
            ft_putstr_fd(argv[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
            exit_code = 1;

        }
        else
        {

        }
        if (name)  free(name);
        if (value) free(value);
        i++;
    }
    return exit_code;
}
