/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanchev <mdanchev@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 16:44:48 by mdanchev          #+#    #+#             */
/*   Updated: 2023/10/07 22:56:30 by mdanchev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1
#endif

void    ft_bzero(char *s, int size)
{
	int i;

	i = 0;
	while (i < size + 1)
	{
		s[i] = '\0';
		i++;
	}
}

char    *ft_calloc(int count)
{
	char *s;

	s = (char *)malloc(sizeof(char) * (count + 1));
	if (s == NULL)
		return (NULL);
	ft_bzero(s, BUFFER_SIZE);
	return (s);
}

size_t ft_strlen(char *s)
{
	size_t i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

bool check_for_n(char *s)
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

char *ft_strjoin(char *line, char *buffer)
{
	size_t  size;
	char    *new;
	int     i;
	int     j;

	i = 0;
	j = 0;
	size = ft_strlen(line) + ft_strlen(buffer);
	if (size == 0)
	{
		free(line);
		line = NULL;
		return (NULL);
	}
	//new = ft_calloc(size);
	new = (char *)malloc(sizeof(char) * (size + 1));
	if (!new)
		return (free(line), NULL);
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
	new[i] = '\0';
	free(line);
	line = NULL;
	//line = new;
	return (new);
}

char *read_fd(char *line, int fd)
{
	char    *buffer;
	int     ret;

	ret = 1;
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 2));
	if (!buffer)
		return (NULL);
	ft_bzero(buffer, BUFFER_SIZE + 1);
	if (!line)
	{
		//line = ft_calloc(0);
		line = malloc(sizeof(char) * 1);
		if (line == NULL)
		{
			free(buffer);
			return (NULL);
		}
		line[0] = '\0';
	}
	while (ret > 0)
	{
		ret = (int)read(fd, buffer, BUFFER_SIZE);
		if (ret < 0 && check_for_n(line) == false)
		{

			free(buffer);
			free(line);
			return (NULL);
		}
		if (ret == 0)
			break ;
		buffer[ret] = '\0';
		line = ft_strjoin(line, buffer);
		if (!line)
			return (free(buffer), NULL);
		if (check_for_n(line) == true)
			return (free(buffer), line);
		ft_bzero(buffer, BUFFER_SIZE);
	}
	free(buffer);
	return (line);
}

char    *ft_split(char *fullline, char *line)
{
	char     *line_to_print;
	int     i;

	i = 0;
	while (fullline[i] != '\0')
	{
		i++;
		if (fullline[i] == '\n')
			break ;
	}
	//printf("i = %d\n", i);
	if (i == 0)
		return (NULL);
	//line_to_print = ft_calloc(i);
	line_to_print = malloc(sizeof(char) * (i + 2));
	if (!line_to_print)
		return (NULL);
	i = 0;
	while (fullline[i] != '\0')
	{
		line_to_print[i] = fullline[i];
		i++;
		if (line_to_print[i - 1] == '\n')
			break ;
	}
	line_to_print[i] = '\0';
	return (line_to_print);
}

char *remember_line(char *fullline)
{
	char    *line;
	int     i;
	int     j;
	int     size;

	i = 0;
	j = 0;
	size = ft_strlen(fullline);
//	printf("ful == %s\n", fullline);
	while (fullline[i])
	{
		if (fullline[i] == '\n')
			break ;
		i++;
	}
	if (fullline[i] == '\0')
	{
		return (NULL);
	}
	if (fullline[i] == '\n')
	{
		i++;
		size = i;
		while(fullline[i])
		{
			i++;
			j++;
		}
	}
//	line = ft_calloc(size - i);
	line = malloc(sizeof(char) * (j + 1));
	if (!line)
	{
		return (NULL);
	}
	i = 0;
	while (fullline[size])
	{
		line[i] = fullline[size];
		i++;
		size++;
	}
	return (line);
}

char *get_next_line(int fd)
{
	static char *line;
	char        *fullline;
	char        *line_to_print;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	fullline = read_fd(line, fd);
	if (!fullline)
	{
		if (line)
			free(line);
		return (NULL);
	}
	line_to_print = ft_split(fullline, line);
//	printf("print = %s\n", line_to_print);
	if (!line_to_print)
	{
		free(fullline);
		return (NULL);
	}
	line = remember_line(fullline);
	free(fullline);
	return (line_to_print);

}

int main(void)
{
	int     fd;
	char    *line;
	int     i;

	i = 0;
	fd = open("README.md", O_RDONLY);
	line = NULL;
	line = get_next_line(fd);
	while (line)
	{
		printf("%d = ", i);
		printf("%s", line);
		free(line);
		line = NULL;
		line = get_next_line(fd);
		i++;
	}
	close (fd);
	return (0);
}