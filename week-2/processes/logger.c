#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include "ipc.h"

void logger_loop(IPCOps *ops) {
    mkdir("logs",0755);
    FILE *f=fopen("logs/simulator.log","a");
    if(!f)_exit(3);
    Message m;
    for (;;) {
        if(ops->logger_recv(&m))break;
        if(m.type==MSG_STOP)break;
        time_t now=time(NULL);
        fprintf(f,"%s | %s\n",ctime(&now),m.text);
        fflush(f);
    }
    fclose(f);
}
