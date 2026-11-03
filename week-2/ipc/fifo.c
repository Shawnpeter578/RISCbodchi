#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ipc.h"
static int uc[2],cu[2],cl[2];
static const char *paths[3]={"/tmp/w4sim_fifo_uc","/tmp/w4sim_fifo_cu","/tmp/w4sim_fifo_cl"};
static int wr(int fd,const Message*m){return write(fd,m,sizeof(*m))==(ssize_t)sizeof(*m)?0:-1;}
static int rd(int fd,Message*m){return read(fd,m,sizeof(*m))==(ssize_t)sizeof(*m)?0:-1;}
static int ucs(const Message*m){return wr(uc[1],m);}static int ucr(Message*m){return rd(uc[0],m);}
static int cus(const Message*m){return wr(cu[1],m);}static int cur(Message*m){return rd(cu[0],m);}
static int cls(const Message*m){return wr(cl[1],m);}static int clr(Message*m){return rd(cl[0],m);}
static void closef(void){for(int i=0;i<2;i++){close(uc[i]);close(cu[i]);close(cl[i]);}for(int i=0;i<3;i++)unlink(paths[i]);}
int fifo_backend_init(IPCOps*o){
    for(int i=0;i<3;i++){unlink(paths[i]);if(mkfifo(paths[i],0600))return -1;}
    /* Open each FIFO read/write to avoid startup deadlock; processes share descriptors. */
    uc[0]=open(paths[0],O_RDWR);uc[1]=uc[0];
    cu[0]=open(paths[1],O_RDWR);cu[1]=cu[0];
    cl[0]=open(paths[2],O_RDWR);cl[1]=cl[0];
    if(uc[0]<0||cu[0]<0||cl[0]<0)return -1;
    o->ui_to_core_send=ucs;o->ui_to_core_recv=ucr;o->core_to_ui_send=cus;o->core_to_ui_recv=cur;
    o->core_to_log_send=cls;o->logger_recv=clr;o->close_all=closef;return 0;
}
