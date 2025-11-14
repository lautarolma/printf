#include "ft_printf.h"

int	print_char(int c)
{
	return (write(1, &c, 1));
}

int	print_str(char *str)
{
	int	count;

	if (!str)
		return (write(1, "(null)", 6));
	count = 0;
	while (*str)
	{
		count += print_char((int)*str);
		str++;
	}
	return (count);
}

int	print_unsigned(unsigned long long n, unsigned int base, char *symbol)
{
	int	count;

	count = 0;
	if (n >= base)
		count += print_unsigned(n / base, base, symbol);
	count += print_char(symbol[n % base]);
	return (count);
}

int	print_signed(long long n, unsigned int base, char *symbol)
{
	int		count;

	count = 0;
	if (n < 0)
	{
		count += write(1, "-", 1);
		n = ((unsigned long long)-(n + 1)) + 1;
	}
	if (n >= base)
		count += print_unsigned(n / base, base, symbol);
	return (count += print_char(symbol[n % base]));
}

int	print_pointer(void *ptr)
{
	char	*x_base;
	int		count;

	count = 0;
	x_base = "0123456789abcdef";
	if (ptr == NULL)
		return (write(1, "(nil)", 5));
	count += write(1, "0x", 2);
	return (count += print_unsigned((unsigned long)ptr, 16, x_base));
}
