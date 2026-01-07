KERNELDIR := /lib/modules/$(shell uname -r)/build
DIR := $(shell pwd)

obj-m += hook.o

all:

	 make -C $(KERNELDIR) M=$(DIR) modules

clean:

	 make -C $(KERNELDIR) M=$(DIR) clean
