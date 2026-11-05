#include <sys/socket.h>
#include <unistd.h>
#include "ipc.h"
static int uc[2],cu[2],cl[2];
static int sendm(int fd,const Message*m){return send(fd,m,sizeof(*m),0)==(ssize_t)sizeof(*m)?0:-1;}
static int recvm(int fd,Message*m){return recv(fd,m,sizeof(*m),MSG_WAITALL)==(ssize_t)sizeof(*m)?0:-1;}
static int ucs(const Message*m){return sendm(uc[0],m);}static int ucr(Message*m){return recvm(uc[1],m);}
static int cus(const Message*m){return sendm(cu[0],m);}static int cur(Message*m){return recvm(cu[1],m);}
static int cls(const Message*m){return sendm(cl[0],m);}static int clr(Message*m){return recvm(cl[1],m);}
static void closes(void){for(int i=0;i<2;i++){close(uc[i]);close(cu[i]);close(cl[i]);}}
int socket_backend_init(IPCOps*o){
    if(socketpair(AF_UNIX,SOCK_STREAM,0,uc)||socketpair(AF_UNIX,SOCK_STREAM,0,cu)||socketpair(AF_UNIX,SOCK_STREAM,0,cl))return -1;
    o->ui_to_core_send=ucs;o->ui_to_core_recv=ucr;o->core_to_ui_send=cus;o->core_to_ui_recv=cur;
    o->core_to_log_send=cls;o->logger_recv=clr;o->close_all=closes;return 0;
}
