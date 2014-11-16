
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <net/sock.h>
#include <linux/netlink.h>

#define NETLINK_TEST 17
struct {
        __u32 pid;
}user_process;

static struct sock *netlinkfd = NULL;

int send_to_user(char *info) //���͵��û��ռ�
{
        int size;
        struct sk_buff *skb;
        unsigned char *old_tail;
        struct nlmsghdr *nlh; //����ͷ

        int retval;

        size = NLMSG_SPACE(strlen(info)); //���Ĵ�С
        skb = alloc_skb(size, GFP_ATOMIC); //����һ���µ��׽��ֻ���,ʹ��GFP_ATOMIC��־���̲�>�ᱻ��Ϊ˯��

        //��ʼ��һ��netlink��Ϣ�ײ�
        nlh = nlmsg_put(skb, 0, 0, 0, NLMSG_SPACE(strlen(info))-sizeof(struct nlmsghdr), 0);
        old_tail = skb->tail;
        memcpy(NLMSG_DATA(nlh), info, strlen(info)); //���������
        nlh->nlmsg_len = skb->tail - old_tail; //������Ϣ����

        //���ÿ����ֶ�
        NETLINK_CB(skb).pid = 0;
        NETLINK_CB(skb).dst_group = 0;

        printk(KERN_DEBUG "[kernel space] skb->data:%s\n", (char *)NLMSG_DATA((struct nlmsghdr *)skb->data));

        //��������
        retval = netlink_unicast(netlinkfd, skb, user_process.pid, MSG_DONTWAIT);
        printk(KERN_DEBUG "[kernel space] netlink_unicast return: %d\n", retval);
        return 0;
}

void kernel_receive(struct sk_buff *__skb) //�ں˴��û��ռ��������
{
        struct sk_buff *skb;
        struct nlmsghdr *nlh = NULL;

        char *data = "This is eric's test message from kernel";

        printk(KERN_DEBUG "[kernel space] begin kernel_receive\n");
        skb = skb_get(__skb);
        if(skb->len >= sizeof(struct nlmsghdr)){
                nlh = (struct nlmsghdr *)skb->data;
                if((nlh->nlmsg_len >= sizeof(struct nlmsghdr))
                                && (__skb->len >= nlh->nlmsg_len)){
                        user_process.pid = nlh->nlmsg_pid;
                        printk(KERN_DEBUG "[kernel space] data receive from user are:%s\n", (char *)NLMSG_DATA(nlh));
                        printk(KERN_DEBUG "[kernel space] user_pid:%d\n", user_process.pid);
                        send_to_user(data);
                }
        }else{
                printk(KERN_DEBUG "[kernel space] data receive from user are:%s\n",(char *)NLMSG_DATA(nlmsg_hdr(__skb)));
                send_to_user(data);
        }
        kfree_skb(skb);
}

int __init test_netlink_init(void)
{

        netlinkfd = netlink_kernel_create(&init_net, NETLINK_TEST, 0, kernel_receive, NULL, THIS_MODULE);
        if(!netlinkfd){
                printk(KERN_ERR "can not create a netlink socket\n");
                return -1;
        }
        return 0;
}

void __exit test_netlink_exit(void)
{
        sock_release(netlinkfd->sk_socket);
        printk(KERN_DEBUG "test_netlink_exit!!\n");
}

module_init(test_netlink_init);
module_exit(test_netlink_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("eric.hu");
