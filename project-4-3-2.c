#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define PAGE_SIZE 4096

int main(int argc, char *argv[])
{
    int fd;
    int i;
    unsigned char *p_map;
    
    /* open proc file */
    fd = open("/proc/mydir/myinfo", O_RDWR);
    if(fd < 0) {
        printf("open fail\n");
        exit(1);
    } else {
        printf("open successfully by Ugochukwu Agamu\n");
    }
    
    // map p_map to the proc file and grant read & write privilege
    p_map = (unsigned char *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p_map == MAP_FAILED) {
        printf("mmap failed\n");
        close(fd);
        exit(1);
    }
    
    // read data from p_map
    for (i = 0; i < 12; i++) {
        printf("%d\n", p_map[i]);
    }
    
    printf("Printed by Ugochukwu Agamu\n");
    
    // unmap p_map from the proc file
    munmap(p_map, PAGE_SIZE);
    close(fd);
    
    return 0;
}
