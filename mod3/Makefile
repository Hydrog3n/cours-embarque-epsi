# Si KERNELRELEASE est defini on peut compiler !

ifneq ($(KERNELRELEASE),)
    obj-m := mod3.o mod3a.o mod3b.o
else
   KERNELDIR ?= /lib/modules/$(shell uname -r)/build
   PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

endif
