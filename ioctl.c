#include "mylib.h"
static int Major;
static int counter;
static int Device_Open;
static char msg[BUFF_LEN];
static const struct file_operations fops = {
	.open		= device_open,
	.release	= device_release,
	.unlocked_ioctl		= device_ioctl
};
static struct miscdevice misc_chardev = {
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= "ioctl",
	.fops		= &fops,
	.mode		= 0666
};

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Buin Buin");
MODULE_DESCRIPTION("Testing char dev module");

int init_module(void)
{
	counter = 0;
	Device_Open = 0;
	Major = misc_register(&misc_chardev);
	if (Major < 0) {
		pr_info("misc_register failed with %d\n", Major);
		return Major;
	}

	return 0;
}

void cleanup_module(void)
{
	pr_info("unregister_chrdev\n");
	misc_deregister(&misc_chardev);
	unregister_chrdev(Major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *file)
{
	if (Device_Open)
		return -EBUSY;
	Device_Open++;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;
	module_put(THIS_MODULE);
	return SUCCESS;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t len,
						loff_t *offset)
{
	return 0;
}

static ssize_t device_write(struct file *filp, const char *buff,
						size_t len, loff_t *offset)
{
	return 0;
}

static long device_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case IOCTL_SET_STRING:
		if (copy_from_user(msg, (char *)arg, BUFF_LEN) < 0) {
			pr_info("copy_from_user failed\n");
			return -EFAULT;
		}
		pr_info("Recieved %s\n", msg);
		break;
	case IOCTL_GET_STRING:
		if (copy_to_user((char *)arg, msg, BUFF_LEN) < 0) {
			pr_info("copy_to_user failed\n");
			return -EFAULT;
		}
		pr_info("Send %s\n", msg);
		break;
	default:
		break;
	}
	return 0;
}