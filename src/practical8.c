#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE (100 * 1024 * 1024)

/* ------------------------------------------------ */
/* Dynamic Memory Allocation Demonstration          */
/* ------------------------------------------------ */

void memory_demo(void)
{
    int i;
    int *malloc_ptr;
    int *calloc_ptr;
    int *temp;

    /* ------- 1. malloc() ------- */

    printf("1. malloc() demonstration\n");

    malloc_ptr = malloc(5 * sizeof(int));

    if (malloc_ptr == NULL)
    {
        printf("malloc() failed\n");
        return;
    }

    for (i = 0; i < 5; i++)
    {
        malloc_ptr[i] = (i + 1) * 10;
    }

    printf("Memory allocated using malloc():\n");

    for (i = 0; i < 5; i++)
    {
        printf("%d ", malloc_ptr[i]);
    }

    printf("\n\n");

    /* ------- 2. calloc() ------- */

    printf("2. calloc() demonstration\n");

    calloc_ptr = calloc(5, sizeof(int));

    if (calloc_ptr == NULL)
    {
        printf("calloc() failed\n");
        free(malloc_ptr);
        return;
    }

    printf("Memory allocated using calloc():\n");

    for (i = 0; i < 5; i++)
    {
        printf("%d ", calloc_ptr[i]);
    }

    printf("\n\n");

    /* ------- 3. realloc() ------- */

    printf("3. realloc() demonstration\n");

    temp = realloc(malloc_ptr, 10 * sizeof(int));

    if (temp == NULL)
    {
        printf("realloc() failed\n");
        free(malloc_ptr);
        free(calloc_ptr);
        return;
    }

    malloc_ptr = temp;

    for (i = 5; i < 10; i++)
    {
        malloc_ptr[i] = (i + 1) * 10;
    }

    printf("Memory after realloc():\n");

    for (i = 0; i < 10; i++)
    {
        printf("%d ", malloc_ptr[i]);
    }

    printf("\n\n");

    /* ------- 4. free() ------- */

    printf("4. free() demonstration\n");

    free(malloc_ptr);
    malloc_ptr = NULL;

    free(calloc_ptr);
    calloc_ptr = NULL;

    printf("Allocated memory successfully released.\n");
}

/* ------------------------------------------------ */
/* Copy-on-Write Demonstration                      */
/* ------------------------------------------------ */

void cow_demo(void)
{
    char *data;
    pid_t pid;

    /* Allocate 100 MB */
    data = malloc(SIZE);

    if (data == NULL)
    {
        perror("malloc");
        return;
    }

    /* Initialize memory */
    for (size_t i = 0; i < SIZE; i++)
    {
        data[i] = 1;
    }

    printf("Parent PID: %d\n", getpid());
    printf("Allocated and initialized 100 MB.\n");
    printf("\nMemory initialized.\n");

    printf("Press Enter to perform fork()...");
    getchar();

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        free(data);
        return;
    }

    /* Child process */
    if (pid == 0)
    {
        printf("\nChild PID: %d\n", getpid());
        printf("Parent PID: %d\n", getppid());
        printf("Child has inherited the memory.\n");

        printf("Press Enter before modifying memory...");
        getchar();

        /*
         * Modify one byte in every 4 KB page.
         * This forces Copy-on-Write.
         */
        for (size_t i = 0; i < SIZE; i += 4096)
        {
            data[i] = 2;
        }

        printf("\nChild modified one byte in every 4 KB page.\n");

        printf("Press Enter to exit child...");
        getchar();

        free(data);

        return;
    }

    /* Parent process */
    else
    {
        printf("\nParent PID: %d\n", getpid());
        printf("Child PID : %d\n", pid);

        printf("Parent and child initially share physical pages using COW.\n");

        printf("Press Enter to let the child modify memory...");
        getchar();

        wait(NULL);

        printf("\nChild has terminated.\n");

        free(data);
    }
}

/* ------------------------------------------------ */
/* Main                                             */
/* ------------------------------------------------ */

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage:\n");
        printf("  %s memory\n", argv[0]);
        printf("  %s cow\n", argv[0]);

        return 1;
    }

    if (strcmp(argv[1], "memory") == 0)
    {
        memory_demo();
    }
    else if (strcmp(argv[1], "cow") == 0)
    {
        cow_demo();
    }
    else
    {
        printf("Invalid option.\n");
        printf("Use 'memory' or 'cow'.\n");

        return 1;
    }

    return 0;
}
