all: acram clean

acram: main.o texcaller.o makelatex.o parser.o mystring.o
	c++ main.o texcaller.o makelatex.o parser.o mystring.o -o acram

main.o:
	c++ -c main.cpp -o main.o
	
texcaller.o:
	gcc -c texcaller.c -o texcaller.o

parser.o:
	c++ -c parser.cpp -o parser.o

makelatex.o:
	c++ -c makelatex.cpp -o makelatex.o

mystring.o:
	c++ -c mystring.cpp -o mystring.o
	
clean:
	rm -rf *.o
