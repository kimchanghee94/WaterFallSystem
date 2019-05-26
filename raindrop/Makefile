KERNELDIR = /lib/modules/4.14.79-v7+/build



obj-m := rains_dev.o

PWD = $(shell pwd)

all :
	make -C $(KERNELDIR) M=$(PWD) modules


clean :
	make -C $(KERNELDIR) M=$(PWD) clean
