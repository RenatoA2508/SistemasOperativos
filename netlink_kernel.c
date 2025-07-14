#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netlink.h>
#include <net/sock.h>
#include <linux/skbuff.h>

#define NETLINK_USER 31

struct sock *nl_sk = NULL;

static void nl_recv_msg(struct sk_buff *skb) {
    struct nlmsghdr *nlh;
    int pid;
    struct sk_buff *skb_out;
    char *msg = "Hola desde el kernel";
    int msg_size = strlen(msg);
    int res;

    nlh = (struct nlmsghdr *)skb->data;
    printk(KERN_INFO "Kernel recibió: %s\n", (char *)nlmsg_data(nlh));
    pid = nlh->nlmsg_pid; // PID del proceso usuario

    skb_out = nlmsg_new(msg_size, 0);
    if (!skb_out) {
        printk(KERN_ERR "Fallo al asignar skb_out\n");
        return;
    }

    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
    NETLINK_CB(skb_out).dst_group = 0; // unicast

    strncpy(nlmsg_data(nlh), msg, msg_size);

    res = nlmsg_send(nl_sk, skb_out, pid, 0, 0, GFP_KERNEL);
    if (res < 0)
        printk(KERN_INFO "Error al enviar la respuesta al usuario\n");
}

static int __init netlink_kernel_init(void) {
    struct netlink_kernel_cfg cfg = {
        .input = nl_recv_msg,
    };

    nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
    if (!nl_sk) {
        printk(KERN_ALERT "Error al crear el socket Netlink\n");
        return -10;
    }

    printk(KERN_INFO "Módulo Netlink cargado correctamente\n");
    return 0;
}

static void __exit netlink_kernel_exit(void) {
    netlink_kernel_release(nl_sk);
    printk(KERN_INFO "Módulo Netlink descargado\n");
}

module_init(netlink_kernel_init);
module_exit(netlink_kernel_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OpenAI / ChatGPT");
MODULE_DESCRIPTION("Módulo de ejemplo usando Netlink sockets");

