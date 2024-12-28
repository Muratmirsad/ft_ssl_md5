#ifndef FT_SSL_H
#define FT_SSL_H

#include <stdint.h>
#include <stddef.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 64
#define TRUE 1
#define FALSE 0
#define MD5 1
#define SHA256 2
#define WHIRLPOOL 3
#define TYPE_FILE 1
#define TYPE_STRING 2
#define TYPE_STDIN 3

typedef struct t_flags
{
    int     command; // 1 = md5 | 2 = sha256 | 3 = whirlpool
    int     flag_stdin; // -p
    int     flag_quiet; // -q
    int     flag_reverse; // -r

} s_flags;

typedef struct t_hash
{
    unsigned char*  hash;
    char*           file;
    int             type; // 1 = file | 2 = -s flag | 3 = stdin
    char            err_msg[100]; //! unnecessary
    struct t_hash*  next;
} s_hash;


extern s_flags  global_flags;
extern s_hash   global_hash;
extern void*    err_buffer[8];


//                          *****     md5     *****

void    ft_md5();


//                          *****     libft     *****

void	*ft_memset(void *str, int c, size_t len);
void	*ft_memcpy(void *dst, const void *src, size_t n);
size_t	ft_strlen(const char *str);
int     ft_strcmp(const char *str1, const char *str2);


//                          *****     utils     *****

void	exit_func(char *msg, int err_buffer_leng);
int     is_valid_av(int ac, char *av[]);
void    write_hash(void);
char    *str_join(char *buf, char *add);


#endif // FT_SSL_H