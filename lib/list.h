#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

struct Node{
	char persona[2];
	struct Node * next;
};

struct Node * head = NULL;
struct Node * current = NULL;

int isEmpty(){
	if(head == NULL){
		return TRUE;
	}
	return FALSE;
}

void printList(){
	struct Node * current=head;
	
	if(isEmpty() == 1){
		printf("List is empty\n");
		return;
	}
	while(current != NULL){
		printf("%s ", current->persona);
		current = current->next;
	}
}

void add(char c[]){
	struct Node *temp=(struct Node*)malloc(sizeof(struct Node));
	strcpy(temp->persona,c);
	temp->next = head;
	head=temp;
}

void cancel(char c[]){
	struct Node *current=head;
	struct Node * previous=NULL;

	if(head == NULL){
		return;
	}

	while(strcmp(current->persona,c) != 0){
		if(current->next==NULL){
			return;
		}else{
			previous=current;
			current=current->next;
		}
	}

	if(current==head){
		head=head->next;
	}else{
		previous->next = current->next;
	}
	free(current);
}


