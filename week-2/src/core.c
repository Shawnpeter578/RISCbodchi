#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define REQUEST_QUEUE  "/riscbodchi_request"
#define RESPONSE_QUEUE "/riscbodchi_response"
#define LOG_QUEUE      "/riscbodchi_log"

#define MAX_MSG_SIZE 256

#define MEMORY_SIZE 100
#define STACK_SIZE 100
#define QUEUE_SIZE 100

int memory[MEMORY_SIZE];

int stack[STACK_SIZE];
int stack_top = -1;

void push(int value)
{
    if (stack_top >= STACK_SIZE - 1)
    {
        printf("CORE: Stack overflow\n");
        return;
    }

    stack_top++;
    stack[stack_top] = value;

    printf("CORE: PUSH %d\n", value);
}

int pop(void)
{
    if (stack_top < 0)
    {
        printf("CORE: Stack underflow\n");
        return -1;
    }

    int value = stack[stack_top];
    stack_top--;

    printf("CORE: POP %d\n", value);

    return value;
}

int queue[QUEUE_SIZE];

int queue_front = 0;
int queue_rear = -1;

void enqueue(int value)
{
    if (queue_rear >= QUEUE_SIZE - 1)
    {
        printf("CORE: Queue full\n");
        return;
    }

    queue_rear++;
    queue[queue_rear] = value;

    printf("CORE: ENQUEUE %d\n", value);
}

int dequeue(void)
{
    if (queue_front > queue_rear)
    {
        printf("CORE: Queue empty\n");
        return -1;
    }

    int value = queue[queue_front];
    queue_front++;

    printf("CORE: DEQUEUE %d\n", value);

    return value;
}

int cpu_add(int a, int b)
{
    return a + b;
}

int cpu_sub(int a, int b)
{
    return a - b;
}

int cpu_mul(int a, int b)
{
    return a * b;
}

int cpu_div(int a, int b)
{
    if (b == 0)
    {
        return 0;
    }

    return a / b;
}

void send_response(
    mqd_t response_queue,
    const char *message
)
{
    if (mq_send(
        response_queue,
        message,
        strlen(message) + 1,
        0
    ) == -1)
    {
        perror("CORE: Failed to send response");
    }
}

void send_log(
    mqd_t log_queue,
    const char *message
)
{
    if (mq_send(
        log_queue,
        message,
        strlen(message) + 1,
        0
    ) == -1)
    {
        perror("CORE: Failed to send log");
    }
}

int main(void)
{
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    mq_unlink(REQUEST_QUEUE);
    mq_unlink(RESPONSE_QUEUE);

    mqd_t request_queue = mq_open(
        REQUEST_QUEUE,
        O_CREAT | O_RDONLY,
        0666,
        &attr
    );

    if (request_queue == (mqd_t)-1)
    {
        perror("CORE: Cannot create request queue");
        return 1;
    }

    mqd_t response_queue = mq_open(
        RESPONSE_QUEUE,
        O_CREAT | O_WRONLY,
        0666,
        &attr
    );

    if (response_queue == (mqd_t)-1)
    {
        perror("CORE: Cannot create response queue");

        mq_close(request_queue);
        mq_unlink(REQUEST_QUEUE);

        return 1;
    }

    mqd_t log_queue = mq_open(
        LOG_QUEUE,
        O_WRONLY
    );

    if (log_queue == (mqd_t)-1)
    {
        perror("CORE: Cannot connect to logger");

        printf("CORE: Make sure LOGGER is running first.\n");

        mq_close(request_queue);
        mq_close(response_queue);

        mq_unlink(REQUEST_QUEUE);
        mq_unlink(RESPONSE_QUEUE);

        return 1;
    }

    printf("\n");
    printf("====================================\n");
    printf("         RISCbodchi CORE\n");
    printf("====================================\n");

    printf("CPU     : READY\n");
    printf("Memory  : READY\n");
    printf("Stack   : READY\n");
    printf("Queue   : READY\n");
    printf("Logger  : CONNECTED\n");

    printf("====================================\n");

    printf("CORE: Waiting for commands...\n");

    char message[MAX_MSG_SIZE];

    while (1)
    {
        ssize_t bytes = mq_receive(
            request_queue,
            message,
            MAX_MSG_SIZE,
            NULL
        );

        if (bytes == -1)
        {
            perror("CORE: mq_receive");
            break;
        }

        message[bytes] = '\0';

        printf("\nCORE received: %s\n", message);

        if (strcmp(message, "exit") == 0)
        {
            send_response(
                response_queue,
                "Core shutting down"
            );

            send_log(
                log_queue,
                "Core shutting down"
            );

            break;
        }

        int a;
        int b;

        if (sscanf(
            message,
            "ADD %d %d",
            &a,
            &b
        ) == 2)
        {
            int result = cpu_add(a, b);

            char response[MAX_MSG_SIZE];

            snprintf(
                response,
                sizeof(response),
                "ADD result = %d",
                result
            );

            send_response(
                response_queue,
                response
            );

            char log_message[MAX_MSG_SIZE];

            snprintf(
                log_message,
                sizeof(log_message),
                "ADD %d %d = %d",
                a,
                b,
                result
            );

            send_log(
                log_queue,
                log_message
            );

            continue;
        }

        if (sscanf(
            message,
            "SUB %d %d",
            &a,
            &b
        ) == 2)
        {
            int result = cpu_sub(a, b);

            char response[MAX_MSG_SIZE];

            snprintf(
                response,
                sizeof(response),
                "SUB result = %d",
                result
            );

            send_response(
                response_queue,
                response
            );

            char log_message[MAX_MSG_SIZE];

            snprintf(
                log_message,
                sizeof(log_message),
                "SUB %d %d = %d",
                a,
                b,
                result
            );

            send_log(
                log_queue,
                log_message
            );

            continue;
        }

        if (sscanf(
            message,
            "MUL %d %d",
            &a,
            &b
        ) == 2)
        {
            int result = cpu_mul(a, b);

            char response[MAX_MSG_SIZE];

            snprintf(
                response,
                sizeof(response),
                "MUL result = %d",
                result
            );

            send_response(
                response_queue,
                response
            );

            char log_message[MAX_MSG_SIZE];

            snprintf(
                log_message,
                sizeof(log_message),
                "MUL %d %d = %d",
                a,
                b,
                result
            );

            send_log(
                log_queue,
                log_message
            );

            continue;
        }

        if (sscanf(
            message,
            "DIV %d %d",
            &a,
            &b
        ) == 2)
        {
            if (b == 0)
            {
                send_response(
                    response_queue,
                    "ERROR: Division by zero"
                );

                send_log(
                    log_queue,
                    "ERROR: Division by zero"
                );
            }
            else
            {
                int result = cpu_div(a, b);

                char response[MAX_MSG_SIZE];

                snprintf(
                    response,
                    sizeof(response),
                    "DIV result = %d",
                    result
                );

                send_response(
                    response_queue,
                    response
                );

                char log_message[MAX_MSG_SIZE];

                snprintf(
                    log_message,
                    sizeof(log_message),
                    "DIV %d %d = %d",
                    a,
                    b,
                    result
                );

                send_log(
                    log_queue,
                    log_message
                );
            }

            continue;
        }

        int value;

        if (sscanf(
            message,
            "PUSH %d",
            &value
        ) == 1)
        {
            push(value);

            send_response(
                response_queue,
                "Value pushed to stack"
            );

            char log_message[MAX_MSG_SIZE];

            snprintf(
                log_message,
                sizeof(log_message),
                "PUSH %d",
                value
            );

            send_log(
                log_queue,
                log_message
            );

            continue;
        }

        if (strcmp(message, "POP") == 0)
        {
            int result = pop();

            char response[MAX_MSG_SIZE];

            snprintf(
                response,
                sizeof(response),
                "POP result = %d",
                result
            );

            send_response(
                response_queue,
                response
            );

            char log_message[MAX_MSG_SIZE];

            snprintf(
                log_message,
                sizeof(log_message),
                "POP -> %d",
                result
            );

            send_log(
                log_queue,
                log_message
            );

            continue;
        }

        if (sscanf(
            message,
            "ENQUEUE %d",
            &value
        ) == 1)
        {
            enqueue(value);

            send_response(
                response_queue,
                "Value added to queue"
            );

            char log_message[MAX_MSG_SIZE];

            snprintf(
                log_message,
                sizeof(log_message),
                "ENQUEUE %d",
                value
            );

            send_log(
                log_queue,
                log_message
            );

            continue;
        }

        if (strcmp(message, "DEQUEUE") == 0)
        {
            int result = dequeue();

            char response[MAX_MSG_SIZE];

            snprintf(
                response,
                sizeof(response),
                "DEQUEUE result = %d",
                result
            );

            send_response(
                response_queue,
                response
            );

            char log_message[MAX_MSG_SIZE];

            snprintf(
                log_message,
                sizeof(log_message),
                "DEQUEUE -> %d",
                result
            );

            send_log(
                log_queue,
                log_message
            );

            continue;
        }

        int address;

        if (sscanf(
            message,
            "STORE %d %d",
            &address,
            &value
        ) == 2)
        {
            if (
                address >= 0 &&
                address < MEMORY_SIZE
            )
            {
                memory[address] = value;

                send_response(
                    response_queue,
                    "Value stored in memory"
                );

                char log_message[MAX_MSG_SIZE];

                snprintf(
                    log_message,
                    sizeof(log_message),
                    "STORE Memory[%d] = %d",
                    address,
                    value
                );

                send_log(
                    log_queue,
                    log_message
                );
            }
            else
            {
                send_response(
                    response_queue,
                    "ERROR: Invalid memory address"
                );

                send_log(
                    log_queue,
                    "ERROR: Invalid memory address"
                );
            }

            continue;
        }

        if (sscanf(
            message,
            "LOAD %d",
            &address
        ) == 1)
        {
            if (
                address >= 0 &&
                address < MEMORY_SIZE
            )
            {
                char response[MAX_MSG_SIZE];

                snprintf(
                    response,
                    sizeof(response),
                    "Memory[%d] = %d",
                    address,
                    memory[address]
                );

                send_response(
                    response_queue,
                    response
                );

                char log_message[MAX_MSG_SIZE];

                snprintf(
                    log_message,
                    sizeof(log_message),
                    "LOAD Memory[%d] -> %d",
                    address,
                    memory[address]
                );

                send_log(
                    log_queue,
                    log_message
                );
            }
            else
            {
                send_response(
                    response_queue,
                    "ERROR: Invalid memory address"
                );

                send_log(
                    log_queue,
                    "ERROR: Invalid memory address"
                );
            }

            continue;
        }

        send_response(
            response_queue,
            "ERROR: Unknown command"
        );

        char log_message[MAX_MSG_SIZE];

        snprintf(
            log_message,
            sizeof(log_message),
            "ERROR: Unknown command -> %.220s",
            message
        );

        send_log(
            log_queue,
            log_message
        );
    }

    mq_close(request_queue);
    mq_close(response_queue);
    mq_close(log_queue);

    mq_unlink(REQUEST_QUEUE);
    mq_unlink(RESPONSE_QUEUE);

    printf("\nCORE: Shutdown complete.\n");

    return 0;
}