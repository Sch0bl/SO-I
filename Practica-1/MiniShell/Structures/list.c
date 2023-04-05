#include "list.h"
#include <stdlib.h>
#include <string.h>

struct List list_create(){
	struct List new_list;
	new_list.first = NULL;
	new_list.last = NULL;
	new_list.len = 0;
	return new_list;
}

struct List list_add(struct List list, char** token, Order pos){
	struct Node* new_node = malloc(sizeof(struct Node));
	new_node->token = *token;
	if (list.first == NULL){
		new_node->next = NULL;
		list.first = list.last = new_node;
		return list;
	}
	if (pos == HEAD){
		new_node->next = list.first;
		list.first = new_node;
	} else {
		new_node->next = NULL;
		list.last->next = new_node;
		list.last = new_node;
	}
	list.len++;
	return list;
}

void list_destroy_aux(struct Node* node){
	struct Node* node_tbd = node;
	for(struct Node* aux = node_tbd->next; aux != NULL; aux = aux->next){
		free(node_tbd);
		node_tbd = aux;
	}
	free(node_tbd);
}

void list_destroy(struct List* list){
	if(list->first == NULL)
		return;
	list_destroy_aux(list->first);
	list->first = list->last = NULL;
}

char** list_to_cmd(struct List list){
	if (list.len == 0){
		return NULL;
	}
	struct Node* node = list.first;
	char** cmd = malloc(sizeof(char*) * (list.len + 1));
	for(int i = 0;i < list.len; i++){
		cmd[i] = node->token;
		node = node->next;
	}
	return cmd;
}


