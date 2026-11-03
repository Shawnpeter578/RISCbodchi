#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <time.h>

#define REQUEST_QUEUE "/riscbodchi_request"
#define RESPONSE_QUEUE "/riscbodchi_response"
#define MAX_MSG_SIZE 256

int main(void)
{
    mqd_t request_queue;
    mqd_t response_queue;

    request_queue = mq_open(REQUEST_QUEUE, O_WRONLY);

    if (request_queue == (mqd_t)-1)
    {
        perror("Cannot open request queue");
        return 1;
    }

    response_queue = mq_open(RESPONSE_QUEUE, O_RDONLY);

    if (response_queue == (mqd_t)-1)
    {
        perror("Cannot open response queue");
        mq_close(request_queue);
        return 1;
    }

    FILE *file = fopen("commands.txt", "r");

    if (file == NULL)
    {
        perror("Cannot open commands.txt");
        mq_close(request_queue);
        mq_close(response_queue);
        return 1;
    }

    char command[MAX_MSG_SIZE];
    char response[MAX_MSG_SIZE];

    int command_count = 0;

    struct timespec start, end;

    printf("\n====================================\n");
    printf("      RISCbodchi Benchmark\n");
    printf("====================================\n");

    clock_gettime(CLOCK_MONOTONIC, &start);

    while (fgets(command, sizeof(command), file))
    {
        command[strcspn(command, "\n")] = '\0';

        if (strlen(command) == 0)
            continue;

        if (mq_send(
            request_queue,
            command,
            strlen(command) + 1,
            0
        ) == -1)
        {
            perror("mq_send");
            fclose(file);
            mq_close(request_queue);
            mq_close(response_queue);
            return 1;
        }

        ssize_t bytes = mq_receive(
            response_queue,
            response,
            MAX_MSG_SIZE,
            NULL
        );

        if (bytes == -1)
        {
            perror("mq_receive");
            fclose(file);
            mq_close(request_queue);
            mq_close(response_queue);
            return 1;
        }

        response[bytes] = '\0';

        command_count++;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    fclose(file);

    double elapsed =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("Commands executed : %d\n", command_count);
    printf("Execution time     : %.6f seconds\n", elapsed);
    printf("Average per command: %.6f ms\n",
           (elapsed * 1000.0) / command_count);

    printf("====================================\n");

    mq_close(request_queue);
    mq_close(response_queue);

    return 0;
}
