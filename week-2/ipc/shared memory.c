#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include "ipc.h"

/* Three one-slot shared-memory channels, each guarded by empty/full semaphores.
   Educational implementation: one producer and one consumer per channel. */
typedef struct { sem_t empty,full; Message msg; } Slot;
static Slot *slots[3]; static int fds[3];
static const char *names[3]={"/w4sim_shm_uc","/w4sim_shm_cu","/w4sim_shm_cl"};
static int put(int k,const Message*m){sem_wait(&slots[k]->empty);slots[k]->msg=*m;sem_post(&slots[k]->full);return 0;}
static int get(int k,Message*m){sem_wait(&slots[k]->full);*m=slots[k]->msg;sem_post(&slots[k]->empty);return 0;}
static int ucs(const Message*m){return put(0,m);}static int ucr(Message*m){return get(0,m);}
static int cus(const Message*m){return put(1,m);}static int cur(Message*m){return get(1,m);}
static int cls(const Message*m){return put(2,m);}static int clr(Message*m){return get(2,m);}
static void close_shm(void) {
    for (int i = 0; i < 2; i++) {
        if (fds[i] >= 0) {
            close(fds[i]);
        }
        shm_unlink(names[i]);
    }
}
int shm_backend_init(IPCOps*o){
    for(int i=0;i<3;i++){
        shm_unlink(names[i]);
        fds[i]=shm_open(names[i],O_CREAT|O_RDWR,0600);if(fds[i]<0)return -1;
        if(ftruncate(fds[i],sizeof(Slot)))return -1;
        slots[i]=mmap(NULL,sizeof(Slot),PROT_READ|PROT_WRITE,MAP_SHARED,fds[i],0);
        if(slots[i]==MAP_FAILED)return -1;
        memset(slots[i],0,sizeof(Slot));
        if(sem_init(&slots[i]->empty,1,1)||sem_init(&slots[i]->full,1,0))return -1;
    }
    o->ui_to_core_send=ucs;o->ui_to_core_recv=ucr;
    o->core_to_ui_send=cus;o->core_to_ui_recv=cur;
    o->core_to_log_send=cls;o->logger_recv=clr;o->close_all=close_shm;return 0;
}
