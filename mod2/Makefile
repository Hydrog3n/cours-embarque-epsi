# Si KERNELRELEASE est defini on peut compiler !

ifneq ($(KERNELRELEASE),)
    obj-m += mod2c.o 
    mod2c-y := mod2.o modPrk.o

else
   KERNELDIR ?= /lib/modules/$(shell uname -r)/build
   PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

endif
