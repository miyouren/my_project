#include <head.h>
#include "myled.h"

int main(int argc, const char *argv[])
{
    int fd;
    int which;
    if ((fd = open("/dev/myled", O_RDWR)) == -1)
    {
        perror("open error");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // which = LED1;
        // ioctl(fd, LED_ON, &which);
        // sleep(1);
        // ioctl(fd, LED_OFF, &which);
        // sleep(1);
        ioctl(fd, LED1ON);
        usleep(200000);
        ioctl(fd, LED1OFF);
        ioctl(fd, LED2ON);
        usleep(200000);
        ioctl(fd, LED2OFF);
        ioctl(fd, LED3ON);
        usleep(200000);
        ioctl(fd, LED3OFF);
        
    }

    close(fd);
    return 0;
}

