#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Global variables */
int global_var = 100;
int global_uninitialized;

/* Static variable */
static int static_var = 200;

/* Function - Code/Text segment */
void display_code_address(void)
{
    printf("Code address       : %p\n",
           (void *)display_code_address);
}

/* ---------------- ADDRESS PART ---------------- */

void address_demo(void)
{
    /* Local variable - Stack */
    int stack_var = 30;

    /* Dynamic memory - Heap */
    int *heap_var = malloc(sizeof(int));

    if (heap_var == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    *heap_var = 40;

    printf("===== Linux Process Address Space =====\n\n");

    /* Code/Text segment */
    display_code_address();

    /* Data segment */
    printf("Global address     : %p\n",
           (void *)&global_var);

    /* Static variable */
    printf("Static address     : %p\n",
           (void *)&static_var);

    /* BSS segment */
    printf("BSS address        : %p\n",
           (void *)&global_uninitialized);

    /* Heap */
    printf("Heap address       : %p\n",
           (void *)heap_var);

    /* Stack */
    printf("Stack address      : %p\n",
           (void *)&stack_var);

    free(heap_var);
}

/* ---------------- MEMORY MAP PART ---------------- */

void memory_demo(void)
{
    /* Local variable - Stack */
    int stack_var = 300;

    /* Dynamic memory - Heap */
    int *heap_var = malloc(sizeof(int));

    if (heap_var == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    *heap_var = 400;

    printf("===== Linux Process Memory Map Demo =====\n\n");

    printf("Process ID (PID): %d\n", getpid());

    /* Code/Text segment */
    printf("Address of code   : %p\n",
           (void *)display_code_address);

    /* Data segment */
    printf("Address of global : %p\n",
           (void *)&global_var);

    /* Static variable */
    printf("Address of static : %p\n",
           (void *)&static_var);

    /* BSS segment */
    printf("Address of BSS    : %p\n",
           (void *)&global_uninitialized);

    /* Heap */
    printf("Address of heap   : %p\n",
           (void *)heap_var);

    /* Stack */
    printf("Address of stack  : %p\n",
           (void *)&stack_var);

    printf("\nProcess is running...\n");
    printf("Open another terminal and run:\n");
    printf("cat /proc/%d/maps\n", getpid());

    /* Keep process running for memory inspection */
    while (1)
    {
        sleep(10);
    }

    free(heap_var);
}

/* ---------------- MAIN ---------------- */

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage:\n");
        printf("  %s address\n", argv[0]);
        printf("  %s memory\n", argv[0]);

        return 1;
    }

    if (strcmp(argv[1], "address") == 0)
    {
        address_demo();
    }
    else if (strcmp(argv[1], "memory") == 0)
    {
        memory_demo();
    }
    else
    {
        printf("Invalid option.\n");
        printf("Use 'address' or 'memory'.\n");

        return 1;
    }

    return 0;
}
