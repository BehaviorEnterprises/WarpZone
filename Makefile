
PROG     =  warpzone
VER      =  0.1
CC       ?= gcc
DEFS     =  -DPROGRAM_NAME=${PROG} -DPROGRAM_VER=${VER}
DEPS     =  cairo
CFLAGS   += $(shell pkg-config --cflags ${DEPS}) ${DEFS}
LDLIBS   += $(shell pkg-config --libs ${DEPS}) -lm
PREFIX   ?= /usr
MODULES  =  cairo warp warpzone
HEADERS  =  warpzone.h
#MANPAGES =  warpzone.1
VPATH    =  src:doc

${PROG}: ${MODULES:%=%.o}

%.o: %.c ${HEADERS}

install: ${PROG}
	echo "Not ready for installation yet"

clean:
	@rm -f ${PROG}-${VER}.tar.gz ${MODULES:%=%.o}

distclean: clean
	@rm -f ${PROG}

dist: distclean
	@tar -czf ${PROG}-${VER}.tar.gz *

.PHONY: clean dist distclean
