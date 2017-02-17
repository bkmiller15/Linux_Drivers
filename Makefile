#############################################################################
#
# Makefile for Linux Drivers
#
# License: GPL (General Public License)
# Author:  Brandon Miller
# Date:    2006/03/04 (version 0.1)
#
# Description:
# ------------
# This makefile is used to build and clean linux kernel modules
#
# Remember that the variable mod has to be passed in and must be the mame of
# the module plus .o
#
# $make mod=test1.o
#
#############################################################################

# Disables Signature Verification
CONFIG_MODULE_SIG=n

# Set object to make
obj-m += $(mod)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
