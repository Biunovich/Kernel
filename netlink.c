#include "mylib.h"
struct sock *nl_sock;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Buin Buin");
MODULE_DESCRIPTION("Testing char dev module");

static void nl_recvmsg(struct sk_buff *skb)
{
	struct nlmsghdr *nlh;
	int pid;
	struct sk_buff *skb_out;
	int msg_size;
	char *msg = "hellow from kernel";
	int res;

	msg_size = strlen(msg);
	nlh = (struct nlmsghdr *)skb->data;
	pr_info("Recieved %s\n", (char *)nlmsg_data(nlh));
	pid = nlh->nlmsg_pid;
	skb_out = nlmsg_new(msg_size, 0);
	if (!skb_out) {
		pr_info("nlmsg_new failed\n");
		return;
	}
	nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
	NETLINK_CB(skb_out).dst_group = 0;
	strncpy(nlmsg_data(nlh), msg, msg_size);
	res = nlmsg_unicast(nl_sock, skb_out, pid);
	if (res < 0)
		pr_info("nlmsg_unicast failed\n");
}

int init_module(void)
{
	struct netlink_kernel_cfg cfg = {
		.input = nl_recvmsg
	};
	pr_info("init_module\n");
	nl_sock = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
	if (!nl_sock) {
		pr_info("netlink_kernel_create failed\n");
		return -10;
	}
	return 0;
}

void cleanup_module(void)
{
	pr_info("cleanup_module\n");
	netlink_kernel_release(nl_sock);
}