#include "../include/minishell.h"

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
	{
		return (1);
	}
	return (0);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
	{
		return (1);
	}
	return (0);
}

int	ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9'))
	{
		return (1);
	}
	return (0);
}

static	int	num_count(int n)
{
	int		count;
	long	nb;

	count = 0;
	nb = (long)n;
	if (nb == 0)
		return (1);
	if (nb < 0)
	{
		count = 1;
		nb = -nb;
	}
	while (nb > 0)
	{
		nb = nb / 10;
		count++;
	}
	return (count);
}

static	void	itoa_fill(char *str, long nb, int index)
{
	while (nb > 0)
	{
		str[index] = (nb % 10) + '0';
		nb = nb / 10;
		index--;
	}
}

char	*ft_itoa(int n)
{
	int		size;
	long	nb;
	char	*str;

	nb = (long)n;
	size = num_count(n);
	str = malloc((size + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[size] = '\0';
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	if (nb == 0)
		str[0] = '0';
	else
		itoa_fill(str, nb, size - 1);
	return (str);
}
