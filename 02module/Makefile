modename ?=myled
arch ?=arm

ifeq ($(arch),arm)
KERNELDIR := /home/ubuntu/linux-stm32mp-5.10.10-r0/linux-5.10.10/#开发板上可安装arm格式
else
KERNELDIR := /lib/modules/$(shell uname -r)/build/ #ubuntu可以安装x86-64格式
endif

PWD:=$(shell pwd)

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

obj-m:=$(modename).o


