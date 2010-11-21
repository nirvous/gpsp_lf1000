# gpSP makefile
# Gilead Kutnick - Exophase
# GP2X port(ion) - Z
# LF1000 port - nirvous

# Global definitions

#Edit values below to reflect your build environment.
#PREFIX  = /this/should/point/to/your/cross/compiler 
PREFIX  = /opt/RidgeRun/arm-eabi-uclibc/arm-linux
CC        = $(PREFIX)/bin/gcc
STRIP     = $(PREFIX)/bin/strip

OBJS      = main.o cpu.o memory.u video.o input.o sound.o lf1000.o gui.o        \
            cheats.o zip.o cpu_threaded.z arm_stub.o video_blend.o            \
            warm.o upscale_aspect.o

OBJS      += pollux_dpc_set.o
BIN       = gpsp_lf1000


# Platform specific definitions 

VPATH      += ..
CFLAGS     += -DARM_ARCH -DGP2X_BUILD
CFLAGS     += -DWIZ_BUILD
# unset LX_BUILD for Didj kernel (2.6.20) binary
#CFLAGS     += -DLX_BUILD


# debug build
#CFLAGS     +=  -ggdb -std=c99 -msoft-float -funsigned-char -fno-common           \
#             -fno-builtin                                                    \

#optimized build
# NOTE: -funroll-loops (and -O3) will slow down compiling considerably
CFLAGS     +=  -O3 -std=c99 -msoft-float -funsigned-char -fno-common           \
              -fno-builtin                                                    \

INCLUDES   = `$(PREFIX)/bin/sdl-config --cflags` -I$(PREFIX)/include
LIBS       = `$(PREFIX)/bin/sdl-config --libs`               \
              -lm -ldl -lpthread -lz

# Compilation:

.SUFFIXES: .c

%.z: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.u: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.S
	$(CC) $(ASFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.s
	$(CC) $(ASFLAGS) $(INCLUDES) -c -o $@ $<

all:	$(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(BIN)
	#$(STRIP) $(BIN)

clean:
	rm -f *.o *.u *.z $(BIN) 

