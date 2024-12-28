/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiraga <mdiraga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 15:45:33 by mdiraga           #+#    #+#             */
/*   Updated: 2022/11/14 15:45:33 by mdiraga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	*ft_memset(void *str, int c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		*((char *)str + i) = c;
		i++;
	}
	return (str);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiraga <mdiraga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 10:28:15 by mdiraga           #+#    #+#             */
/*   Updated: 2022/12/05 17:08:23 by mdiraga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	if (!dst && !src)
		return (0);
	i = 0;
	while (i < n)
	{
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dst);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiraga <mdiraga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 13:02:08 by mdiraga           #+#    #+#             */
/*   Updated: 2022/11/14 13:02:08 by mdiraga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c		                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiraga <mdiraga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 15:16:40 by mdiraga           #+#    #+#             */
/*   Updated: 2022/11/11 15:16:40 by mdiraga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strcmp(const char *str1, const char *str2)
{
	size_t	i;

	i = 0;
	while (str1[i] || str2[i])
	{
		if ((unsigned char)str1[i] != (unsigned char)str2[i])
			return ((unsigned char)str1[i] - (unsigned char)str2[i]);
		i++;
	}
	return (0);
}

//	msg = error message | err_buffer_leng = leng of err_buffer
void	exit_func(char *msg, int err_buffer_leng)
{
    write(2, msg, ft_strlen(msg));

    for (int index = 0; index < err_buffer_leng; index++)
	{
        if (err_buffer[index])
		{
            free(err_buffer[index]);
            err_buffer[index] = NULL;
        }
    }

    exit(EXIT_FAILURE);
}

void    write_hash(void) //TODO free(global_hash) islemi yapilacak
{
	s_hash* tmp = &global_hash;

	while (TRUE)
	{
		int key = 0;

		for (int i = 0; i < 16; i++)
			if (tmp->hash[i] != '\0')
				key++;

		if (key == 0)
		{
			if (tmp->next == NULL)
				break;
			else
				tmp = tmp->next;
			continue;
		}

		char* hash_hex = malloc(33);

		for (int i = 0; i < 16; i++)
		{
			sprintf(hash_hex + (i * 2), "%02x", tmp->hash[i]);
		}
		hash_hex[32] = '\0'; // Null terminator

		if (global_flags.flag_quiet == FALSE)
		{
			if (global_flags.flag_reverse == FALSE)
			{
				if (tmp->type == TYPE_FILE)
				{
					printf("MD5 (%s) = ", tmp->file);
				}
				else if (tmp->type == TYPE_STRING)
				{
					printf("MD5 (\"%s\") = ", tmp->file);
				}
				else
				{
					printf("MD5 (STDIN) = ", tmp->file);
				}

				//printf("MD5 (%s) = ", tmp->file);
				printf("%s\n", hash_hex);
			}
			else
			{
				printf("%s = ", hash_hex);

				if (tmp->type == TYPE_FILE)
				{
					printf("MD5 (%s)\n", tmp->file);
				}
				else if (tmp->type == TYPE_STRING)
				{
					printf("MD5 (\"%s\")\n", tmp->file);
				}
				else
				{
					printf("MD5 (STDIN)\n", tmp->file);
				}
			}
		}
		else
			printf("%s\n", hash_hex);
		
		free(hash_hex);

		if (tmp->next == NULL)
			break;
		else
			tmp = tmp->next;
	}
}

#include <stddef.h> // NULL için

// İki stringi birleştiren basit bir strcat fonksiyonu
char *ft_strcat(char *dest, const char *src)
{
    char *start = dest;

    // `dest` dizisinin sonunu bul
    while (*dest)
        dest++;

    // `src`'yi `dest`'in sonuna ekle
    while (*src)
        *dest++ = *src++;

    // Null-terminator ekle
    *dest = '\0';

    return start;
}


char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = ft_strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + ft_strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		ft_strcat(newbuf, buf);
	free(buf);
	ft_strcat(newbuf, add);
	return (newbuf);
}