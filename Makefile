KERNELDIR:=/usr/src/linux-headers-$(shell uname -r)
PWD := $(shell pwd)
ccflags-y := -std=gnu99
#obj-m += chardev.o
#obj-m += sysfs.o
#obj-m += ioctl.o
obj-m += netlink.o
all:
	sudo $(MAKE) -C $(KERNELDIR) M=$(PWD) modules