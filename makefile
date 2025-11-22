a.out : main.o Addition.o division.o function.o multiplication.o Subtraction.o
	gcc -o a.out  main.o Addition.o division.o function.o multiplication.o Subtraction.o
main.o: main.c
	gcc -c main.c
Addition.o : Addition.c
	gcc -c Addition.c
Subtraction.o : Subtraction.c
	gcc -c Subtraction.c
multiplication.o : multiplication.c
	gcc -c multiplication.c
division.o : division.c
	gcc -c division.c
function.o : function.c
	gcc -c function.c
clean : 
	rm -f *.o a.out