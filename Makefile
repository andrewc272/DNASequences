proj2: proj2.c
	gcc proj2.c -o proj2 -Wall -lpthread

clean:
	rm -f *.o proj2
