#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define BUFFER_SIZE 1024

typedef struct Node {
	char* data;
	struct Node* next;
}Node;

typedef struct List {
	Node* head;
	size_t size;
}List;

List* createList() {
	List* list = malloc(sizeof(List));
	if (list == NULL) {
		return NULL;
	}
	list->head = NULL;
	list->size = 0;
	return list;
}

Node* createNode(char* str) {
	Node* node = malloc(sizeof(Node));
	if (node == NULL) {
		return NULL;
	}
	node->next = NULL;
	size_t length = strlen(str);
	node->data = strdup(str);
	return node;
}

void pushBack(List* list, Node* node) {
	if (list->size == 0) {
		list->size = 1;
		list->head = node;
		return;
	}
	Node* tmp = createNode("");
	tmp = list->head;
	while (tmp->next != NULL) {
		tmp = tmp->next;
	}
	tmp->next = node;
	list->size++;
	return;
}

void printList(List* list) {
	Node* t = list->head;
	while (t != NULL) {
		printf("%s", t->data);
		t = t->next;
	}
}

void removeList(List* list) {
	Node* tmp = createNode("");
	Node* p = createNode("");
	tmp = list->head;
	while (tmp != NULL) {
		p = tmp;
		tmp = tmp->next;
		free(p->data);
		free(p);
		list->head = tmp;
	}
	free(list);
}

int main() {
	List* list = createList();
	if (list == NULL) {
		perror("allocation error\n");
		return 2;
	}
	char buffer[BUFFER_SIZE];

	while (1) {
		if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
			perror("reading error\n");
			removeList(list);
			return 1;
		}
		fflush(stdin);
		if (buffer[0] == '.') {
			break;
		}
		Node *node = createNode(buffer);
		if (node == NULL) {
			perror("allocation error\n");
			removeList(list);
			return 2;
		}
		pushBack(list, node);
	}

	printList(list);
	removeList(list);
	return 0;
}
