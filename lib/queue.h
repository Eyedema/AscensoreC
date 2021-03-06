#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

struct Node{
	char persona[2];
	struct Node* next;
};

struct Node* front = NULL; //puntatore alla testa della coda
struct Node* rear = NULL; //puntatore all'ultimo elemento della coda

void enqueue(char c[]){
	struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
	strcpy(temp->persona,c);
	temp->next = NULL;
	if(front == NULL && rear == NULL){
		front = rear = temp;
		return;
	}
	rear->next = temp;
	rear = temp;
}

void dequeue(){
	if(front == NULL){
		return;
	}
	if(front == rear){
		front = rear = NULL;
	}
	else{
		front = front -> next;
	}
}

int isEmpty(){
	if(front == NULL){
		return TRUE;
	}
	return FALSE;
}

void printQueue(){
	struct Node* temp = front;
	if(front == NULL){
		printf("coda vuota\n");
	}
	while(temp != NULL){
		printf("%s ",temp->persona);
		temp = temp->next;
	}
	printf("\n");
}
		
