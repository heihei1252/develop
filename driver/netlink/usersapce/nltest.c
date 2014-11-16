
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/netlink.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>

//#define NETLINK_TEST 31 // �Զ����Э���  

#define NETLINK_TEST  17

/** ��Ϣ���� **/
#define NLMSG_GETECHO  (0x10 + 2)
/** ���Э�鸺��(�̶�) **/
#define MAX_PAYLOAD 101

struct sockaddr_nl src_addr, dst_addr;
struct iovec iov;
int sockfd;
struct nlmsghdr *nlh = NULL;
struct msghdr msg;

int main( int argc, char *argv[])
{
        if (argc != 2) {
                printf("usage: ./a.out <str>\n");
                exit(-1);
        }
        sockfd = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_TEST); // ����NETLINK_TESTЭ���socket
        /* ���ñ��ض˵㲢�󶨣��������� */
        bzero(&src_addr, sizeof(src_addr));
        src_addr.nl_family = AF_NETLINK;
        src_addr.nl_pid = getpid();
        src_addr.nl_groups = 0; //δ����ಥ��
        bind(sockfd, (struct sockaddr*)&src_addr, sizeof(src_addr));
        /* ����Ŀ�Ķ˵㣬���ڷ��� */
        bzero(&dst_addr, sizeof(dst_addr));
        dst_addr.nl_family = AF_NETLINK;
        dst_addr.nl_pid = 0; // ��ʾ�ں�
        dst_addr.nl_groups = 0; //δָ�����նಥ��
        /* ���췢����Ϣ */
        nlh = malloc(NLMSG_SPACE(MAX_PAYLOAD));
        nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD); //��֤����
        nlh->nlmsg_pid = getpid();  /* self pid */
        nlh->nlmsg_flags = 0;
        nlh->nlmsg_type = NLMSG_GETECHO;
        strcpy(NLMSG_DATA(nlh), argv[1]);
        iov.iov_base = (void *)nlh;
        iov.iov_len = nlh->nlmsg_len;
        msg.msg_name = (void *)&dst_addr;
        msg.msg_namelen = sizeof(dst_addr);
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;

        sendmsg(sockfd, &msg, 0); // ����
        /* ������Ϣ����ӡ */
        memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
        recvmsg(sockfd, &msg, 0);
        printf(" Received message payload: %s\n", (char *)NLMSG_DATA(nlh));
        return 0;
}
