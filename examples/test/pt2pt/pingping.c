/* pingping - based on pingpong, but sending at the same time
			using asynchronous calls MPI_Isend */

#include <stdio.h>

#include "mpi.h"

#define MAXSIZE (512*1024)
#define MINSIZE (0)
#define INCSIZE (2)
#define INCOP   *
#define REPEAT  50

char *buffer;
int min_size, max_size, inc_size, repeats;

void ping1 (int to, int from);
void ping2( int from );

int main(int argc, char **argv) {
  MPI_Status status;
  int myrank, mysize;
  int first;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &mysize);

  if(mysize % 2 != 0) {
    printf ("pingping must be used with an even number of processes (currently %d processes). \n",mysize);
    fprintf( stderr, "Aborting\n");fflush(stderr);
	MPI_Abort (MPI_COMM_WORLD, 0);
  }

  if (argc != 4) {
      if (!myrank) {
	  printf ("# usage: %s min_size max_size repeats\n", argv[0]);
	  printf ("# using default values for this run\n");
      }

      min_size = MINSIZE;
      max_size = MAXSIZE;
      inc_size = INCSIZE;
      repeats  = REPEAT;
  } else {
      min_size = atoi( argv[1] );
      max_size = atoi( argv[2] );
      inc_size = INCSIZE;
      repeats  = atoi( argv[3] );
  }
  
  buffer = (char *)malloc(max_size);

  if ( myrank < mysize/2 ) {
      if (myrank % 2 == 0)
	  ping1(  myrank + mysize/2, myrank);
      else
	  ping2( myrank + mysize/2 );
  } else {
      first = (mysize/2) % 2;
      if (myrank % 2 == first)
	  ping2( myrank - mysize/2 );
      else 
	  ping1( myrank - mysize/2, myrank );
  }
  free (buffer);
  MPI_Finalize();
  return 0;
}




void ping1( int to, int from ) {
	MPI_Status status;
	double starttime, totaltime;
	int i, j;
	MPI_Request handle;
	printf("pingping with MPI_Isend nonblocking send\n");
	printf("# blocksize \tbandwidth [MB/s]\t latency [ms]\n");

	for (i = min_size; i <= max_size; i = (i == 0) ? i + 1 : i INCOP inc_size) {
		MPI_Barrier(MPI_COMM_WORLD);

		starttime = MPI_Wtime();
		for( j = 0; j < repeats; j++) {
			MPI_Isend( buffer, i, MPI_CHAR, to, 1, MPI_COMM_WORLD,&handle);
			MPI_Recv( buffer, i, MPI_CHAR, to, 2, MPI_COMM_WORLD, &status);
			MPI_Wait(&handle,&status);
		}   
		totaltime = MPI_Wtime() - starttime;

		printf("%7d \t %10.3f \t %7.4f\n",i,i*repeats/(totaltime*1024*1024),totaltime/repeats/2*1000);
		fflush(stdout);
	}
}  





void ping2( int from ) {
	MPI_Status status;
	int i, j;
	MPI_Request handle;

	for( i = min_size; i <= max_size; i = (i == 0) ? i + 1 : i INCOP inc_size) {
		MPI_Barrier(MPI_COMM_WORLD);

		for(j = 0; j < repeats; j++) {
			MPI_Isend( buffer, i, MPI_CHAR, from, 2, MPI_COMM_WORLD,&handle);
			MPI_Recv( buffer, i, MPI_CHAR, from, 1, MPI_COMM_WORLD, &status);	
			MPI_Wait(&handle,&status);
		}
	}  
}






