CC = gcc
CFLAGS = -g -Wall -std=c99
INCLUDE = -I.. -I../libstrophe
LIBS = -lssl -lresolv ../libstrophe/libstrophe.a ../libstrophe/libexpat.a 

gtalkbot:handler.o settings.o log.o plugin.o main.c
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LIBS)

%.o:%.c %.h
	$(CC) -c $^ $(CFLAGS) $(INCLUDE)

clean:
	rm handler.o settings.o log.o plugin.o

