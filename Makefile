###############################################################################
# NAME:		    makefile
#
# AUTHOR:	    Ethan D. Twardy
#
# DESCRIPTION:	    Makefile for the executable C code contained in
#		    queuearray.c
#
# CREATED:	    06/07/2017
#
# LAST EDITED:	    01/04/2018
###

CC=gcc
ifeq ($(MAKECMDGOALS),debug)
	CFLAGS = -g -O0 -Wall -D CONFIG_DEBUG_QARRAY
else
	CFLAGS = -Wall -O3
endif

.PHONY: debug clean

qarray:

debug: qarray

clean:
	rm -rf *.dSYM
	rm -f *.o
	rm -f qarray

###############################################################################
