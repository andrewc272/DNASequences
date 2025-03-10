#include <stdio.h>
#include "proj2.h"

int main ( int argc, char *argv[] ) {

	acceptArguments(argc, argv);
	
	return 0;
}

void acceptArguments( int argc, char *argv[] ) {
	if ( argc == 4 ) printf("Valid\n");
	else printf("Invalid\n");
}
