EXEC=manager

manager.o: manager.c

all:manager.o
	gcc main.c manager.o manager.h -g -o $(EXEC) 

run:
	./$(EXEC) addresses.txt

debug:
	gdb $(EXEC)

clean:
	rm $(EXEC) *.o -f