#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include "ipc.h"
#include "algorithms.h"

void core_loop(IPCOps *);
void logger_loop(IPCOps *);

static int read_int(const char *prompt,int lo,int hi) {
    char b[64]; long x;
    printf("%s",prompt);fflush(stdout);
    if(!fgets(b,sizeof(b),stdin))return -1;
    char *e; x=strtol(b,&e,10);
    if(e==b||x<lo||x>hi)return -1;
    return (int)x;
}
static void run_once(IPCType type,int alg,int frame_count) {
    IPCOps ops;
    if(ipc_init(type,&ops)){perror("IPC initialization");return;}
    pid_t core=fork();
    if(core==0) { core_loop(&ops); _exit(0); }
    if(core<0){perror("fork core");ops.close_all();return;}
    pid_t logger=fork();
    if(logger==0) { logger_loop(&ops); _exit(0); }
    if(logger<0){perror("fork logger");kill(core,SIGTERM);waitpid(core,NULL,0);ops.close_all();return;}

    Message req={0};req.type=MSG_REQUEST;req.algorithm=alg;
    req.frame_count=frame_count;req.page_count=13;
    int sample[]={7,0,1,2,0,3,0,4,2,3,0,3,2};
    memcpy(req.pages,sample,sizeof(sample));
    if(ops.ui_to_core_send(&req))perror("ui send");
    Message result;
    if(!ops.core_to_ui_recv(&result)) {
        printf("\nIPC: %s | Algorithm: %s\n",ipc_name(type),algorithm_name(result.algorithm));
        printf("Page faults: %d | Hits: %d | Algorithm time: %.9f s\n",
               result.page_faults,result.hits,result.execution_seconds);
    }
    Message stop={0};stop.type=MSG_STOP;
    ops.ui_to_core_send(&stop);
    waitpid(core,NULL,0);
    waitpid(logger,NULL,0);
    ops.close_all();
}
int main(int argc,char **argv) {
    if(argc>1&&!strcmp(argv[1],"--benchmark")) {
        for(int t=1;t<=5;t++)run_once((IPCType)t,ALG_FIFO,3);
        return 0;
    }
    puts("=== Multi-Process Page Replacement Simulator ===");
    puts("1. POSIX Message Queues\n2. Pipes\n3. Shared Memory\n4. Unix Domain Sockets\n5. FIFO / Named Pipes");
    int t=read_int("Select IPC mechanism: ",1,5);
    if(t<1)return 1;
    puts("\nAlgorithms:\n1. FIFO\n2. LRU\n3. Optimal");
    int a=read_int("Select algorithm: ",1,3);
    if(a<1)return 1;
    int frames=read_int("Number of frames (1-64): ",1,64);
    if(frames<1)return 1;
    run_once((IPCType)t,a,frames);
    return 0;
}
