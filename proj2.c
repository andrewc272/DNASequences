#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <semaphore.h>
#include <sys/wait.h>
#include "proj2.h"

int main ( int argumentCount, char *argumentValues[] ) {

	acceptArguments(argumentCount, argumentValues);
	
	runDNASequenser();

	return 0;
}

void acceptArguments( int argumentCount, char *argumentValues[] ) {
	if ( argumentCount == 4 ){
		sequence_file = argumentValues[1];
		subsequence_file = argumentValues[2];
		process_num = atoi(argumentValues[3]);
		sequence = txtToString(sequence_file);
		subsequence = txtToString(subsequence_file);
		//printArguments();
	}
	else printf("ERROR 1: Invalid Number of Arguments\n");
}

void printArguments( void ){
	printf("sequence_file: %s\n", sequence_file);
	printf("subsequence_file: %s\n", subsequence_file);
	printf("process_num: %d\n", process_num);
}

char* txtToString( char *filename ) {
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		printf("Could not open file %s for reading.\n", filename);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	rewind(file);

	char *content = (char*)malloc((file_size + 1) * sizeof(char));
	if (content == NULL) {
		printf("Memory allocation failed.\n");
		fclose(file);
		return NULL;
	}

	fread(content, sizeof(char), file_size, file);
	content[file_size] = '\0';
	fclose(file);
	return content;
}


void runDNASequenser( void ){
	SharedMemory *shm = mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    	if (shm == MAP_FAILED) {
        	perror("mmap failed");
        	exit(1);
    	}

    	// Initialize semaphore
    	if (sem_init(&shm->sem, 1, 1) == -1) { // Initialize semaphore (1 for shared between processes)
        	perror("sem_init failed");
        	exit(1);
    	}

    	// Initialize shared memory data
    	shm->data[0] = -1;  // position
   	shm->data[1] = -1;  // count

	// fork
	int i;
	for(i = 0; i < process_num; ++i ){
		if (fork() == 0){
			for ( int pos = i; pos < strlen(sequence); pos = pos + process_num){
				int matches = 0;
				for ( int k = 0; k < strlen(subsequence); k++){
					if ( pos + k >= strlen(sequence)) k = strlen(subsequence);
					else if (subsequence[k] == sequence[pos + k]) matches++;
				}

				// post data if better
				sem_wait(&shm->sem);
				//printf("pos %d matches %d\n", pos, matches);
				if ( matches > shm->data[1] ) {
					shm->data[0] = pos;
					shm->data[1] = matches;
				}
				sem_post(&shm->sem);
			}
			exit(0);
		}
		else{
			wait(NULL);
		}
	}

	sem_destroy(&shm->sem);

	printf("Number of Processes:\t%d\n", process_num);
	printf("Best Match Position:\t%d\n", shm->data[0]);
	printf("Best Match Count:\t%d\n", shm->data[1]);

    	// Unmap the shared memory
    	munmap(shm, sizeof(SharedMemory));

}







