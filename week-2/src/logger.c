#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define LOG_QUEUE "/riscbodchi_log"
#define MAX_MSG_SIZE 256

int main(void)
{
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    // Remove old queue so it gets recreated with the correct size
    mq_unlink(LOG_QUEUE);

    mqd_t log_queue = mq_open(
        LOG_QUEUE,
        O_CREAT | O_RDONLY,
        0666,
        &attr
    );

    if (log_queue == (mqd_t)-1)
    {
        perror("LOGGER: mq_open");
        return 1;
    }

    FILE *log_file = fopen("simulator.log", "a");

    if (log_file == NULL)
    {
        perror("LOGGER: fopen");

        mq_close(log_queue);
        mq_unlink(LOG_QUEUE);

        return 1;
    }

    printf("\n");
    printf("====================================\n");
    printf("       RISCbodchi LOGGER\n");
    printf("====================================\n");
    printf("LOGGER: Waiting for messages...\n");

    char message[MAX_MSG_SIZE];

    while (1)
    {
        ssize_t bytes = mq_receive(
            log_queue,
            message,
            MAX_MSG_SIZE,
            NULL
        );

        if (bytes == -1)
        {
            perror("LOGGER: mq_receive");
            break;
        }

        message[bytes] = '\0';

        printf("LOG: %s\n", message);

        fprintf(log_file, "%s\n", message);
        fflush(log_file);

        if (strcmp(message, "Core shutting down") == 0)
        {
            break;
        }
    }

    fclose(log_file);

    mq_close(log_queue);
    mq_unlink(LOG_QUEUE);

    printf("LOGGER: Shutdown complete.\n");

    return 0;
}
