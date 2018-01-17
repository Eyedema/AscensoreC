#include <stdio.h>
#include <time.h>

void setData(char data[]){
	time_t timer;
	char buffer[26];
	struct tm* tm_info;
	
	time(&timer);
	tm_info=localtime(&timer);
	
	strftime(buffer,26, "%d/%m/%Y %H:%M:%S", tm_info);
	sprintf(data,"%s",buffer);
}
