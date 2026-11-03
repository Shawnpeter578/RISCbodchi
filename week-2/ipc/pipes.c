#include <unistd.h>
#include "ipc.h"
static int uc[2],cu[2],cl[2];
static int write_msg(int fd,const Message*m){return write(fd,m,sizeof(*m))==(ssize_t)sizeof(*m)?0:-1;}
static int read_msg(int fd,Message*m){return read(fd,m,sizeof(*m))==(ssize_t)sizeof(*m)?0:-1;}
static int ucs(const Message*m){return write_msg(uc[1],m);} static int ucr(Message*m){return read_msg(uc[0],m);}
static int cus(const Message*m){return write_msg(cu[1],m);} static int cur(Message*m){return read_msg(cu[0],m);}
static int cls(const Message*m){return write_msg(cl[1],m);} static int clr(Message*m){return read_msg(cl[0],m);}
static void closep(void){for(int i=0;i<2;i++){close(uc[i]);close(cu[i]);close(cl[i]);}}
int pipe_backend_init(IPCOps*o){
    if(pipe(uc)||pipe(cu)||pipe(cl))return -1;
    o->ui_to_core_send=ucs;o->ui_to_core_recv=ucr;
    o->core_to_ui_send=cus;o->core_to_ui_recv=cur;
    o->core_to_log_send=cls;o->logger_recv=clr;o->close_all=closep;return 0;
}