#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

Node *createNode(char data[])
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = malloc(strlen(data) + 1);
    newNode->nodeSize = 1;
    strcpy(newNode->data, data);
    newNode->next = NULL;
    newNode->isCompleted = 0;

    return newNode;
}

Node *insertAtBeg(Node *head, char data[])
{
    Node *newNode = createNode(data);
    newNode->next = head;
    newNode->nodeSize += 1;

    return newNode;
}

Node *insertAtEnd(Node *head, char data[])
{
    Node *elt = head;
    Node *newNode = createNode(data);
    while (elt->next != NULL)
    {
        elt = elt->next;
    }

    elt->next = newNode;
    head->nodeSize += 1;

    return head;
}

Node *insert(Node *head, char data[], int index)
{
    if (index < 1)
    {
        return head;
    }

    if (index == 1)
    {
        return insertAtBeg(head, data);
    }

    Node *elt = head->next;

    for (int i = 2; i < index && elt != NULL; i++)
    {
        elt = elt->next;
    }
    if (elt == NULL)
    {
        return head;
    }

    Node *newNode = createNode(data);

    newNode->next = elt->next;
    elt->next = newNode;
    head->nodeSize += 1;

    return head;
}

Node *delete(Node *head, int index)
{
    if (head->nodeSize == 1)
    {
        return NULL;
    }
    Node *node = head;

    for (int i = 1; i < index - 1; i++)
    {
        node = node->next;
    }

    Node *start = node;
    Node *end = node->next->next;
    start->next = end;
    head->nodeSize -= 1;

    return head;
}

void printList(Node *node)
{
    int i = 1;

    while (node != NULL)
    {
        if (node->isCompleted)
        {
            printf("%d\t%s\tComplete\n", i, node->data);
        }
        else
        {
            printf("%d\t%s\tIncomplete\n", i, node->data);
        }
        node = node->next;
        i++;
    }
}
