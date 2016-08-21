#include "mylib.h"
static struct kobject *my_kobject;
static char msg[BUFF_LEN];
static struct kobj_attribute my_attribute = __ATTR(msg, 0660,
				my_show, my_store);

static ssize_t my_show(struct kobject *kobj, struct kobj_attribute *attr,
					char *buf) {
	static int size;

	size = sprintf(buf, "%s\n", msg);
	pr_info("Send to user %s", msg);
	return size;
}

static ssize_t my_store(struct kobject *kobj, struct kobj_attribute *attr,
					char *buf, size_t count) {
	if (sscanf(buf, "%s", msg) < 0) {
		pr_info("sscanf failed\n");
		return -1;
	}
	pr_info("Recieved from user %s\n", msg);
	return count;
}

static int __init mymodule_init(void)
{
	static int ret;

	pr_info("Module initialized successfully\n");
	my_kobject = kobject_create_and_add("my_kobject", kernel_kobj);
	if (!my_kobject)
		return -ENOMEM;
	ret = sysfs_create_file(my_kobject, &my_attribute.attr);
	if (ret < 0) {
		pr_info("Failed to create file\n");
		return ret;
	}
	return 0;
}

static void __exit mymodule_exit(void)
{
	pr_info("Module un initialized successfully\n");
	kobject_put(my_kobject);
}

module_init(mymodule_init);
module_exit(mymodule_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Buin Buin");
MODULE_DESCRIPTION("Testing sys fs module");