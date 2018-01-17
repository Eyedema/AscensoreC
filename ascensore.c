#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "lib/list.h"
#include "lib/readLine.h"
#include "lib/socket.h"
#include "lib/data.h"

#define TRUE 1
#define FALSE 0
#define PESOMAX 300
#define ALTO 0
#define BASSO 1

int piano=0; //piano corrente
int direzione=0; //direzione in cui muoversi
int pesoDisponibile=300;
char socketName[14]; //nome della socket
void nuovoPiano(int pianoCorrente); //calcola il nuovo piano di destinazione
void stampaResume(); //stampa sul file e sul terminale il riepilogo dell'attività dell'ascensore
void riceviPersone(int fd); //legge le persone dalla socket inviate dal piano
int getPeso(char c[]); //restituisce il peso di una persona
void rimuoviPersone(char p); //rimuove le persone che scendono dall'ascensore
void arrivoPiano(); //si connette al piano
int controlloPiano(); //controlla che ci siano ancora persona in coda in un qualche piano
void stampaSalita(char p,char d); //stampa sul terminale e sul file di log le persone salite a bordo dell'ascensore
void stampaDiscesa(char p); //stampa sul terminale e sul file di log le persone scese dall'ascensore
FILE *logAsc; //puntatore al file di log
int iTotale,iBambini,iAdulti,iConsegna=0; //rispettivamente persone trasportate in totale, i bambini, gli adulti e gli addetti alla consegna
int partenza=FALSE; //se false ascensore fermo
int finePiano[4]={FALSE,FALSE,FALSE,FALSE}; //un valore per ogni piano, se false significa che ci sono ancora persone in attesa

int main(){

		logAsc = fopen("log_Ascensore.txt","a");
		
		if(logAsc==NULL){
			perror("errore in apertura");
			exit(1); 
		}

		while(partenza==FALSE){
			int fd = creaSocketClient("piano0_server");
			write(fd,"here",5);
			char p[200];
			readLine(fd,p);
			if(strcmp(p,"go")==0){
				partenza=TRUE;
				printf("[AVVIO]\n");
			}
			close(fd);	
		}	
		while(1){
				
		 	char p[20];
			sprintf(socketName,"piano%d_server",piano);		
			int fd = creaSocketClient(socketName);	
			arrivoPiano();
			write(fd,"here",5);
			rimuoviPersone(piano+'0');
			sleep(3);
			if(controlloPiano()==TRUE){
				write(fd,"exit",5);
				close(fd);
				break;
			}else{
				sprintf(p,"%d %d",piano,pesoDisponibile);			
				write(fd,p,strlen(p)+1);			
				riceviPersone(fd);
				nuovoPiano(piano);
				sleep(3);
			}
			close(fd);
		}
		stampaResume();
		fclose(logAsc);
		return;		
}

void nuovoPiano(int pianoCorrente){
	if(direzione == ALTO){
		if(piano == 3){
			direzione = BASSO;
			piano--;
		}else{
			piano++;
		}
		}else{
			if(piano==0){
				direzione=ALTO;
				piano++;
			}else{
				piano--;
		}
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

void riceviPersone(int fd){
	while(1){
		char p[200];
		readLine(fd,p);
		if(strcmp(p,"xx")==0){
			break;
		}else if(strcmp(p,"eof")==0){
			finePiano[piano]=TRUE;
			break;
		}else{
			add(p);
			stampaSalita(p[0],p[1]-'0');
			pesoDisponibile = pesoDisponibile - getPeso(p);	
		}
	}
}

void rimuoviPersone(char p){
	struct Node * current=head;
	
	while(current != NULL){
		char temp[20];
		strcpy(temp,current->persona);
		if(temp[1]==p){
			cancel(current->persona);
			stampaDiscesa(temp[0]);
			pesoDisponibile = pesoDisponibile + getPeso(temp);
			current = current->next;
		}else{
			current = current->next;
		}
	}
}

void arrivoPiano(){
	char data[20];
	setData(data);
	printf("[FERMATA] Piano %d, %s\n", piano,data);
	fprintf(logAsc,"[FERMATA] Piano %d, %s\n", piano,data);
}

void stampaSalita(char p,char d){
	char data[20];
	setData(data);
	switch(p){
		case 'B': 
			printf("[SALITO A BORDO] Bambino al piano %d, %s, Destinazione=%d\n", piano, data, d);
			fprintf(logAsc,"[SALITO A BORDO] Bambino al piano %d, %s, Destinazione=%d\n", piano, data, d);
			break;
		case 'A': 
			printf("[SALITO A BORDO] Adulto al piano %d, %s, Destinazione=%d\n", piano, data, d);
			fprintf(logAsc,"[SALITO A BORDO] Adulto al piano %d, %s, Destinazione=%d\n", piano, data, d);
			break;	
		case 'C': 
			printf("[SALITO A BORDO] Adetto alla consegna al piano %d, %s, Destinazione=%d\n", piano, data, d);
			fprintf(logAsc,"[SALITO A BORDO] Adetto alla consegna al piano %d, %s, Destinazione=%d\n", piano, data, d);
			break;
	}
}

void stampaDiscesa(char p){
	char data[20];
	setData(data);
	switch(p){
		case 'B': 
			printf("[SCESO] Bambino al piano %d, %s\n", piano, data); 
			fprintf(logAsc,"[SCESO] Bambino al piano %d, %s\n", piano, data);
			iTotale++;
			iBambini++; 
			break;
		case 'A': 
			printf("[SCESO] Adulto al piano %d, %s\n", piano, data);
			fprintf(logAsc,"[SCESO] Adulto al piano %d, %s\n", piano, data);
			iTotale++;
			iAdulti++; 
			break;	
		case 'C': 
			printf("[SCESO] Adetto alla consegna al piano %d, %s\n", piano, data);
			fprintf(logAsc,"[SCESO] Adetto alla consegna al piano %d, %s\n", piano, data);
			iTotale++;
			iConsegna++; 
			break;
	}
}

void stampaResume(){
	char term[]="Terminazione esecuzione!\nRiassunto dell'attività odierna:\n";
	char tot[]="Numero totale di persone trasportate=";
	char b[]="Numero bambini trasportati=";
	char a[]="Numero adulti trasportati=";
	char c[]="Numero addetti alla consegna trasportati=";
	char totale[200];
	char bambini[200];
	char adulti[200];
	char consegna[200];
	sprintf(totale,"%s%d\n",tot,iTotale);
	sprintf(bambini,"%s%d\n",b,iBambini);
	sprintf(adulti,"%s%d\n",a,iAdulti);
	sprintf(consegna,"%s%d\n",c,iConsegna);
	printf("%s%s%s%s%s",term,totale,bambini,adulti,consegna);
	fprintf(logAsc,"%s%s%s%s%s",term,totale,bambini,adulti,consegna);
}

int controlloPiano(){
	int i;
	for(i=0;i<4;i++){
		if(finePiano[i]==FALSE){
			return FALSE;
		}
	}
	if(isEmpty()==TRUE){
		return TRUE;
	}
	return FALSE;
}		
