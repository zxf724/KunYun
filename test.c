#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	const char *p = strcat("pic","i");
	int dest_file;

	if((dest_file = open("./picture/p",O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) == -1)
                {
                        printf("open jpg file error\n");
                }
	printf("the end!!");
}
