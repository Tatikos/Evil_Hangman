# Makefile for Evil Hangman Project
# EPL232 - Programming Techniques and Tools

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = hangMan
OBJS = as2.o hashtable.o dictionary.o game.o

# Main target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Object files
as2.o: as2.c hashtable.h dictionary.h game.h
	$(CC) $(CFLAGS) -c as2.c

hashtable.o: hashtable.c hashtable.h
	$(CC) $(CFLAGS) -c hashtable.c

dictionary.o: dictionary.c dictionary.h hashtable.h
	$(CC) $(CFLAGS) -c dictionary.c

game.o: game.c game.h hashtable.h
	$(CC) $(CFLAGS) -c game.c

# Test individual modules
test_hashtable: hashtable.c hashtable.h
	$(CC) $(CFLAGS) -DHASHTABLE_MAIN -o test_hashtable hashtable.c

test_dictionary: dictionary.c dictionary.h hashtable.c hashtable.h
	$(CC) $(CFLAGS) -DDICTIONARY_MAIN -o test_dictionary dictionary.c hashtable.c

test_game: game.c game.h hashtable.c dictionary.c hashtable.h dictionary.h
	$(CC) $(CFLAGS) -DGAME_MAIN -o test_game game.c hashtable.c dictionary.c

# Clean
clean:
	rm -f $(OBJS) $(TARGET) test_hashtable test_dictionary test_game

# Valgrind test
valgrind: $(TARGET)
	valgrind --tool=memcheck --leak-check=full --show-reachable=yes --num-callers=20 --track-fds=yes --track-origins=yes ./$(TARGET) toyexample.txt &> valgrind.txt

.PHONY: clean valgrind