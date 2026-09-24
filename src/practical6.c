#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define SERVER_FIFO "/tmp/server_fifo"
#define MAX_MSG 256

typedef struct
{
    pid_t client_pid;
    char message[MAX_MSG];
} Request;

typedef struct
{
    char response[MAX_MSG];
} Response;

/* ---------------- SERVER ---------------- */

void run_server()
{
    int server_fd;
    Request request;
    char client_fifo[100];

    /* Create server FIFO */
    if (mkfifo(SERVER_FIFO, 0666) == -1 && errno != EEXIST)
    {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    printf("Server started...\n");
    printf("Waiting for clients...\n");

    /*
     * Open server FIFO for reading and writing.
     * O_RDWR prevents EOF when there are
     * temporarily no clients.
     */
    server_fd = open(SERVER_FIFO, O_RDWR);

    if (server_fd == -1)
    {
        perror("open");
        unlink(SERVER_FIFO);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        ssize_t n = read(server_fd, &request, sizeof(Request));

        if (n <= 0)
        {
            continue;
        }

        printf("\nReceived from Client PID %d: %s\n",
               request.client_pid,
               request.message);

        /* Create unique FIFO name for the client */
        snprintf(client_fifo,
                 sizeof(client_fifo),
                 "/tmp/client_%d_fifo",
                 request.client_pid);

        int client_fd = open(client_fifo, O_WRONLY);

        if (client_fd == -1)
        {
            perror("open client FIFO");
            continue;
        }

        Response response;

        /*
         * Limit the message length so the response
         * always fits inside the 256-byte buffer.
         */
        snprintf(response.response,
                 sizeof(response.response),
                 "Server processed: %.230s",
                 request.message);

        write(client_fd, &response, sizeof(response));

        close(client_fd);

        printf("Response sent to Client PID %d\n",
               request.client_pid);
    }

    close(server_fd);
    unlink(SERVER_FIFO);
}

/* ---------------- CLIENT ---------------- */

void run_client()
{
    int server_fd;
    int client_fd;

    char client_fifo[100];
    char message[MAX_MSG];

    Request request;
    Response response;

    pid_t pid = getpid();

    /* Create unique FIFO for this client */
    snprintf(client_fifo,
             sizeof(client_fifo),
             "/tmp/client_%d_fifo",
             pid);

    if (mkfifo(client_fifo, 0666) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    printf("Client PID: %d\n", pid);
    printf("Enter message: ");

    if (fgets(message, sizeof(message), stdin) == NULL)
    {
        printf("Unable to read message.\n");
        unlink(client_fifo);
        exit(EXIT_FAILURE);
    }

    /* Remove newline */
    message[strcspn(message, "\n")] = '\0';

    request.client_pid = pid;
    strcpy(request.message, message);

    /* Open server FIFO */
    server_fd = open(SERVER_FIFO, O_WRONLY);

    if (server_fd == -1)
    {
        perror("Unable to open server FIFO");
        unlink(client_fifo);
        exit(EXIT_FAILURE);
    }

    /* Send request to server */
    write(server_fd, &request, sizeof(request));

    close(server_fd);

    printf("Message sent to server.\n");

    /* Open client's FIFO for response */
    client_fd = open(client_fifo, O_RDONLY);

    if (client_fd == -1)
    {
        perror("Unable to open client FIFO");
        unlink(client_fifo);
        exit(EXIT_FAILURE);
    }

    /* Receive response */
    if (read(client_fd, &response, sizeof(response)) > 0)
    {
        printf("Server Response: %s\n",
               response.response);
    }

    close(client_fd);

    /* Remove client's FIFO */
    unlink(client_fifo);
}

/* ---------------- MAIN ---------------- */

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage:\n");
        printf("  %s server\n", argv[0]);
        printf("  %s client\n", argv[0]);

        return 1;
    }

    if (strcmp(argv[1], "server") == 0)
    {
        run_server();
    }
    else if (strcmp(argv[1], "client") == 0)
    {
        run_client();
    }
    else
    {
        printf("Invalid option.\n");
        printf("Use 'server' or 'client'.\n");

        return 1;
    }

    return 0;
}
