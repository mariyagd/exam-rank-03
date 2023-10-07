/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getNextLine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanchev <mdanchev@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 12:59:16 by mdanchev          #+#    #+#             */
/*   Updated: 2023/10/07 13:18:47 by mdanchev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1
#endif

void *ft_calloc(int count)
{
	char    *s;
	int     i;

	i = 0;
	s = malloc(sizeof(char) * (count + 1));
	if (!s)
		return (NULL);
	while (i < count + 1)
	{
		s[i] = '\0';
		i++;
	}
	return (s);
}

char *getNextLine(int fd)
{
	ssize_t nBytes;
	int     i;
	char    *buf;
	char    c;
	int     size;

	i = 0;
	size = 0;
	if (fd <= 0 || BUFFER_SIZE <= 0)
		return (NULL);
	nBytes = read(fd, &c, 1);
	if (nBytes <= 0)
		return (NULL);
	//buf = malloc(sizeof(char *) * BUFFER_SIZE + 1);
	buf = ft_calloc(10000);
	if (!buf)
		return (NULL);
	while (nBytes > 0)
	{
		buf[i] = c;
		if (c  == '\n')
			break ;
		nBytes = read(fd, &c, 1);
		if (nBytes < 0)
		{
			free(buf);
			return (NULL);
		}
		i++;
	}
	buf[i] = '\0';
	return (buf);
}

int main(void)
{
	int fd;
	char *line;

	fd = open("README.md", O_RDONLY);
	line = getNextLine(fd);
	while (line)
	{
		printf("%s\n", line);
		free(line);
		line = getNextLine(fd);
	}
	return (0);
}
