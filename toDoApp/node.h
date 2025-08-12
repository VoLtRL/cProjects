#ifndef NODE_H
#define NODE_H

typedef struct Node
{
    char *data;
    int isCompleted;
    struct Node *next;
} Node;

Node *createNode(char data[]);

Node *insertAtBeg(Node *head, char data[]);

Node *insertAtEnd(Node *head, char data[]);

Node *insert(Node *head, char data[], int index);

Node *delete(Node *head, int index);

int nodeSize(Node *head);

char *nodeData(Node *head, int index);

void printList(Node *node);

#endif