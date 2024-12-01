OBJS = .obj/reciter.o .obj/sam.o .obj/render.o .obj/main.o .obj/debug.o .obj/processframes.o .obj/createtransitions.o

.PHONY: debug

CC = gcc

OBJ_DIR = .obj

# libsdl present
CFLAGS =  -Wall -Wno-char-subscripts -Wno-unused-function -Wno-pointer-sign -Og -ggdb3 -DUSESDL `sdl-config --cflags`
LFLAGS = `sdl-config --libs`

# no libsdl present
#CFLAGS =  -Wall -O2
#LFLAGS = 

sam: $(OBJS)
	$(CC) -o sam $(OBJS) $(LFLAGS)

$(OBJ_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

package: 
	tar -cvzf sam.tar.gz README.md Makefile sing src/

ARG=-sing -pitch 64 -phonetic ohohoh
# ARG=I love pears one 129 thyme kees
# ARG=one two three four five six seven eight nine ten eleven twelve thirteen fourteen fifteen sixteen seventeen eighteen nineteen twenty twentyone twentytwo twentythree twentyfour twentyfive twentysix twentyseven twentyeight twentynine thirty thirtyone thirtytwo thirtythree thirtyfour thirtyfive thirtysix thirtyseven thirtyeight thirtynine forty fortyone fortytwo fortythree fortyfour fortyfive fortysix fortyseven fortyeight fortynine fifty fiftyone fiftytwo fiftythree fiftyfour fiftyfive fiftysix fiftyseven fiftyeight fiftynine
run: sam
	./sam $(ARG) &> /tmp/tmpsam
	@diff -q /tmp/tmpsam ~/good || true
	@diff -q /tmp/tmpsam ~/good || true
	@diff -q /tmp/tmpsam ~/good || true
	@diff -q /tmp/tmpsam ~/good || true
	@diff -q /tmp/tmpsam ~/good || true
	@diff -q /tmp/tmpsam ~/good || true
	@diff -q /tmp/tmpsam ~/good || true
	@diff -q /tmp/tmpsam ~/good || true
	@diff -q /tmp/tmpsam ~/good
debug: sam
	gdb -q --args sam -debug $(ARG)
valgrind: sam
	# valgrind --leak-check=summary --log-file=valgrind.log ./sam $(ARG)
	valgrind --leak-check=summary ./sam $(ARG) 2>&1 | tee valg.log

clean:
	rm -f *.o
	rm -f .obj/*.o
