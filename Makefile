#
#		8080&CP/M emulator
#	Makefile for FreeBSD
#		by S.Takeoka
#
#	 If the byte order of your machine is like 68K then define B_ENDIAN,
#	or VAX like then define L_ENDIAN.
#	 If you want a 'trace' and a 'break point' with the monitor
#	then define TRACE.
LIB_DIR = /usr/local/lib/
BIN_DIR = /usr/local/bin/

CFLAGS = -DLINUX -DTERMCAP  -DL_ENDIAN -DPARITY -DEMACS  #-DDEBUG -DVT_VT

##CFLAGS = -DMACOSX -DTERMCAP  -DL_ENDIAN -DPARITY -DEMACS  #-DDEBUG -DVT_VT
#CFLAGS = -DMACOSX  -DL_ENDIAN -DPARITY -DEMACS  #-DDEBUG -DVT_VT
#CFLAGS =  -traditional -DL_ENDIAN -DPARITY -DEMACS  #-DDEBUG -DVT_VT
#LIBS= -lcompat
#CFLAGS =  -DLINUX -DL_ENDIAN -DPARITY -DEMACS  -DVT_VT #-DDEBUG

#CFLAGS =  -O4 -DB_ENDIAN -DPARITY -DEMACS #-DDEBUG -DVT_VT
#CFLAGS = -I. -DB_ENDIAN -DPARITY -DWORDSTAR
#CFLAGS = -O4 -I. -DB_ENDIAN -DPARITY -DEMACS
#CFLAGS = -I. -DB_ENDIAN -DPARITY -DTRACE -DEMACS

#CFLAGS = -I. -O -DB_ENDIAN
#CFLAGS = -I. -O -DB_ENDIAN -DSYSV
#CFLAGS = -I.-O -DTRACE -DB_ENDIAN -DSYSV
#CFLAGS = -I. -O -DL_ENDIAN



all: cpm

#
#	8080&CP/M emulator
#
80OBJECTS =	mon.o bios.o i80.o bdos.o ccp.o main.o lined.o
80SRC =		mon.c bios.c i80.c bdos.c ccp.c main.c lined.c getopt.c
# for BSD
#80OBJECTS +=	getopt.o 

cpm: ${80OBJECTS}
	${CC} ${CFLAGS} -o cpm ${80OBJECTS} -ltermcap $(LIBS)

mon.c : i80.h bios.h
bios.o : env.h i80.h bdos.h bios.h emt.h
i80.o : i80.h
bdos.o : env.h i80.h bdos.h bdosfunc.h
ccp.o : env.h i80.h bdos.h
main.o : env.h i80.h bdos.h default.h bios.h
lined.o: env.h
getopt.o :


#
#	install
#
install:
	cp cpm $(BIN_DIR)
	cp bios64.sys $(LIB_DIR)
	cp cpmconf $(LIB_DIR)

# tar
DATE=`date +"%Y%m%d-%H%M%S"`
tar:
	(cd ..; tar cvzf cpmsim-${DATE}.tgz cpsim)


#
#	clean
#
clean:
	rm -f *.o cpm

