CC = gcc
CC_FLAGS = $(shell pkg-config --cflags glib-2.0 gthread-2.0 gobject-2.0)
CC_FLAGS += -std=c99 -g
LD_FLAGS = $(shell pkg-config --libs glib-2.0 gthread-2.0 gobject-2.0)

gobjectdemo:main.o my_demo.o
	$(CC) -o $@ $+ $(LD_FLAGS)

%.o:%.c
	$(CC) $(CC_FLAGS) -c -o $@ $<
 
clean:
	rm -rf *.o gobjectdemo
