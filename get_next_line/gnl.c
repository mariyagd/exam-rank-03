/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanchev <mdanchev@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 18:47:06 by mdanchev          #+#    #+#             */
/*   Updated: 2023/10/23 18:49:13 by mdanchev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "gnl.h"

void	ft_bzero(char *s, int size)
{
	int i;

	i = 0;
	if (!s)
		return ;
	while (i < size)
	{
		s[i] = '\0';
		i++;
	}
}

char	*ft_calloc(size_t size, size_t n_bytes)
{
	char	*s;
	size_t 	i;

	s = malloc(size * n_bytes);
	if (!s)
		return (NULL);
	i = 0;
	while (i < size * n_bytes)
	{
		s[i] = '\0';
		i++;
	}
	return (s);
}

bool there_is_n(char *s)
{
	int i;

	i = 0;
	if (!s)
		return (false);
	while (s[i] != '\0')
	{
		if (s[i] == '\n')
			return (true);
		i++;
	}
	return (false);
}

size_t ft_strlen(char *s)
{
	size_t i;

	i = 0;
	if (!s)
		return (i);
	while (s[i])
		i++;
	return (i);
}

char	*free_(char *s1, char *s2)
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

char	*ft_join(char *s1, char *s2)
{
	int i;
	int j;
	char	*new;

	i = 0;
	j = 0;
	new = ft_calloc(sizeof (char), ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!new)
		return (NULL);
	while (s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		new[i] = s2[j];
		i++;
		j++;
	}
	free_(s1, NULL);
	return (new);
}

char	*read_fd(char *line, int fd)
{
	char	*buffer;
	int 	bytes;

	bytes = 1;

	if (!line)
	{
		line = ft_calloc(sizeof(char), 1);
		if (!line)
		{
			printf("hello\n");
			return (NULL);
		}
	}
	if (there_is_n(line) == true)
		return (line);
	buffer = ft_calloc(sizeof (char), BUFFER_SIZE);
	if (!buffer)
		return (free_(line, NULL));
	while (bytes > 0)
	{
		bytes = read(fd, buffer, (size_t)BUFFER_SIZE);
		if (bytes < 0)
			return (free_(buffer, line));
		buffer[bytes] = '\0';
		line = ft_join(line, buffer);
		if (!line)
			return (free_(line, buffer));
		if (there_is_n(line) == true)
			break ;
	}
	free_(buffer, NULL);
	if (line[0] == '\0')
		return (free_(line, NULL));
	return (line);
}

char	*get_nx_line(char *line)
{
	char	*new;
	size_t i;
	size_t j;

	i = 0;
	j = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	if (line[i] == '\n')
		i++;
	new = ft_calloc(sizeof(char), ft_strlen(line) - i + 1);
	if (!new)
		return (NULL);
	while(line[i] != '\0')
	{
		new[j] = line[i];
		i++;
		j++;
	}
	free_(line, NULL);
	return (new);

}

char	*get_result(char *line)
{
	size_t 	i;
	size_t len;
	char	*new;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	if (line[i] == '\n')
		len = i + 1;
	else
		len = i;
	new = ft_calloc(sizeof (char), len + 1);
	if (!new)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new[i] = line[i];
		i++;
	}
	return (new);
}

char	*get_next_line(int fd)
{
	static char	*line;
	char		*result;

	if (BUFFER_SIZE <= 0 || fd <= 0)
		return (NULL);
	line = read_fd(line, fd);
	if (!line)
		return (NULL);
	result = get_result(line);
	if (!result)
		return (free_(line, NULL));
	line = get_nx_line(line);
	if (!line)
		return (free_(result, line));
	return (result);
}


int	main(void)
{
	int		fd;
	char	*line;
	int 	i;

	i = 0;
	fd = open("test", O_RDONLY);
	line = get_next_line(fd);
	while(line)
	{
		printf("[%d] = %s", i, line);
		free(line);
		line = get_next_line(fd);
		i++;
	}
	return (0);
}