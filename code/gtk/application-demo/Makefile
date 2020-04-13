LDFLAGS = `pkg-config --cflags --libs glib-2.0 gobject-2.0 gtk+-3.0`
CC            = gcc
targets       = application-demo

all:
	$(CC) main.c -o ${targets} $(LDFLAGS)

.PHONY: clean

clean:
	rm -f *.o
	rm -f ${targets}
