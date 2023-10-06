# Makefile for chessgame

# variables
CFLAGS = -Wall -std=c11
LFLAGS = -Wall -lm
GTKO = `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --cflags gtk+-2.0`
GTKC = `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --libs gtk+-2.0`

#default target
all: chessgame

#Generate Object Files
piece.o: piece.h
	gcc $(CFLAGS) -c piece.c -o piece.o

logfile.o: logfile.h piece.h
	gcc $(CFLAGS) -c logfile.c -o logfile.o

chess.o: logfile.h piece.h chess.h
	gcc $(CFLAGS) -c chess.c -o chess.o

ai.o: ai.h logfile.h piece.h chess.h
	gcc $(CFLAGS) -c ai.c -o ai.o

graphics.o: graphics.h ai.h logfile.h
	gcc $(CFLAGS) $(GTKO) -g -c graphics.c -o graphics.o

#Generate Executables

chessgame: piece.o logfile.o chess.o AI.o main.o graphics.o
	gcc $(LFLAGS) $(GUIFLAG) piece.o logfile.o chess.o AI.o main.o graphics.o -o chessgame

testg: graphics.o ai.o chess.o piece.o logfile.o
	gcc $(LFLAGS) -g $(GTKC) graphics.o ai.o chess.o logfile.o piece.o -o testg

#Others
clean:
	rm -f *o
