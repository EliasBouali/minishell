#include "minishell.h"

static	int	ft_count_words(char const *str, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i] != '\0')
	{
		while (str[i] && str[i] == c)
			i++;
		if (str[i])
			count++;
		while (str[i] && str[i] != c)
			i++;
	}
	return (count);
}

static	char	*ft_make_word(char const *str, char c, int i)
{
	int		len;
	int		j;
	char	*word;

	len = 0;
	j = i;
	while (str[j] && str[j] != c)
	{
		len++;
		j++;
	}
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	j = 0;
	while (j < len)
		word[j++] = str[i++];
	word[j] = '\0';
	return (word);
}

static	char	**ft_free(char **words, int j)
{
	while (j >= 0)
		free(words[j--]);
	free(words);
	return (NULL);
}

static	int	fill_word(char **words, const char *s, char c, int *i)
{
	while (s[*i] && s[*i] == c)
		(*i)++;
	if (!s[*i])
		return (0);
	words[0] = ft_make_word(s, c, *i);
	if (!words[0])
		return (-1);
	while (s[*i] && s[*i] != c)
		(*i)++;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	char	**words;
	int		status;

	if (!s)
		return (0);
	words = malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	i = 0;
	j = 0;
	if (!words)
		return (NULL);
	while (s[i])
	{
		status = fill_word(&words[j], s, c, &i);
		if (status == -1)
			return (ft_free(words, j));
		j += status;
	}
	words[j] = NULL;
	return (words);
}
