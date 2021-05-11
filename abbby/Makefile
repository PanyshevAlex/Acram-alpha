all: acram

acram: main.o texcaller.o parser.o
	c++ main.o texcaller.o parser.o -o acram

main.o:
	c++ -c main.cpp -o main.o
	
texcaller.o:
	gcc -c texcaller.c -o texcaller.o

parser.o:
	c++ -c parser.cpp -o parser.o

clean:
	rm -rf *.o
