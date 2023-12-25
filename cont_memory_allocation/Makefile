EXEC = addresses

# command.o: command.c command.h

all:
	gcc main.c command.c command.h -o $(EXEC) -g

debug:
	gdb $(EXEC)

run:
	./$(EXEC) 1048576

clean:
	rm $(EXEC) *.o -f