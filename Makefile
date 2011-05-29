CFLAGS = `pkg-config --cflags gtk+-2.0` `curl-config --cflags` 
LIBS   = `pkg-config --libs gtk+-2.0` `curl-config --libs`

all: main.o cJSON/cJSON.o
	gcc $(LIBS) -o GtkUrlShortener main.o cJSON.o

clean:
	rm -rf *.o GtkUrlShortener

%.o: %.c
	gcc $(CFLAGS) -c $<
