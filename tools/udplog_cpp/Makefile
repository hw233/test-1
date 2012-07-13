
CPP=g++
AR=ar

SRCS=$(shell ls *.cpp)
OBJS=$(SRCS:.cpp=.o)
TARGET=libudplog.a

all : $(OBJS)
	$(AR) rcu $(TARGET) $^

clean:
	rm -f *.o *.a

%.o : %.cpp
	$(CPP) -O2 -o $@ -c $^

