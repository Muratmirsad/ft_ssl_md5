#include "ft_ssl.h"

void*   err_buffer[8];

s_flags global_flags = {
    .command = 0,
    .flag_stdin = FALSE,
    .flag_quiet = FALSE,
    .flag_reverse = FALSE
};

s_hash  global_hash = {
    .hash = NULL,
    .file = NULL,
    .type = NULL,
    .next = NULL
};

int main (int ac, char** av)
{
    is_valid_av(ac, av);

    if (global_flags.command == MD5)
    {
        ft_md5();
    }
    else if (global_flags.command == SHA256)
    {

    }
    else if (global_flags.command == WHIRLPOOL)
    {

    }

    write_hash();

    system("leaks ft_ssl");
}