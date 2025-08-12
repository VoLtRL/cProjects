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

    FILE *ef = fopen(FILENAME ".txt", "a+");
    fprintf(ef, "%d;%s;Incomplete\n", nodeSize(Tasks) + 1, data);
    fclose(ef);

    if (Tasks == NULL)
    {
        Tasks = createNode(data);
    }
    else
    {
        Tasks = insertAtEnd(Tasks, data);
    }
    printf("Task added successfully!\n");
    free(data);
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
        printf("ID	Task\t\tStatus\n");
        printf("-------------------------------------------------\n");
        printList(Tasks);
    }
};

Node *completeTask(Node *Tasks)
{
    Node *elt = Tasks;
    int index;
    printf("Enter task ID to mark as complete: ");
    scanf("%d", &index);
    for (int i = 1; i < index; i++)
    {
        elt = elt->next;
    }
    elt->isCompleted = 1;

    FILE *temp = fopen("temp.txt", "w");
    Node *tempNode = Tasks;

    fprintf(temp, "ID	Task\t\tStatus\n");
    fprintf(temp, "-------------------------------------------------\n");
    for (int i = 1; i < nodeSize(Tasks) + 1; i++)
    {
        fprintf(temp, "%d;%s;%s\n", i, tempNode->data, (tempNode->isCompleted) ? "Complete" : "Incomplete");
        tempNode = tempNode->next;
    }
    fclose(temp);
    remove(FILENAME ".txt");
    rename("temp.txt", FILENAME ".txt");

    return Tasks;
}

Node *deleteTask(Node *Tasks)
{
    int index;
    printf("Enter task ID to delete: ");
    scanf("%d", &index);

    if (index > nodeSize(Tasks) || index < 1)
    {
        printf("This task ID doesnt exist.\nDelete cancelled.\n");
        return Tasks;
    }

    FILE *temp = fopen("temp.txt", "w");
    Node *tempNode = Tasks;

    fprintf(temp, "ID	Task\t\tStatus\n");
    fprintf(temp, "-------------------------------------------------\n");
    for (int i = 1; i < nodeSize(Tasks) + 1; i++)
    {
        if (i != index)
        {
            fprintf(temp, "%d;%s;%s\n", (i > index) ? i - 1 : i, tempNode->data, (tempNode->isCompleted) ? "Complete" : "Incomplete");
        }
        tempNode = tempNode->next;
    }
    fclose(temp);
    remove(FILENAME ".txt");
    rename("temp.txt", FILENAME ".txt");

    Tasks = delete(Tasks, index);

    return Tasks;
}

Node *loadTasks(Node *Tasks)
{
    FILE *ef = fopen(FILENAME ".txt", "a+");
    if (fgetc(ef) == EOF)
    {
        fprintf(ef, "ID	Task\t\tStatus\n");
        fprintf(ef, "-------------------------------------------------\n");
        printf("Created a new file named %s.txt\n", FILENAME);
    }
    else
    {
        rewind(ef);
        char line[100];
        while (fgets(line, 100, ef))
        {
            line[strcspn(line, "\n")] = 0;
            char *cuttedLine = strtok(line, ";");
            int count = 0;
            while (cuttedLine != NULL)
            {
                if (count == 1)
                {
                    printf("%s\n", cuttedLine);
                    if (Tasks == NULL)
                    {
                        Tasks = createNode(cuttedLine);
                    }
                    else
                    {
                        Tasks = insertAtEnd(Tasks, cuttedLine);
                    }
                }
                cuttedLine = strtok(NULL, ";");
                count++;
            }
        }
    }
    fclose(ef);
    return Tasks;
}

int main()
{

    int choice;
    int size = 0;
    Node *Tasks = NULL;

    // Create or Edit the defined file name
    Tasks = loadTasks(Tasks);

    printList(Tasks);

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

        default:
            break;
        }
    };
    return 0;
}
