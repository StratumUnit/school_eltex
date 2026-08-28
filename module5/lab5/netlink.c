
#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <net/net_namespace.h>

#define NETLINK_USER 31

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shmakov");
MODULE_DESCRIPTION("a educational kernel module for lab5");

struct sock *nl_sk = NULL;

static void hello_nl_recv_msg(struct sk_buff *skb)
{

    struct nlmsghdr *nlh;
    int pid;
    struct sk_buff *skb_out;
    int msg_size;
    char *user_msg;
    char *prefix = "Kernel received: ";
    int res;

    printk(KERN_INFO "Entering: %s\n", __FUNCTION__);

    nlh = (struct nlmsghdr *)skb->data;

    user_msg = nlmsg_data(nlh);

    printk(KERN_INFO "Netlink received msg payload: %s\n", (char *)user_msg);
    pid = nlh->nlmsg_pid; /*pid of sending process */

    msg_size = strlen(prefix) + strlen(user_msg) + 1;

    skb_out = nlmsg_new(msg_size, 0);

    if (!skb_out)
    {

        printk(KERN_ERR "Failed to allocate new skb\n");
        return;
    }
    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
    NETLINK_CB(skb_out).dst_group = 0;

    snprintf((char *)nlmsg_data(nlh), msg_size, "%s%s", prefix, user_msg);

    res = nlmsg_unicast(nl_sk, skb_out, pid);

    if (res < 0)
        printk(KERN_INFO "Error while sending bak to user\n");
}

struct netlink_kernel_cfg cfg = {
    .groups = 1,
    .input = hello_nl_recv_msg,
};

static int __init hello_init(void)
{

    printk("Entering: %s\n", __FUNCTION__);
    nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);

    // nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, 0, hello_nl_recv_msg,
    //                              NULL, THIS_MODULE);
    if (!nl_sk)
    {
        printk(KERN_ALERT "Error creating socket.\n");
        return -10;
    }

    return 0;
}

static void __exit hello_exit(void)
{

    printk(KERN_INFO "exiting netlink module\n");
    netlink_kernel_release(nl_sk);
}
module_init(hello_init);
module_exit(hello_exit);
