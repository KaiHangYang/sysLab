a: main.o get put copy 
	gcc -o a main.o -lpv -L./
get: get.o
	gcc -o get get.o -lpv -L./
copy: copy.o
	gcc -o copy copy.o -lpv -L./
put: put.o
	gcc -o put put.o -lpv -L./
main.o: main.o
	gcc -c main.c
get.o: get.c pv.h
	gcc -c get.c 
copy.o: copy.c pv.h
	gcc -c copy.c
put.o: put.c pv.h
	gcc -c put.c
clean:
	rm a main.o get.o put.o copy.o get copy put
