#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/* Flags modified by signal handlers */
volatile sig_atomic_t sigint_received = 0;
volatile sig_atomic_t sigterm_received = 0;
volatile sig_atomic_t sigusr1_received = 0;

/* SIGINT handler */
void handle_sigint(int signal)
{
    (void)signal;
    sigint_received = 1;
}

/* SIGTERM handler */
void handle_sigterm(int signal)
{
    (void)signal;
    sigterm_received = 1;
}

/* SIGUSR1 handler */
void handle_sigusr1(int signal)
{
    (void)signal;
    sigusr1_received = 1;
}

int main()
{
    struct sigaction sa_int;
    struct sigaction sa_term;
    struct sigaction sa_usr1;

    /* SIGINT */
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;

    /* SIGTERM */
    sa_term.sa_handler = handle_sigterm;
    sigemptyset(&sa_term.sa_mask);
    sa_term.sa_flags = 0;

    /* SIGUSR1 */
    sa_usr1.sa_handler = handle_sigusr1;
    sigemptyset(&sa_usr1.sa_mask);
    sa_usr1.sa_flags = 0;

    /* Register signal handlers */
    if (sigaction(SIGINT, &sa_int, NULL) == -1)
    {
        perror("sigaction SIGINT");
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGTERM, &sa_term, NULL) == -1)
    {
        perror("sigaction SIGTERM");
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGUSR1, &sa_usr1, NULL) == -1)
    {
        perror("sigaction SIGUSR1");
        exit(EXIT_FAILURE);
    }

    printf("Signal Handler Program Started\n");
    printf("Process ID: %d\n", getpid());
    printf("Waiting for signals...\n");

    while (1)
    {
        pause();

        if (sigusr1_received)
        {
            printf("SIGUSR1 received.\n");
            sigusr1_received = 0;
        }

        if (sigint_received)
        {
            printf("SIGINT received.\n");
            sigint_received = 0;
        }

        if (sigterm_received)
        {
            printf("SIGTERM received.\n");
            printf("Gracefully terminating...\n");
            break;
        }
    }

    printf("Program terminated.\n");

    return 0;
}
