CFLAGS = `pkg-config --cflags gtk+-2.0` `curl-config --cflags` 
LIBS   = `pkg-config --libs gtk+-2.0` `curl-config --libs`

all: main.o cJSON.o
	gcc $(LIBS) -o GtkUrlShortener main.o cJSON.o

clean:
	rm -rf *.o GtkUrlShortener

main.o: main.c
	gcc $(CFLAGS) -c main.c

cJSON.o: cJSON/cJSON.c
	gcc $(CFLAGS) -c cJSON/cJSON.c

