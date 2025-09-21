# Fill this makefile with the makefile format instructions to 
#   build your program.
dict1: main.o list.o data.o patricia.o
	gcc -o dict1 main.o list.o data.o patricia.o

dict2: main.o list.o data.o patricia.o
	gcc -o dict2 main.o list.o data.o patricia.o

dict3: main.o list.o data.o patricia.o
	gcc -o dict3 main.o list.o data.o patricia.o

dict4: main.o list.o data.o patricia.o
	gcc -o dict4 main.o list.o data.o patricia.o


main.o: main.c list.h data.h patricia.h
	gcc -Wall -g -c -o main.o main.c

list.o: list.c list.h
	gcc -Wall -g -c -o list.o list.c

data.o: data.c data.h
	gcc -Wall -g -c -o data.o data.c

patricia.o: patricia.c patricia.h
	gcc -Wall -g -c -o patricia.o patricia.c

clean:
	rm -f dict1 dict2 *.o

run_dict1: dict1
	./dict1

run_dict2: dict2
	./dict2

run_dict3: dict3
	./dict3

run_dict4: dict4
	./dict4
