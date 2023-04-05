#ifndef __LIST__
#define __LIST__

typedef enum {
	HEAD,
	TAIL
} Order;

struct Node{
	char* token;
	struct Node* next;
};

struct List{
	struct Node* first;
	struct Node* last;
	int len;
};

typedef struct List* Tokens;

struct List list_create();

struct List list_add(struct List list, char** token, Order pos);

void list_destroy_aux(struct Node* node);

char** list_to_cmd(struct List list);


#endif
