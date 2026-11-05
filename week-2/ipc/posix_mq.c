#include <stdio.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ipc.h"

static mqd_t q_uc=(mqd_t)-1, q_cu=(mqd_t)-1, q_cl=(mqd_t)-1;
static const char *names[]={"/w4sim_uc","/w4sim_cu","/w4sim_cl"};
static int sendq(mqd_t q,const Message *m) { return mq_send(q,(const char*)m,sizeof(*m),0); }
static int recvq(mqd_t q,Message *m) { return mq_receive(q,(char*)m,sizeof(*m),NULL)<0?-1:0; }
static int uc_s(const Message*m){return sendq(q_uc,m);} static int uc_r(Message*m){return recvq(q_uc,m);}
static int cu_s(const Message*m){return sendq(q_cu,m);} static int cu_r(Message*m){return recvq(q_cu,m);}
static int cl_s(const Message*m){return sendq(q_cl,m);} static int cl_r(Message*m){return recvq(q_cl,m);}
static void closeq(void) {
    if(q_uc!=(mqd_t)-1) mq_close(q_uc);
    if(q_cu!=(mqd_t)-1) mq_close(q_cu);
    if(q_cl!=(mqd_t)-1) mq_close(q_cl);
    for(int i=0;i<3;i++) mq_unlink(names[i]);
}
int mq_backend_init(IPCOps *o) {
    struct mq_attr a={0}; a.mq_maxmsg=10; a.mq_msgsize=sizeof(Message);
    for(int i=0;i<3;i++) mq_unlink(names[i]);
    q_uc=mq_open(names[0],O_CREAT|O_RDWR,0600,&a);
    q_cu=mq_open(names[1],O_CREAT|O_RDWR,0600,&a);
    q_cl=mq_open(names[2],O_CREAT|O_RDWR,0600,&a);
    if(q_uc==(mqd_t)-1||q_cu==(mqd_t)-1||q_cl==(mqd_t)-1) return -1;
    o->ui_to_core_send=uc_s;o->ui_to_core_recv=uc_r;
    o->core_to_ui_send=cu_s;o->core_to_ui_recv=cu_r;
    o->core_to_log_send=cl_s;o->logger_recv=cl_r;
    o->close_all=closeq; return 0;
}
