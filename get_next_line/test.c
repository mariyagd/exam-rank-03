/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanchev <mdanchev@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 15:30:59 by mdanchev          #+#    #+#             */
/*   Updated: 2023/10/09 16:59:32 by mdanchev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1
#endif

void     ft_bzero(char *s, int size)
{
	int i;

	i = 0;
	if (!s)
		return ;
	while (i < size + 1)
	{
		s[i] = '\0';
		i++;
	}
}

char    *ft_calloc(int count)
{
	char    *s;

	s = malloc(sizeof(char) * (count + 1));
	if (!s)
		return (NULL);
	ft_bzero(s, count);
	return (s);
}

int ft_strlen(char  *s)
{
	int i;

	i = 0;
	if (!s)
		return (i);
	while (s[i])
		i++;
	return (i);
}

bool    there_is_n(char *s)
{
	int i;

	i = 0;
	if (!s)
		return (false);
	while (s[i])
	{
		if (s[i] == '\n')
			return (true);
		i++;
	}
	return (false);
}

char    *free_helper(char *s1, char *s2)
{
	if (s1)
	{
		free(s1);
		s1 = NULL;
	}
	if (s2)
	{
		free(s2);
		s2 = NULL;
	}
	return (NULL);
}

char    *ft_strjoin(char *line, char *buffer)
{
	char    *new;
	int     i;
	int     j;

	i = 0;
	j = 0;
	if (ft_strlen(line) + ft_strlen(buffer) == 0)
		return (NULL);
	new = ft_calloc(ft_strlen(line) + ft_strlen(buffer));
	if (!new)
		return (NULL);
	while (line[i])
	{
		new[i] = line[i];
		i++;
	}
	while (buffer[j])
	{
		new[i] = buffer[j];
		i++;
		j++;
	}
	free_helper(line, NULL);
	return (new);
}



char    *read_fd(char *line, int fd)
{
	int     nBytes;
	char    *buffer;

	nBytes = 1;
	buffer = ft_calloc(BUFFER_SIZE +1);
	if (!buffer)
		return (free_helper(line, NULL));
	if (!line)
	{
		line = ft_calloc(0);
		if (!line)
			return (free_helper(buffer, NULL));
	}
	while (there_is_n(line) == false)
	{
		nBytes = (int) read(fd, buffer, BUFFER_SIZE);
		if (nBytes < 0)
			return (free_helper(line, buffer));
		else if (nBytes == 0)
			break ;
		line = ft_strjoin(line, buffer);
		if (!line)
			return (free_helper(line, buffer));
		ft_bzero(buffer, BUFFER_SIZE);
	}
	free(buffer);
	return (line);
}

char    *ft_split(char *fullline)
{
	char    *line_to_print;
	int     i;

	i = 0;
	if (fullline[i] =='\0')
		return (NULL);
	while (fullline[i] != '\0' && fullline[i] != '\n')
		i++;
	line_to_print = ft_calloc(i);
	if (!line_to_print)
		return (NULL);
	i = 0;
	while (fullline[i] != '\0' && line_to_print[i - 1] != '\n')
	{
		line_to_print[i] = fullline[i];
		i++;
	}
	return (line_to_print);
}

/*
int line_to_remember(char *fullline)
{
	int     i;

	i = 0;
	while (fullline[i] != '\0' && fullline[i] != '\n')
		i++;
	if (fullline[i] == '\0' || (fullline[i] == '\n' && fullline[i + 1] == '\0'))
		return (0);
	return (i + 1);
}

char    *remember_line(char *fullline, int start)
{
	int     size;
	char    *line;

	size = ft_strlen(fullline);
	line = ft_calloc(size - start);
	if (!line)
		return (NULL);
	size = 0;
	while (fullline[start])
	{
		line[size] = fullline[start];
		start++;
		size++;
	}
	return (line);
}
*/
char *remember_line(char *fullline)
{
	char    *line;
	int     i;
	int     j;

	i = 0;
	j = 0;
	while (fullline[i] != '\0' && fullline[i] != '\n')
		i++;
	if (fullline[i] == '\0')
		return (free_helper(fullline), NULL);
	i++;
	line = ft_calloc(ft_strlen(fullline) - i);
	if (!line)
		return (free_helper(fullline, NULL));
	while (fullline[i])
	{
		line[j] = fullline[i];
		i++;
		j++;
	}
	free(fullline);
	fullline = NULL;
	return (line);
}

char    *get_next_line(int fd)
{
	static char *line;
	char        *line_to_print;

	line_to_print = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = read_fd(line, fd);
	if (!line)
		return (NULL);
	line_to_print = ft_split(line);
	if (!line_to_print)
	{
		free_helper(line, NULL);
		return (NULL);
	}
	line = remember_line(line);
	return (line_to_print);
}

int main(void)
{
	int     fd;
	char    *line;
	int     i;

	i = 0;
	fd = open("test", O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("%d = ", i);
		printf("%s", line);
		free(line);
	line = get_next_line(fd);
		i++;
	}
	return (0);
}