#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define FILE_NAME(i) (i) 

int main()
{
	FILE *fp;

	int i;
 	for(i=0;i<5;i++){

	char buf[10] = "abcdef";

 	char file_num[5];
 	sprintf(file_num,"pic_%d.txt",i);
 	printf(file_num);
 	printf("\n");
 	fp = fopen(FILE_NAME(file_num),"w");
	fwrite(buf,sizeof(int),10,fp);
 	fclose(fp);
 	}
 	printf("Hello\n");
 	return 0;
 }

