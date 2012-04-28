# Makefile for NAS_LCD Module Linux driver

CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)
DEST = /lib/modules/$(CURRENT)/misc

TARGET = nas_lcd

obj-m	:=	$(TARGET).o

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules


# remove builded files
clean:
	@rm -f *.o .*.cmd .*.flags *.mod.c *.order
	@rm -f .*.*.cmd *.symvers *~ *.*~
	@rm -fR .tmp*
	@rm -rf .tmp_versions


# install driver into system
install:
	echo "Option doesn't ready yet."


# remove driver from system
remove:
	echo "Option doesn't ready yet."
