/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 14:24:56 by chon              #+#    #+#             */
/*   Updated: 2024/01/25 13:38:59 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*free_and_return_null(char *str)
{
	free(str);
	return (NULL);
}

char	*move_buffer_pos(char *buffer)
{
	char	*new_buffer_pos;
	int		i;
	int		j;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
		return (free_and_return_null(buffer));
	new_buffer_pos = malloc(ft_strlen(buffer) - i++ + 1);
	if (!new_buffer_pos)
		return (NULL);
	j = 0;
	if (!buffer[i])
	{
		free(new_buffer_pos);
		return (free_and_return_null(buffer));
	}
	while (buffer[i])
		new_buffer_pos[j++] = buffer[i++];
	new_buffer_pos[j] = '\0';
	free(buffer);
	return (new_buffer_pos);
}

char	*get_line(char *buffer)
{
	char	*line;
	int		i;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	line = malloc(i + 2);
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*pull_text(char *buffer, int fd)
{
	char	*text;
	int		bytes_read;

	text = malloc(BUFFER_SIZE + 1);
	if (!text)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr(buffer, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, text, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			if (buffer)
				free(buffer);
			return (free_and_return_null(text));
		}
		text[bytes_read] = '\0';
		buffer = ft_strjoin(buffer, text);
	}
	free(text);
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer[10240];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX - 1)
		return (NULL);
	buffer[fd] = pull_text(buffer[fd], fd);
	if (ft_strlen(buffer[fd]) == 0)
	{
		free(buffer[fd]);
		return (NULL);
	}
	line = get_line(buffer[fd]);
	buffer[fd] = move_buffer_pos(buffer[fd]);
	return (line);
}
