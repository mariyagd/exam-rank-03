/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanchev <mdanchev@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 10:12:11 by mdanchev          #+#    #+#             */
/*   Updated: 2023/10/09 11:58:09 by mdanchev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 0
#endif

void    ft_bzero(char *s, int size)
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

int ft_strlen(char *s)
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
	int     i;
	int     j;
	char    *new;

	i = 0;
	j = 0;
	if ((ft_strlen(line) + ft_strlen(buffer)) == 0)
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



char *read_fd(char *line, int fd)
{
	int     nBytes;
	char    *buffer;

	nBytes = 1;
	buffer = ft_calloc(BUFFER_SIZE + 1);
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
		nBytes = (int)read(fd, buffer, BUFFER_SIZE);
		if (nBytes < 0)
			return (free_helper(line, buffer));
		else if (nBytes == 0)
			break ;
	//	buffer[nBytes] = '\0';
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
	char     *line_to_print;
	int     i;

	i = 0;
	if (fullline[i] == '\0')
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
		return (NULL);
	i++;
	line = ft_calloc(ft_strlen(fullline) - i);
	if (!line)
		return (NULL);
	while (fullline[i])
	{
		line[j] = fullline[i];
		i++;
		j++;
	}
	return (line);
}

char *get_next_line(int fd)
{
	static char *line;
	char        *fullline;
	char        *line_to_print;

	fullline = NULL;
	line_to_print = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	fullline = read_fd(line, fd);
	if (!fullline)
		return (NULL);
	line_to_print = ft_split(fullline);
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
	fd = open("test", O_RDONLY);
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