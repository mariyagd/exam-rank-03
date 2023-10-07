#include "ft_printf.h"
#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>


void print_string(char *s, int *count)
{
    int i;

    i = 0;
    while (s[i])
    {
        write(1, &s[i], 1);
        (*count)++;
        i++;
    }
}

void    ft_itoa(int num, int *count)
{
    char c;

    c = 0;
    if (num == INT_MIN)
    {
        write(1, "-2147483648", 11);
        (*count) += 11;
        return ;
    }
    if (num < 0)
    {
        write(1, "-", 1);
        (*count)++;
        num *= -1;
    }
    if (num > 9)
    {
        ft_itoa(num / 10, count);
        ft_itoa(num % 10, count);
    }
    if (num < 10)
    {
        c = num + 48;
        write(1, &c, 1);
        (*count)++;
    }
}

void print_hexadecimal(unsigned int num, int *count)
{
    (void) num;
    (void) *count;
}


int ft_printf(const char *last, ...)
{
    va_list ap;
    int i;
    int count;

    i = 0;
    count = 0;
    va_start(ap, last);
    while (last[i])
    {
        if (last[i] != '%')
        {
            write(1, &last[i], 1);
            count++;
        }
        else
        {
            if (last[i + 1] == 's')
                print_string(va_arg(ap, char *), &count);
            else if (last[i + 1] == 'd')
                ft_itoa(va_arg(ap, int), &count);
            else if (last[i + 1] == 'x')
                print_hexadecimal(va_arg(ap, unsigned int), &count);
            i++;
        }

        i++;
    }
    va_end(ap);
    return (count);
}

int main(void)
{
    char    *s = "test";
    int     a = 65935;
    int     res1;
    int     res2;

    res1 = ft_printf("hello %s %d\n", s, a);
    res2 = printf("hello %s %d\n", s, a);

    printf("%d\n", res1);
    printf("%d\n", res2);
    return (0);
}
// hello test 65935