//Parent
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSort(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void toString(char str[], int num)
{
    sprintf(str, "%d", num);
}

int main()
{
    printf("\nThis is the main process");
    printf("\nProcess Id: %d", getpid());
    printf("\nParent Id: %d\n", getppid());

    int arr[] = {10, 5, 1, 60, 20};
    int n = 5;

    printf("Current Array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    printf("Sorting the array using bubble sort...\n");
    bubbleSort(arr, n);

    printf("Sorted array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n\nForking the current process...\n");

    pid_t cpid = fork();
    if (cpid > 0) // Parent process
    {
        printf("\nParent is running:\nParent Id: %d\nIts Id: %d\n", getppid(), getpid());
        printf("Parent is waiting for the child to finish...\n\n");
        wait(NULL);
        printf("Parent is exiting\n");
    }
    else if (cpid == 0) // Child process
    {
        printf("\nChild is running:\nParent Id: %d\nIts Id: %d\n", getppid(), getpid());

        char *arrChar[n + 2];
        arrChar[0] = (char *)"child";

        for (int i = 0; i < n; i++)
        {
            char *string = malloc(20 * sizeof(char));
            toString(string, arr[i]);
            arrChar[i + 1] = string;
        }
        arrChar[n + 1] = NULL;

        printf("Child process calling execv()...\n");
        execv("./child", arrChar);

        // This part executes only if execv fails
        perror("execv failed");
        exit(1);
    }
    else // Fork failed
    {
        perror("Fork failed");
        exit(1);
    }

    return 0;
}





//Child
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("No data received from parent process.\n");
        return 1;
    }

    int *data = (int *)malloc((argc - 1) * sizeof(int));
    if (!data)
    {
        perror("malloc failed");
        return 1;
    }

    printf("\nArgc: %d\n", argc);

    for (int i = 1; i < argc; i++)
    {
        data[i - 1] = atoi(argv[i]);
    }

    printf("Printing elements in reverse order: ");
    for (int i = argc - 2; i >= 0; i--)
    {
        printf("%d ", data[i]);
    }
    printf("\nEXECV task completed\n");

    free(data);
    return 0;
}
