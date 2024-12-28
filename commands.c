#include "ft_ssl.h"

void    add_to_global_hash(char* file, int type)
{
    s_hash* tmp = &global_hash;

    if (tmp->type > 0)
    {
        while (tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = (s_hash*)malloc(sizeof(s_hash));

        if (!tmp->next) //TODO
        {
            printf("malloc err\n");
            exit(1);
        }

        tmp = tmp->next;
    }

    tmp->type = type;
    tmp->file = file;

    tmp->hash = (unsigned char*)malloc(16);
    for (int i = 0; i < 16; i++)
        tmp->hash[i] = '\0';

    if (!tmp->hash) //TODO
    {
        printf("malloc err\n");
        exit(1);
    }
}

int is_flag_valid(int ac, char *av[])
{
    int     result = (ac == 3);
    s_hash* tmp = &global_hash;

    if (ft_strcmp("md5", av[1]) == 0)
    {
        global_flags.command = MD5;
        result = TRUE;
    }
    else if (ft_strcmp("sha256", av[1]) == 0)
    {
        global_flags.command = SHA256;
        result = TRUE;
    }
    else if (ft_strcmp("whirlpool", av[1]) == 0)
    {
        global_flags.command = WHIRLPOOL;
        result = TRUE;
    }
    else
    {
        write(2, "Unknown command: ", ft_strlen("Unknown command: "));
        write(2, av[1], ft_strlen(av[1]));
        exit_func("\nUsage: ./ft_ssl [command] [flags] [file/string]\nAllowed commands:\nmd5\tsha256\twhirlpool\n", 0);
    }

    for (int i = 2; i < ac; i++)
    {
        result = TRUE;

        if (ft_strcmp(av[i], "-p") == 0)
        {
            global_flags.flag_stdin = TRUE;

            //add_to_global_hash("stdin", TYPE_STDIN);

            char* buffer = (char*)malloc(1001);
            //char* msg = NULL;
            int bytes_read;

            write(STDOUT_FILENO, "> ", 2);

            bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
            if(bytes_read > 1000) exit(0); //TODO
            buffer[bytes_read - 1] = '\0';

            //while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
            //{
            //    buffer[bytes_read] = '\0';
            //    msg = str_join(msg, buffer);
            //}

            add_to_global_hash(buffer, TYPE_STDIN);
        }
        else if (ft_strcmp(av[i], "-q") == 0)
        {
            global_flags.flag_quiet = TRUE;
        }
        else if (ft_strcmp(av[i], "-r") == 0)
        {
            global_flags.flag_reverse = TRUE;
        }
        else if (ft_strcmp(av[i], "-s") == 0)
        {
            i++;
            if (i == ac)
                exit_func("Missing string\nUsage: ./ft_ssl [command] -s [string]\n", 0);

            if (ft_strlen(av[i]))
            {
                add_to_global_hash(av[i], TYPE_STRING);
            }
            else if (i + 1 != ac) //TODO
                exit_func("Empty string\nUsage: ./ft_ssl [command] -s [string]\n", 0);
        }
        else if (i + 1 == ac)
        {
            add_to_global_hash(av[i], TYPE_FILE);
        }
        else //TODO YAPILMADI
        {
            exit(1);
            result = FALSE;
            break;
        }
    }

    return result;
}

int is_valid_av(int ac, char *av[])
{
    if (ac < 3)
    {
        exit_func("Missing arguman\nUsage: ./ft_ssl [command] [flags] [file/string]\n", 0);
    }
    else if (is_flag_valid(ac, av) == FALSE)
    {
        exit_func("Flag error\nUsage: ./ft_ssl [command] [flags] [file/string]\nAllowed commands:\nmd5\tsha256\twhirlpool\n", 0);
    }
    return TRUE;
} 