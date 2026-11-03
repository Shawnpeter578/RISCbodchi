#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define REQUEST_QUEUE  "/riscbodchi_request"
#define RESPONSE_QUEUE "/riscbodchi_response"

#define MAX_MSG_SIZE 256


int main(void)
{
    /* =========================
       CONNECT TO REQUEST QUEUE
       ========================= */

    mqd_t request_queue = mq_open(
        REQUEST_QUEUE,
        O_WRONLY
    );

    if (request_queue == (mqd_t)-1)
    {
        perror("UI: Cannot connect to Core");

        printf("UI: Make sure CORE is running first.\n");

        return 1;
    }


    /* =========================
       CONNECT TO RESPONSE QUEUE
       ========================= */

    mqd_t response_queue = mq_open(
        RESPONSE_QUEUE,
        O_RDONLY
    );

    if (response_queue == (mqd_t)-1)
    {
        perror("UI: Cannot connect to response queue");

        mq_close(request_queue);

        return 1;
    }


    /* =========================
       UI HEADER
       ========================= */

    printf("\n");
    printf("====================================\n");
    printf("        RISCbodchi UI\n");
    printf("====================================\n");

    printf("Commands:\n");

    printf("ADD a b\n");
    printf("SUB a b\n");
    printf("MUL a b\n");
    printf("DIV a b\n");

    printf("PUSH value\n");
    printf("POP\n");

    printf("ENQUEUE value\n");
    printf("DEQUEUE\n");

    printf("STORE address value\n");
    printf("LOAD address\n");

    printf("exit\n");

    printf("====================================\n");


    char message[MAX_MSG_SIZE];
    char response[MAX_MSG_SIZE];


    /* =========================
       UI LOOP
       ========================= */

    while (1)
    {
        printf("\nUI > ");

        fflush(stdout);


        if (fgets(
            message,
            sizeof(message),
            stdin
        ) == NULL)
        {
            break;
        }


        /* Remove newline */

        message[
            strcspn(message, "\n")
        ] = '\0';


        /* Ignore empty input */

        if (strlen(message) == 0)
        {
            continue;
        }


        /* =========================
           SEND COMMAND TO CORE
           ========================= */

        if (mq_send(
            request_queue,
            message,
            strlen(message) + 1,
            0
        ) == -1)
        {
            perror("UI: Failed to send command");
            break;
        }


        /* =========================
           WAIT FOR CORE RESPONSE
           ========================= */

        ssize_t bytes = mq_receive(
            response_queue,
            response,
            MAX_MSG_SIZE,
            NULL
        );

        if (bytes == -1)
        {
            perror("UI: Failed to receive response");
            break;
        }


        response[bytes] = '\0';


        printf("CORE: %s\n", response);


        /* =========================
           EXIT
           ========================= */

        if (strcmp(message, "exit") == 0)
        {
            break;
        }
    }


    /* =========================
       CLEANUP
       ========================= */

    mq_close(request_queue);
    mq_close(response_queue);

    printf("\nUI: Shutdown complete.\n");

    return 0;
}
