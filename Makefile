OBJS = reciter.o sam.o render.o main.o debug.o processframes.o createtransitions.o

CC = gcc

# libsdl present
CFLAGS =  -Wall -Wno-char-subscripts -Wno-unused-function -Wno-pointer-sign -Og -ggdb3 -DUSESDL `sdl-config --cflags`
LFLAGS = `sdl-config --libs`

# no libsdl present
#CFLAGS =  -Wall -O2
#LFLAGS = 

sam: $(OBJS)
	$(CC) -o sam $(OBJS) $(LFLAGS)

%.o: src/%.c
	$(CC) $(CFLAGS) -c $<

package: 
	tar -cvzf sam.tar.gz README.md Makefile sing src/

ARG=one two three four
run: sam
	./sam -debug $(ARG)
debug: sam
	gdb -q --args sam -debug $(ARG)

clean:
	rm *.o
