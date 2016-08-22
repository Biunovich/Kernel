#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/string.h> 
#include <asm/uaccess.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/miscdevice.h>

#define SUCCESS 0
#define DEVICE_NAME "chardev"
#define BUFF_LEN 512
#define IOC_MAGIC   'h' 
#define IOCTL_GET_STRING _IOR(IOC_MAGIC, 1, char *) 
#define IOCTL_SET_STRING _IOW(IOC_MAGIC, 0, char *)
#define NETLINK_USER 31

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static ssize_t my_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t my_store(struct kobject *kobj, struct kobj_attribute *attr, char *buf, size_t count);

static long device_ioctl (struct file *, unsigned int, unsigned long);