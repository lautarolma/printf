/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laviles <laviles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 03:17:47 by laviles           #+#    #+#             */
/*   Updated: 2025/11/09 07:07:26 by laviles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

static int	print_char(char c)
{
	return (write(1, &c, 1));
}

static int	print_str(char *str)
{
	int	count;

	if (!str)
		return (write(1, "(null)", 6));
	count = 0;
	while (*str)
	{
		print_char((int)*str);
		++count;
		++str;
	}
	return (count);
}

static int	print_digit(unsigned long n, unsigned int base, char *symbol)
{
	int		count;

	if (n < 0)
	{
		count = write(1, "-", 1);
		n *= -1;
	}
	else if (n < base)
	{
		return (print_char(symbol[n]));
	}
	else
		count = print_digit(n / base, base, symbol);
	return (count + print_digit(n % base, base, symbol));
}

static int	print_format(char specifier, va_list ap)
{
	int		count;
	void	*ptr;
	char	*x_base;

	x_base = "0123456789abcdef";
	count = 0;
	if (specifier == 'c')
		count += print_char(va_arg(ap, int));
	else if (specifier == 's')
		count += print_str(va_arg(ap, char *));
	else if (specifier == 'd' || specifier == 'i')
		count += print_digit(va_arg(ap, int), 10, "0123456789");
	else if (specifier == 'x')
		count += print_digit(va_arg(ap, unsigned int), 16, "0123456789abcdef");
	else if (specifier == 'X')
		count += print_digit(va_arg(ap, unsigned int), 16, "0123456789ABCDEF");
	else if (specifier == 'p')
	{
		ptr = va_arg(ap, void *);
		if (ptr == NULL)
			count += write(1, "(nil)", 5);
		else
		{
			count += write(1, "0x", 2);
			count += print_digit((unsigned long)ptr, 16, x_base);
		}
	}
	else
		count += write(1, &specifier, 1);
	return (count);
}

int	ft_printf(char const *format, ...)
{
	int		count;
	va_list	ap;

	if (!format)
		return (-1);
	va_start(ap, format);
	count = 0;
	while (*format)
	{
		if (*format == '%')
		{
			count += print_format(*++format, ap);
			format++;
		}
		else
		{
			count += write(1, format, 1);
			format++;
		}
	}
	va_end(ap);
	return (count);
}

int	main()
{
	int		my_nbytes	= 0;
	int		nbytes		= 0;
	//char	*user		= "JHON";
	short	age			= 24;
	int		lvl			= 4;
	//void	*location	= &user;
	void	*new		= &age;

	my_nbytes = printf("User1: %s\n Age: %d\n Language lvl: %i\n Memory Location:\t %p\n Next Location Adress: \t %p\n\n", (char *)NULL, age, lvl, NULL, new);
	nbytes = ft_printf("User2: %s\n Age: %d\n Language lvl: %i\n Memory Location:\t %p\n Next Location Adress: \t %p\n\n", (char *)NULL, age, lvl, NULL, new);
	printf("My printf\t\tprint %dbytes\n", nbytes);
	printf("The Oiginal printf\tprint %dbytes\n", my_nbytes);
	return(0);
}
/* debo sacar el control de if(n < 0) print - y count +1 y llevarlo a la funcion de print_format en el caso especifico de los %d y %i. De modo contrario estas flags no estan soportando los imputs de int negativos, pues seran casteados al ingresar a un unsigned long, wich forse to positive value.
else if (specifier == 'd' || specifier == 'i')
{
	int num = va_arg(ap, int);

	if (num < 0)
	{
		count += write(1, "-", 1);
		num = -num;
	}
	count += print_digit((unsigned long)num, 10, "0123456789");
}*/
