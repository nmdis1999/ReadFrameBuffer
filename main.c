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
    uint32_t *buf;
    if ((fd = open(dev_mem, O_RDWR | O_SYNC)) == -1) {
        printf("can't open /dev/mem .\n");
        exit(EXIT_FAILURE);
    }

    buf = mmap((uint32_t *) map_addr, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
               map_base);
    if (buf == (void *) -1) {
        printf("Can't be mapped. \n");
        exit(EXIT_FAILURE);
    } else
        map_addr = (long unsigned) buf;

    // fwrite(buf, 1, map_size, stdout);


    // Fletcher's 32 bit checksum algorithm

    uint32_t sum1 = 0xffff, sum2 = 0xffff;
    uint32_t tempLen;
    while (map_size) {
        tempLen = map_size >= 359 ? 359 : map_size;
        map_size -= tempLen;
        do {
            sum2 += sum1 += *buf++;
        } while (--tempLen);
        sum1 = (sum2 & 0xffff) + (sum1 >> 16);
        sum2 = (sum2 & 0xffff) + (sum2 >> 16);
    }

    sum1 = (sum1 & 0xffff) + (sum1 >> 16);
    sum2 = (sum2 & 0xffff) + (sum2 >> 16);

    uint32_t sum = sum2 << 16 | sum1;

    printf("%u", sum);

    close(fd);

    return 0;
}
