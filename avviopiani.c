#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void main(){
	int pid0 = fork();
	if(pid0 == 0){
		execl("piano", "piano", "0" , (char*)NULL);
	}else{
		int pid1 = fork();
		if(pid1 == 0){
			execl("piano", "piano", "1" , (char*)NULL);
		}else{
			int pid2 = fork();
			if(pid2 == 0){
				execl("piano", "piano", "2" , (char*)NULL);
			}else{
				int pid3 = fork();
				if(pid3 == 0){
					execl("piano", "piano", "3" , (char*)NULL);
				}
			}
		}
	}
		
}
