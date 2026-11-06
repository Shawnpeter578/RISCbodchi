#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

int main() {
    printf("========================================\n");
    printf("        RISC-bodchi OS Simulator        \n");
    printf("========================================\n");
    printf("Booting IPC processes...\n");
    sleep(2);

    pid_t logger_pid = fork();
    if (logger_pid == 0) {
        int log_fd = open("simulator.log", O_WRONLY | O_CREAT | O_TRUNC, 0666);
        dup2(log_fd, STDOUT_FILENO);
        close(log_fd);
        execl("./build/logger", "./build/logger", NULL);
        perror("Failed to start logger");
        exit(1);
    }

    pid_t ui_pid = fork();
    if (ui_pid == 0) {
        execl("./build/ui", "./build/ui", "./build/core", NULL);
        perror("Failed to start UI");
        exit(1);
    }

    waitpid(ui_pid, NULL, 0);

    kill(logger_pid, SIGTERM);
    waitpid(logger_pid, NULL, 0);
    
    printf("\nSimulator shutdown complete.\n");
    printf("Execution logs have been saved to 'simulator.log'.\n");
    return 0;
}
