
#
#DVB API Profiler by Greg Wickham is licensed under a
# Creative Commons Attribution-ShareAlike 4.0 International License.
#

PRG = dvb-pai-profiler

MAIN_OBJS = main.o profile.o adapter.o lib.o ofdm.o

OBJS = $(MAIN_OBJS)

CC = g++

CCFLAGS = -Wall -g

DEFINES = $(OPTIONS)

PKGCONFIG=/usr/bin/pkg-config

INCLUDES = `${PKGCONFIG} glibmm-2.4 sigc++-2.0 gthread-2.0 --cflags`

%.o: %.c Makefile
	@echo "    CC $<"
	@$(CC) $(CCFLAGS) -c $(DEFINES) $(INCLUDES) $<

LIBDIR	=
LIBS = `${PKGCONFIG} glibmm-2.4 sigc++-2.0 gthread-2.0 --libs` ${EXLIBS}

all : ${PRG}

clean :
	@echo "  CLEAN *.o ..."
	@rm -f ${PRG} ${OBJS}

${PRG}: ${OBJS}
	@echo "  LINK $(PRG)"
	@$(CC) ${CCFLAGS} -o ${PRG} ${OBJS} ${LIBDIR} ${LIBS}

# DO NOT DELETE
