// Copyright(C) 2018 Iti Shree

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

static uint32_t map_size = 0x08000000;
static uint32_t map_base = 0x18000000;
static uint32_t map_addr = 0x00000000;

static char *dev_mem = "/dev/mem";

int main(int argc, char **argv) {
    int fd;
    char *buf;
    if ((fd = open(dev_mem, O_RDWR | O_SYNC)) == -1) {
        printf("can't open /dev/mem .\n");
        exit(EXIT_FAILURE);
    }

    buf = mmap((char *) map_addr, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
               map_base);
    if (buf == (void *) -1) {
        printf("Can't be mapped. \n");
        exit(EXIT_FAILURE);
    } else
        map_addr = (long unsigned) buf;

    fwrite(buf, 1, map_size, stdout);


    //Fletcher's checksum algorithm

    register short int sum1 = 0;
    register unsigned long int sum2 = 0;

    while (map_size--) {
        sum1 += *buf++;
        if (sum1 >= 255)
            sum1 -= 255;

    }

    sum2 %= 255;

    printf("%lu\n", sum2);

    close(fd);

    return 0;
}
