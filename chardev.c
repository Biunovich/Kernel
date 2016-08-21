#include "mylib.h"
static int Major;
static int counter;
static int Device_Open;
static char msg[BUFF_LEN];
static char *msg_Ptr;
static const struct file_operations fops = {
	.read		= device_read,
	.write		= device_write,
	.open		= device_open,
	.release	= device_release
};
static struct miscdevice misc_chardev = {
	MISC_DYNAMIC_MINOR,
	"chardev",
	&fops
};

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Buin Buin");
MODULE_DESCRIPTION("Testing char dev module");

int init_module(void)
{
	static int ret;

	counter = 0;
	Device_Open = 0;
	Major = register_chrdev(0, DEVICE_NAME, &fops);
	if (Major < 0) {
		pr_info("register_chrdev failed with %d\n", Major);
		return Major;
	}
	ret = misc_register(&misc_chardev);
	if (ret < 0) {
		pr_info("misc_register failed with %d\n", ret);
		return ret;
	}
	pr_info("My Major %d\n", Major);

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
	sprintf(msg, "%d Hellow world\n", counter++);
	msg_Ptr = msg;
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
	static int bytes_read;

	bytes_read = 0;
	if (*msg_Ptr == 0)
		return 0;
	while (len && *msg_Ptr) {
		put_user(*(msg_Ptr++), buffer++);
		len--;
		bytes_read++;
	}
	return bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buff,
						size_t len, loff_t *offset)
{
	static int bytes_read;

	memset(msg, 0, len + 1);
	copy_from_user(msg, buff, len);
	bytes_read = strlen(msg);
	pr_info("Recieved from user %d %s\n", bytes_read, msg);
	return bytes_read;
}