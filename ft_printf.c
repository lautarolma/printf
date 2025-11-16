/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laviles <laviles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 03:17:47 by laviles           #+#    #+#             */
/*   Updated: 2025/11/15 23:50:54 by laviles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_format(char specifier, va_list ap)
{
	int		count;

	count = 0;
	if (specifier == 'c')
		count += print_char(va_arg(ap, int));
	else if (specifier == 's')
		count += print_str(va_arg(ap, char *));
	else if (specifier == 'd' || specifier == 'i')
		count += print_signed((long long)va_arg(ap, int), 10, "0123456789");
	else if (specifier == 'u')
		count += print_unsigned((unsigned long long)va_arg(ap, unsigned int),
				10, "0123456789");
	else if (specifier == 'x')
		count += print_unsigned((unsigned long long)va_arg(ap, unsigned int),
				16, "0123456789abcdef");
	else if (specifier == 'X')
		count += print_unsigned((unsigned long long)va_arg(ap, unsigned int),
				16, "0123456789ABCDEF");
	else if (specifier == 'p')
		count += print_pointer(va_arg(ap, void *));
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
			format++;
			count += print_format(*format, ap);
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
/*
int	main()
{
	int		my_nbytes	= 0;
	int		nbytes		= 0;
	char	*user		= "JHON";
	int		age			= 24;
	int		lvl			= 5;
	void	*location	= &user;
	void	*new		= &age;
	char	bloodtype	= 'B';

//	my_nbytes = printf("User1: %s\n Age: %d\n Language lvl: %i\n
//		Memory Location:\t %p\n Next Location Adress: \t %p\n\n",
//		(char *)NULL, age, lvl, NULL, new);
//	nbytes = ft_printf("User2: %s\n Age: %d\n Language lvl: %i\n
//		Memory Location:\t %p\n Next Location Adress: \t %p\n\n",
//		(char *)NULL, age, lvl, NULL, new);
	my_nbytes = printf(" User1: %s\n Blood Type: %c\n Age: %d\n Language lvl:\
	%i\n Memory Location:\t %p\n Next Location Adress: \t %p\n %%100\n\n",\
	(char *)user, bloodtype, age, lvl, location, new);
	nbytes = ft_printf(" User2: %s\n Blood Type: %c\n Age: %d\n Language lvl:\
	%i\n Memory Location:\t %p\n Next Location Adress: \t %p\n %%100\n\n",\
	(char *)user, bloodtype, age, lvl, location, new);
	printf("The Oiginal printf\tprint %dbytes\n", my_nbytes);
	printf("My printf\t\tprint %dbytes\n", nbytes);
	return(0);
}*/
