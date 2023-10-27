/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanchev <mdanchev@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 07:13:49 by mdanchev          #+#    #+#             */
/*   Updated: 2023/10/27 15:28:50 by mdanchev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

void    print_string(char *s, int *count)
{
	if (!s)
	{
		*count += (int)write(1, "(null)", 6);
		return ;
	}
	while (*s)
	{
		*count += (int)write(1, &(*s), 1);
		s++;
	}
}

void print_decimal(int num, int *count)
{
	if (num == -2147483648)
	{
		*count += (int)write(1, "-2147483648", 11);
		return ;
	}
	if (num < 0)
	{
		*count += (int)write(1, "-", 1);
		num *= -1;
	}
	if (num > 9)
	{
		print_decimal(num / 10, count);
		print_decimal(num % 10, count);
	}
	else
		*count += (int)write(1, &(char){num + 48}, 1);
}

void    print_hexadecimal(unsigned int hex, int *count)
{
	if (hex > 15)
	{
		print_hexadecimal(hex / 16, count);
		print_hexadecimal(hex % 16, count);
	}
	else if (hex > 9 && hex < 16)
		*count += (int)write(1, &(char){hex + 'a' - 10}, 1);
	else
		*count += (int)write(1, &(char){hex + '0'}, 1);
}

int ft_printf(const char *last, ... )
{
	int     i;
	int     count;
	va_list ap;

	i = 0;
	count = 0;
	va_start(ap, last);
	while (last[i])
	{
		if (last[i] != '%')
			count += (int)write(1, &last[i], 1);
		else
		{
			if (last[i + 1] == 's')
				print_string(va_arg(ap, char *), &count);
			if (last[i + 1] == 'd')
				print_decimal(va_arg(ap, int), &count);
			if (last[i + 1] == 'x')
				print_hexadecimal(va_arg(ap, unsigned int), &count);
			if (last[i + 1] == '%')
				count += (int)write(1, &last[i], 1);
			i++;
		}
		i++;
	}
	va_end(ap);
	return (count);
}

/*
int	main(void)
{
	int	res1;
	int	res2;

	res1 = printf("salut %s sa%dva%xhaha\n", "comment", 10, 654);
	res2 = ft_printf("salut %s sa%dva%xhaha\n", "comment", 10, 654);

	printf("res1 = %d\n", res1);
	printf("res2 = %d\n", res2);
}*/
