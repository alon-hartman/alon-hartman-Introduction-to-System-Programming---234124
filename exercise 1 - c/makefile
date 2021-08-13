MAIN_FILE = chessSystemTestsExample
DEBUG_FLAG = # -g for debug
COMP_FLAG = -std=c99 -Wall -Werror -pedantic-errors
EXEC = chess
OBJS = chess.o tournament.o list.o player.o game.o libmap.a
CC = gcc

$(EXEC): $(MAIN_FILE).o $(OBJS)
	$(CC) $(DEBUG_FLAG) $(COMP_FLAG) $(MAIN_FILE).o $(OBJS) -o $@
$(MAIN_FILE).o: ./tests/$(MAIN_FILE).c chessSystem.h test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) ./tests/$*.c
chess.o: chessSystem.c chessSystem.h map.h tournament.h game.h \
 list.h player.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) chessSystem.c -o chess.o
tournament.o: tournament.c tournament.h game.h map.h list.h player.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
list.o: list.c list.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
player.o: player.c player.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
game.o: game.c game.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

clean: 
	rm -f *.o