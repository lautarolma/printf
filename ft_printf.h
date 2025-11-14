#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

int		ft_printf(char const *format, ...);
int		print_pointer(void *ptr);
int		print_char(int c);
int		print_str(char *str);
int		print_signed(long long n, unsigned int base, char *symbol);
int		print_unsigned(unsigned long long n, unsigned int base, char *symbol);
int		print_format(char specifier, va_list ap);

#endif
