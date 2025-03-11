
int process_num;
char *sequence_file, *subsequence_file;
char *sequence, *subsequence;

// PASSING IN ARGUMENTS
void acceptArguments( int argc, char *argv[] );


void printArguments( void );

char* txtToString( char * filename );

void runDNASequenser( void );

typedef struct {
    sem_t sem;  // Semaphore to protect the shared memory
    int data[2];  // Two integers in shared memory
} SharedMemory;
