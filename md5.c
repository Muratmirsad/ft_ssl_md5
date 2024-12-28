#include "ft_ssl.h"

//TODO 512bit bloklar olarak ayarlanacak
//TODO son 64 bit veri uzunlugu olacak

//? on ek
uint32_t A, B, C, D;
// sola cevirme
#define LEFT_ROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

// md islemleri
#define F(B, C, D) (((B) & (C)) | (~(B) & (D)))
#define G(B, C, D) (((B) & (D)) | ((C) & ~(D)))
#define H(B, C, D) ((B) ^ (C) ^ (D))
#define I(B, C, D) ((C) ^ ((B) | ~(D)))


// T[i] = floor(2^32 × ∣sin(i)∣)
// Tablolar ve Döndürme Miktarları
static const uint32_t T[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a,
    0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340,
    0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
    0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa,
    0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
    0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static const uint32_t S[64] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};


void process_block(void* block)
{
    uint32_t M[16];
    uint32_t AA = A, BB = B, CC = C, DD = D;
    uint32_t F_val, g, temp;

    // Blok verisini 16 adet 32-bit kelimeye böl
    ft_memcpy(M, block, 64);

    // 64 adımda hashing işlemi
    for (int i = 0; i < 64; i++)
    {

        if (i < 16)
        {
            F_val = F(BB, CC, DD);
            g = i;
        }
        else if (i < 32)
        {
            F_val = G(BB, CC, DD);
            g = (5 * i + 1) % 16;
        }
        else if (i < 48)
        {
            F_val = H(BB, CC, DD);
            g = (3 * i + 5) % 16;
        }
        else
        {
            F_val = I(BB, CC, DD);
            g = (7 * i) % 16;
        }

        temp = DD;
        DD = CC;
        CC = BB;
        BB = BB + LEFT_ROTATE((AA + F_val + M[g] + T[i]), S[i]);
        AA = temp;
    }

    // Başlangıç değerlerini güncelle
    A += AA;
    B += BB;
    C += CC;
    D += DD;
}

void pad_data(void* data, int read_bytes)
{
    unsigned char* buffer = (unsigned char*)data;
    uint64_t bit_length = read_bytes * 8;

    buffer[read_bytes] = 0x80; // İlk bit
    ft_memset(buffer + read_bytes + 1, 0, BUFFER_SIZE - read_bytes - 1 - 8);
    ft_memcpy(buffer + BUFFER_SIZE - 8, &bit_length, sizeof(bit_length));
}

void   ft_md5()
{
    unsigned int    len;
    int             fd;
    int             read_bytes;
    void            *buffer;
    unsigned char   *hash;
    s_hash*         tmp;

    buffer = malloc(BUFFER_SIZE);
    tmp = &global_hash;

    if (!buffer) //!
    {
        exit_func("malloc failed for buffer\n", 0);
    }

    while (TRUE)
    {
        A = 0x67452301;
        B = 0xEFCDAB89;
        C = 0x98BADCFE;
        D = 0x10325476;
    
        if (tmp->type == TYPE_FILE)
        {
            fd = open(tmp->file, O_RDONLY);

            if (fd == -1)
            {
                //exit_func("File could not be opened\n", 0); //TODO

                write(2, "File could not be opened: ", ft_strlen("File could not be opened: "));
                write(2, global_hash.file, ft_strlen(global_hash.file));
                write(2, "\n", 1);

                if (tmp->next == NULL)
                    break;
                else
                    tmp = tmp->next;
                
                continue;
            }

            while ((read_bytes = read(fd, buffer, BUFFER_SIZE)) > 0)
            {
                if (read_bytes < BUFFER_SIZE)
                {
                    pad_data(buffer, read_bytes);
                }

                process_block(buffer);

                if (read_bytes < BUFFER_SIZE)
                    break;
            }
        }
        else if (tmp->type == TYPE_STRING || tmp->type == TYPE_STDIN)
        {
            len = ft_strlen(tmp->file); // file yerine global_flags.flag_string kullanabilirsin eger bug olursa

            if (len > BUFFER_SIZE)
            {
                read_bytes = 0;
                while (len > read_bytes)
                {
                    read_bytes += ft_strlen(tmp->file + read_bytes);
                    ft_memcpy(buffer, tmp->file, read_bytes);

                    if (read_bytes % BUFFER_SIZE)
                    {
                        pad_data(buffer, read_bytes);
                    }
                    
                    process_block(buffer);
                }
            }
            else
            {
                ft_memcpy(buffer, tmp->file, len);
                pad_data(buffer, len);
                process_block(buffer);
            }
        }

        tmp->hash = malloc(16);

        if (!tmp->hash)
        {
            err_buffer[0] = buffer;
            exit_func("malloc failed for hash\n", 1);
        }

        ft_memcpy(tmp->hash, &A, 4);
        ft_memcpy(tmp->hash + 4, &B, 4);
        ft_memcpy(tmp->hash + 8, &C, 4);
        ft_memcpy(tmp->hash + 12, &D, 4);

        if (tmp->next == NULL)
            break;
        else
            tmp = tmp->next;
    }

    // hash = malloc(16);

    // if (!hash)
    // {
    //     err_buffer[0] = buffer;
    //     exit_func("malloc failed for hash\n", 1);
    // }

    // ft_memcpy(hash, &A, 4);
    // ft_memcpy(hash + 4, &B, 4);
    // ft_memcpy(hash + 8, &C, 4);
    // ft_memcpy(hash + 12, &D, 4);


    //! ------------------------------------------
    // char* hash_hex = malloc(33);

    // for (int i = 0; i < 16; i++) {
    // sprintf(hash_hex + (i * 2), "%02x", hash[i]);
    // }
    // hash_hex[32] = '\0'; // Null terminator
    // printf("Computed MD5: %s\n", hash_hex);


    free(buffer);
    return hash;
}