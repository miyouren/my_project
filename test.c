#include <head.h>  // arm-linux-gnueabihf-gcc test.c -I /usr/include 
int main(int argc, const char *argv[])
{
    int fd;
    char buf[3] = {0};
    if ((fd = open("/dev/01led", O_RDWR)) == -1)
    {
        perror("open error");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
		bzero(buf,sizeof(buf));
        printf("input 0(1off),1(1on),2(2on),3(2off),4(3on),5(3off) > ");
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf) - 1] = '\0';

        write(fd, buf, sizeof(buf));
    }

    close(fd);
    return 0;
}
