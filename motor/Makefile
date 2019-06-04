KERNELDIR = /lib/modules/4.14.98-v7+/build

obj-m := servo180_dev.o

PWD := $(shell pwd)

default :
	make -C $(KERNELDIR) M=$(PWD) modules

clean :
	make -C $(KERNELDIR) M=$(PWD) clean
