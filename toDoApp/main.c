#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

#define FILENAME "Tasks"

Node *addTask(Node *Tasks)
{
    char *data = malloc(100 * sizeof(char));
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    printf("Enter task name: ");
    fgets(data, 100, stdin);
    data[strcspn(data, "\n")] = '\0';

    if (Tasks == NULL)
    {
        Tasks = createNode(data);
    }
    else
    {
        Tasks = insertAtEnd(Tasks, data);
    }
    printf("Task added successfully!\n");
    return Tasks;
};

void displayTasks(Node *Tasks)
{
    if (Tasks == NULL)
    {
        printf("There is nothing to show. Start by adding a task\n");
    }
    else
    {
        printf("ID	Task		Status\n");
        printf("-------------------------------------------------\n");
        printList(Tasks);
    }
};

Node *completeTask(Node *Tasks)
{
    Node *elt = Tasks;
    int index;
    printf("Enter task ID to mark as complete: ");
    scanf("%i", &index);
    for (int i = 1; i < index; i++)
    {
        elt = elt->next;
    }
    elt->isCompleted = 1;

    return Tasks;
}

Node *deleteTask(Node *Tasks)
{
    int index;
    printf("Enter task ID to delete: ");
    scanf("%d", &index);

    if (index > Tasks->nodeSize || index < 1)
    {
        printf("This task ID doesnt exist.\nDelete cancelled.\n");
        return Tasks;
    }

    Tasks = delete(Tasks, index);

    return Tasks;
}

int main()
{

    int choice;
    int size = 0;
    Node *Tasks = NULL;

    // Create or Edit the defined file name

    FILE *ef = fopen(FILENAME ".txt", "a+");
    if (fgetc(ef) == EOF)
    {
        printf("Created a new file named %s.txt\n", FILENAME);
        printf("TIP : Don't forget to exit the c program at the end of your modifications or it will not update the file.");
        fprintf(ef, "ID	Task		Status\n");
        fprintf(ef, "-------------------------------------------------\n");
    }

    for (;;)
    {
        printf("-------------------------------------------------\n");
        printf("ToDoApp CLI \n");
        printf("1. Add Task\n");
        printf("2. Display Tasks\n");
        printf("3. Mark Task as Complete\n");
        printf("4. Delete Task\n");
        printf("5. Exit (also save the %s.txt file)\n", FILENAME);

        printf("Enter your choice: ");
        scanf("%i", &choice);

        switch (choice)
        {
        case 1:
            Tasks = addTask(Tasks);
            break;

        case 2:
            displayTasks(Tasks);
            break;

        case 3:
            Tasks = completeTask(Tasks);
            break;

        case 4:

            Tasks = deleteTask(Tasks);
            break;

        case 5:
            exit(EXIT_SUCCESS);
            fclose(ef);

        default:
            break;
        }
    };
    return 0;
}
