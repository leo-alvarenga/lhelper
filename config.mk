NAME = lstatus

SRC = lstatus.c main.c
OUT = lstatus

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

# includes and libs
INCS = -I. -I/usr/include -I${X11INC}
LIBS = -L/usr/lib -lc -L${X11LIB} -lX11

PREFIX = /usr/local

# compiler and linker
CC = cc
CFLAGSX = -O0 ${INCS}
LDFLAGSX = -g ${LIBS}
CFLAGS = 
LDFLAGS = 
