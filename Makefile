# A simple makefile

.POSIX:
.SUFFIXES:

include config.mk

release:

ifeq ($(HAVEDEBUGINFO), debug_info)
release: clean
else
debug: clean
endif

release: smm

debug: LDFLAGS += $(DBGFLAGS)
debug: CFLAGS += $(DBGFLAGS)
debug: smm

smm: $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $(OBJ) $(LDLIBS)

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $<

smm.o: smm.c smm.h matrix.h draw.h log.h
matrix.o: matrix.c matrix.h draw.h log.h
draw.o: draw.c matrix.h draw.h log.h
log.o: log.c log.h

clean:
	rm -f $(OBJ)

.PHONY: debug release clean
