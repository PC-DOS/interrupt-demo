#!/bin/bash

# Specify source code
# Define object file
obj-m += interrupt-demo.o

# KDIR specifies source code directory
KRNLDIR := /home/picsell-dois/iTop4412/LinuxKernel/iTop4412_Kernel_3.0

# PWD specifies current working directory
PWD ?= $(shell pwd)

# Operations when calling make
all:
	rm -rf *.ko *.o *.mod.* *.order *.symvers *.cmd *.*.cmd .*.cmd .*.*.cmd .tmp_versions
	make -C $(KRNLDIR) M=$(PWD) modules
	chmod 777 *.ko

# Operations when calling make clean
clean:
	rm -rf *.o *.mod.* *.order *.symvers *.cmd *.*.cmd .*.cmd .*.*.cmd .tmp_versions
