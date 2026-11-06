#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define REQUEST_QUEUE  "/riscbodchi_request"
#define RESPONSE_QUEUE "/riscbodchi_response"
#define LOG_QUEUE      "/riscbodchi_log"

#define MAX_MESSAGE 256
#define STACK_SIZE 100
#define QUEUE_SIZE 100
#define MEMORY_SIZE 100

int memory[MEMORY_SIZE];
int stack[STACK_SIZE];
int stack_top = -1;
int cpu_queue[QUEUE_SIZE];
int queue_front = 0;
int queue_rear = -1;

void cpu_push(int value) {
    if (stack_top >= STACK_SIZE - 1) return;
    stack[++stack_top] = value;
}

int cpu_pop(void) {
    if (stack_top < 0) return -1;
    return stack[stack_top--];
}

void cpu_enqueue(int value) {
    if (queue_rear >= QUEUE_SIZE - 1) return;
    cpu_queue[++queue_rear] = value;
}

int cpu_dequeue(void) {
    if (queue_front > queue_rear) return -1;
    return cpu_queue[queue_front++];
}

int cpu_add(int a, int b) { return a + b; }
int cpu_subtract(int a, int b) { return a - b; }

void send_response(mqd_t response_queue, const char *message) {
    mq_send(response_queue, message, strlen(message) + 1, 0);
}

void send_log(mqd_t log_queue, const char *message) {
    mq_send(log_queue, message, strlen(message) + 1, 0);
}

void process_command(char *command, mqd_t response_queue, mqd_t log_queue) {
    int a, b, value, address;
    char response[MAX_MESSAGE], log_message[MAX_MESSAGE];

    if (sscanf(command, "ADD %d %d", &a, &b) == 2) {
        int result = cpu_add(a, b);
        snprintf(response, MAX_MESSAGE, "ADD result = %d", result);
        snprintf(log_message, MAX_MESSAGE, "ADD %d %d = %d", a, b, result);
        send_response(response_queue, response); send_log(log_queue, log_message);
        return;
    }
    if (sscanf(command, "SUBTRACT %d %d", &a, &b) == 2) {
        int result = cpu_subtract(a, b);
        snprintf(response, MAX_MESSAGE, "SUBTRACT result = %d", result);
        snprintf(log_message, MAX_MESSAGE, "SUBTRACT %d %d = %d", a, b, result);
        send_response(response_queue, response); send_log(log_queue, log_message);
        return;
    }
    if (sscanf(command, "PUSH %d", &value) == 1) {
        cpu_push(value);
        snprintf(log_message, MAX_MESSAGE, "PUSH %d", value);
        send_response(response_queue, "Value pushed to stack"); send_log(log_queue, log_message);
        return;
    }
    if (strcmp(command, "POP") == 0) {
        int result = cpu_pop();
        snprintf(response, MAX_MESSAGE, "POP result = %d", result);
        snprintf(log_message, MAX_MESSAGE, "POP -> %d", result);
        send_response(response_queue, response); send_log(log_queue, log_message);
        return;
    }
    if (sscanf(command, "ENQUEUE %d", &value) == 1) {
        cpu_enqueue(value);
        snprintf(log_message, MAX_MESSAGE, "ENQUEUE %d", value);
        send_response(response_queue, "Value added to queue"); send_log(log_queue, log_message);
        return;
    }
    if (strcmp(command, "DEQUEUE") == 0) {
        int result = cpu_dequeue();
        snprintf(response, MAX_MESSAGE, "DEQUEUE result = %d", result);
        snprintf(log_message, MAX_MESSAGE, "DEQUEUE -> %d", result);
        send_response(response_queue, response); send_log(log_queue, log_message);
        return;
    }
    if (sscanf(command, "STORE %d %d", &address, &value) == 2) {
        if (address >= 0 && address < MEMORY_SIZE) {
            memory[address] = value;
            snprintf(log_message, MAX_MESSAGE, "STORE memory[%d] = %d", address, value);
            send_response(response_queue, "Value stored in memory"); send_log(log_queue, log_message);
        } else {
            send_response(response_queue, "ERROR: Invalid memory address"); send_log(log_queue, "ERROR: Invalid memory address");
        }
        return;
    }
    if (sscanf(command, "LOAD %d", &address) == 1) {
        if (address >= 0 && address < MEMORY_SIZE) {
            snprintf(response, MAX_MESSAGE, "Memory[%d] = %d", address, memory[address]);
            snprintf(log_message, MAX_MESSAGE, "LOAD memory[%d] -> %d", address, memory[address]);
            send_response(response_queue, response); send_log(log_queue, log_message);
        } else {
            send_response(response_queue, "ERROR: Invalid memory address"); send_log(log_queue, "ERROR: Invalid memory address");
        }
        return;
    }
    
    send_response(response_queue, "ERROR: Unknown command");
    snprintf(log_message, MAX_MESSAGE, "ERROR: Unknown command -> %.200s", command);
    send_log(log_queue, log_message);
}

int main(void) {
    struct mq_attr attributes = { .mq_flags = 0, .mq_maxmsg = 10, .mq_msgsize = MAX_MESSAGE, .mq_curmsgs = 0 };

    mq_unlink(REQUEST_QUEUE);
    mq_unlink(RESPONSE_QUEUE);

    mqd_t request_queue = mq_open(REQUEST_QUEUE, O_CREAT | O_RDONLY, 0666, &attributes);
    mqd_t response_queue = mq_open(RESPONSE_QUEUE, O_CREAT | O_WRONLY, 0666, &attributes);
    mqd_t log_queue = mq_open(LOG_QUEUE, O_WRONLY);

    char command[MAX_MESSAGE];
    while (1) {
        ssize_t received = mq_receive(request_queue, command, MAX_MESSAGE, NULL);
        if (received == -1) break;
        command[received] = '\0';
        
        if (strcmp(command, "exit") == 0) {
            send_response(response_queue, "Core shutting down");
            send_log(log_queue, "Core shutting down");
            break;
        }
        process_command(command, response_queue, log_queue);
    }

    mq_close(request_queue); mq_close(response_queue); mq_close(log_queue);
    mq_unlink(REQUEST_QUEUE); mq_unlink(RESPONSE_QUEUE);
    return 0;
}
