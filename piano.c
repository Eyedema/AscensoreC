#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "lib/socket.h"
#include "lib/queue.h"
#include "lib/readLine.h"
#include "lib/data.h"

#define FALSE 0
#define TRUE 1

char *piano; //indica il numero del piano
void operazioneClient(); //codice relativo al processo piano_client
void operazioneServer(Socket s); //codice relativo al processo piano_client
void comunicazioneServerAscensore(int pesoDisponibile,int fd); //eseguita dal piano_server per comunicare con l'ascensore
void generaPersona(char type,char date[],int d,FILE *f); //stampa sul log l'arrivo di una persona al piano
void stampaPersona(char type,char date[],int d); //stampa sul terminale l'arrivo di una persona al piano
void comunicazioneServerClient(int fd,char msg[]); //eseguita dal piano_server per comunicare con il relativo piano_client
int getPeso(char c[]); //restituisce il peso di una persona
int start=TRUE; //se true significa ascensore avviato
int endFile=FALSE; //se true significa che è stato scansionato interamente il file piano.txt



int main(int argc, char *argv[]){
	piano=argv[1];
	if(strcmp(piano,"0")==0){
		start=FALSE;
	}
	char name[14];
	sprintf(name,"piano%s_server",piano);
	Socket s = creaSocketServer(name);
	
	int pid = fork();
	if(pid==0){ //piano client
		operazioneClient();
	} else if (pid== -1) {
		perror("fork");
		exit(1);
	}else{ //piano server
		operazioneServer(s);
	}
}

////PIANO CLIENT

void operazioneClient(){
	FILE *dati;
	char data[26];
	int slp,dest,temp=0;
	char tipo;
	char logName[20];
	char dataName[7];
	char socketName[14];
	FILE *log;
	sprintf(socketName,"piano%s_server",piano);
	sprintf(logName,"piano%s_log.txt",piano);
	sprintf(dataName,"piano%s",piano);
	log = fopen(logName,"a");
	if(log==NULL){
		perror("errore in apertura");
		exit(1); 
	}
	dati = fopen(dataName,"r");
	if(dati==NULL){
		perror("errore in apertura");
		exit(1); 
	}
				

	while(fscanf(dati,"%c %d %d\n",&tipo,&slp,&dest)>=0){
		sleep(slp-temp);
		int fd = creaSocketClient(socketName);
		temp=slp;
		setData(data);
		generaPersona(tipo,data,dest,log);
		char pers[2];
		sprintf(pers,"%c%d",tipo,dest);
		write(fd,pers,3);
		close(fd);					
	}
	int newfd = creaSocketClient(socketName);
	fclose(log);
	fclose(dati);
	write(newfd,"term",5);
	close(newfd);		
	printf("[TERMINAZIONE] esecuzione piano%s_client\n",piano);
	exit(0);
}

////PIANO SERVER

void operazioneServer(Socket s){
	Socket b = s;
	printf("Piano %s pronto!\n",piano);
	while(1){
		int ClientFd = accept(b.fd,b.sockAddrPtr,&b.len);
		char msg[20];
		readLine (ClientFd, msg);
		if(strcmp(msg,"here")==0 && start==FALSE){
			if(isEmpty()==FALSE){
				write(ClientFd,"go",3);
				start=TRUE;
				close(ClientFd);
			}else{
				write(ClientFd,"wait",5);
				close(ClientFd);
			}	
		}else if(strcmp(msg,"here")==0 && start==TRUE){
			printf("[ARRIVO] al piano %s\n",piano);
			int pesoDisponibile;
			int piano;
			char msg3[20];
			readLine (ClientFd, msg3);
			if(strcmp(msg3,"exit")==0){
				printf("[TERMINAZIONE] esecuzione piani server\n");
				killpg(0,SIGINT);
			}else{
				sscanf(msg3,"%d %d",&piano,&pesoDisponibile);
				comunicazioneServerAscensore(pesoDisponibile,ClientFd);
			}
		}else{	
			comunicazioneServerClient(ClientFd,msg);
		}
	}
}

void generaPersona(char type,char date[],int d,FILE *f){
	switch(type){
		case 'B': 
			fprintf(f,"[GENERATO] Bambino, %s, Destinazione=%d\n", date, d);
			break;
		case 'A': 
			fprintf(f,"[GENERATO] Adulto, %s, Destinazione=%d\n", date, d); 			
			break;	
		case 'C': 
			fprintf(f,"[GENERATO] Addetto alla consegna, %s, Destinazione=%d\n", date, d);  
			break;
	}	
}

void stampaPersona(char type,char date[],int d){
	switch(type){
		case 'B': 
			printf("[GENERATO] Bambino al piano %s, %s, Destinazione=%d\n", piano, date, d); 
			break;
		case 'A': 
			printf("[GENERATO] Adulto al piano %s, %s, Destinazione=%d\n", piano, date, d); 
			break;	
		case 'C': 
			printf("[GENERATO] Adetto alla consegna al piano %s, %s, Destinazione=%d\n", piano, date, d); 
			break;
	}
}

void comunicazioneServerClient(int fd,char msg[]){
	if(strcmp(msg,"term")==0){
		endFile=TRUE;
		close(fd);	
		return;
	}else{
		char data[200];
		setData(data);
		stampaPersona(msg[0],data,msg[1]-'0');
		enqueue(msg);
		close(fd);
	}
	
}

void comunicazioneServerAscensore(int pesoDisponibile,int fd){
	int pesoAttuale = pesoDisponibile;
	if(endFile==TRUE && isEmpty()==TRUE){//file scansionato e coda vuota
		write(fd,"eof",4);
		close(fd);
		return;
	} 
	if(isEmpty()==FALSE){ //controlla che ci sia almeno una persona in coda
		int pesoTesta = getPeso(front->persona);
		while(pesoTesta < pesoAttuale){
			pesoAttuale=pesoAttuale-pesoTesta;
			char pers[2];
			strcpy(pers,front->persona);
			write(fd,pers,3);
			dequeue();
			if(isEmpty()==TRUE){
				break;
			}else{
				pesoTesta = getPeso(front->persona);
			}
		}
		write(fd,"xx",3);
		close(fd);
	}else{
		write(fd,"xx",3);
		close(fd);
	}
}

int getPeso(char c[]){
	char p=c[0];
	switch(p){
		case 'B': return 40; break;
		case 'A': return 80; break;
		case 'C': return 90; break; 
	}
}
