CC=gcc
SRCS=arraylist.c  debug.c  json_object.c  json_tokener.c  json_util.c  linkhash.c  printbuf.c
OBJS=$(SRCS:.c=.o)
TARGET=libjson.a

all : $(OBJS)
	$(AR) rcu $(TARGET) $^

distclean: clean
clean:
	rm -f *.o *.a

%.o : %.c
	$(CC) -g -O2 -o $@ -c $^

