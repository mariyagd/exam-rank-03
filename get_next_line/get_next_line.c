/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanchev <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 12:10:13 by mdanchev          #+#    #+#             */
/*   Updated: 2023/10/27 14:16:49 by mdanchev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (i);
	while (s[i])
		i++;
	return (i);
}

char	*ft_calloc(size_t count)
{
	char		*s;
	size_t		i;

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

char	*free_(char *s1, char *s2)
{
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	return (NULL);
}

bool	there_is_n(const char *s)
{
	int	i;

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

char	*ft_join(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*new;

	new = ft_calloc(ft_strlen(s1) + ft_strlen(s2));
	if (!new)
		return (free_(s1, NULL));
	i = 0;
	j = 0;
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

char	*prepare_string(char **line, char **buffer)
{
	if (!*line)
	{
		*line = ft_calloc(0);
		if (!*line)
			return (NULL);
	}
	*buffer = ft_calloc(BUFFER_SIZE);
	if (!*buffer)
		return (free_(*line, NULL));
	return ("ok");
}

char	*read_fd(int fd, char *line)
{
	int		bytes;
	char	*buffer;

	buffer = NULL;
	if (prepare_string(&line, &buffer) == NULL)
		return (NULL);
	while (line && there_is_n(line) == false)
	{
		bytes = (int)read(fd, buffer, BUFFER_SIZE);
		if (bytes < 0)
			return (free_(line, buffer));
		if (bytes == 0)
			break ;
		buffer[bytes] = '\0';
		line = ft_join(line, buffer);
	}
	free(buffer);
	if (line && line[0] == '\0')
		return (free_(line, NULL));
	return (line);
}

char	*get_result(const char *line)
{
	int		i;
	int		len;
	char	*new;

	len = 0;
	while (line[len] != '\0' && line[len] != '\n')
		len++;
	if (line[len] == '\n')
		len++;
	new = ft_calloc(len);
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

char	*get_nx_line(char *line)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	if (line[i] == '\n')
		i++;
	new = ft_calloc(ft_strlen(line) - i);
	if (!new)
		return (free_(line, NULL));
	j = 0;
	while (line[i])
	{
		new[j] = line[i];
		i++;
		j++;
	}
	free_(line, NULL);
	return (new);
}

char	*get_next_line(int fd)
{
	static char	*line;
	char		*result;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		free_(line, NULL);
		line = NULL;
		return (NULL);
	}
	line = read_fd(fd, line);
	if (!line)
		return (NULL);
	result = get_result(line);
	if (!result)
		return (free_(line, NULL));
	line = get_nx_line(line);
	if (!line)
	{
		free(result);
		line = NULL;
		result = NULL;
	}
	return (result);
}
