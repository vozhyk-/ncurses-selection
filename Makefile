
CFLAGS += -std=c99
LDFLAGS += -lncurses

OBJS = sel.o dyn_arr.o

all: sel

sel: $(OBJS)

dyn_arr.o: dyn_arr.c dyn_arr.h

clean:
	rm sel $(OBJS)
