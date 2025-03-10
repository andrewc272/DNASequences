#include <stdio.h>
#include <stdlib.h>
#include "proj2.h"

int main ( int argumentCount, char *argumentValues[] ) {

	acceptArguments(argumentCount, argumentValues);
	
	return 0;
}

void acceptArguments( int argumentCount, char *argumentValues[] ) {
	if ( argumentCount == 4 ){
		sequence_file = argumentValues[1];
		subsequence_file = argumentValues[2];
		process_num = atoi(argumentValues[3]);
		printArguments();
	}
	else printf("ERROR 1: Invalid Number of Arguments\n");
}

void printArguments( void ){
	printf("sequence_file: %s\n", sequence_file);
	printf("subsequence_file: %s\n", subsequence_file);
	printf("process_num: %d\n", process_num);
}

