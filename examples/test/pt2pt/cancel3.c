/*
 * This file shows a typical use of MPI_Cancel to free Persistent Send's that
 * are not wanted.  We check for both successful and unsuccessful 
 * cancels
 */

#include "mpi.h"
#include <stdio.h>

#if defined(NEEDS_STDLIB_PROTOTYPES)
#include "protofix.h"
#endif

int main( int argc, char **argv )
{
    MPI_Request r1;
    int         size, rank;
    int         err = 0;
    int         partner, buf[10], flag, idx, index;
    MPI_Status  status;

    MPI_Init( &argc, &argv );

    MPI_Comm_size( MPI_COMM_WORLD, &size );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    
    if (size < 2) {
	  printf( "Cancel test requires at least 2 processes\n" );
	  fprintf( stderr, "[%i] Aborting\n",rank );fflush(stderr);
	  MPI_Abort( MPI_COMM_WORLD, 1 );
    }

    /* 
     * Here is the test.  First, we ensure an unsatisfied Irecv:
     *       process 0             process size-1
     *       Sendrecv              Sendrecv
     *       Irecv                    ----
     *       Cancel                   ----
     *       Sendrecv              Sendrecv
     * Next, we confirm receipt before canceling
     *       Irecv                 Send
     *       Sendrecv              Sendrecv
     *       Cancel
     */
	if (rank == 0) {
		partner = size - 1;
		/* Cancel succeeds for wait/waitall */
		printf( "[%i]: Cancel for wait/waitall\n",rank);fflush(stdout);
		MPI_Send_init( buf, 10, MPI_INT, partner, 0, MPI_COMM_WORLD, &r1 );
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );
		MPI_Start( &r1 );
		MPI_Cancel( &r1 );
		MPI_Wait( &r1, &status );
		MPI_Test_cancelled( &status, &flag ); 
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );
		if (!flag) {
			err++; 
			printf( "[%i]: Cancel of a send failed where it should succeed (Wait).\n" ,rank);
		}
		MPI_Request_free( &r1 ); 

		printf( "[%i]: Cancel for test/testall\n",rank);fflush(stdout);
		/* Cancel fails for test/testall */
		buf[0] = 3;
		MPI_Send_init( buf, 3, MPI_INT, partner, 2, MPI_COMM_WORLD, &r1 );
		MPI_Start( &r1 );
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );
		MPI_Cancel( &r1 );
		MPI_Test( &r1, &flag, &status );
		MPI_Test_cancelled( &status, &flag );
		if (flag) {
			err++;
			printf( "[%i]: Cancel of a send succeeded where it shouldn't (Test).\n" ,rank);
		}
		MPI_Request_free( &r1 );

		printf( "[%i]: Cancel for waitany\n",rank);fflush(stdout);
		/* Cancel succeeds for waitany */
		MPI_Send_init( buf, 10, MPI_INT, partner, 0, MPI_COMM_WORLD, &r1 );
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );
		MPI_Start( &r1 );
		MPI_Cancel( &r1 );
		MPI_Waitany( 1, &r1, &idx, &status );
		MPI_Test_cancelled( &status, &flag );
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );
		if (!flag) {
			err++;
			printf( "[%i]: Cancel of a send failed where it should succeed (Waitany).\n" ,rank);
		}
		MPI_Request_free( &r1 );

		printf( "[%i]: Cancel for testany\n",rank);fflush(stdout);
		/* Cancel fails for testany */
		buf[0] = 3;
		MPI_Send_init( buf, 3, MPI_INT, partner, 2, MPI_COMM_WORLD, &r1 );
		MPI_Start( &r1 );
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );
		MPI_Cancel( &r1 );
		MPI_Testany( 1, &r1, &idx, &flag, &status );
		MPI_Test_cancelled( &status, &flag );
		if (flag) {
			err++;
			printf( "[%i]: Cancel of a send succeeded where it shouldn't (Testany).\n" ,rank);
		}
		MPI_Request_free( &r1 );

		printf( "[%i]: Cancel for waitsome\n",rank);fflush(stdout);
		/* Cancel succeeds for waitsome */
		MPI_Send_init( buf, 10, MPI_INT, partner, 0, MPI_COMM_WORLD, &r1 );
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );
		MPI_Start( &r1 );
		MPI_Cancel( &r1 );
		MPI_Waitsome( 1, &r1, &idx, &index, &status );
		MPI_Test_cancelled( &status, &flag );
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );
		if (!flag) {
			err++;
			printf( "[%i]: Cancel of a send failed where it should succeed (Waitsome).\n",rank );
		}
		MPI_Request_free( &r1 );

		printf( "[%i]: Cancel for testsome\n",rank);fflush(stdout);
		/* Cancel fails for testsome*/
		buf[0] = 3;
		MPI_Send_init( buf, 3, MPI_INT, partner, 2, MPI_COMM_WORLD, &r1 );
		MPI_Start( &r1 );
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );
		printf("[%i]: Calling MPI_Cancel\n",rank);fflush(stdout);
		MPI_Cancel( &r1 );
		printf("[%i]: Calling MPI_Testsome\n",rank);fflush(stdout);
		MPI_Testsome( 1, &r1, &idx, &index, &status );
		printf("[%i]: Calling MPI_Test_cancelled\n",rank);fflush(stdout);
		MPI_Test_cancelled( &status, &flag );
		if (flag) {
			err++;
			printf( "Cancel of a send succeeded where it shouldn't (Testsome).\n" ,rank);
		}
		MPI_Request_free( &r1 );

		if (err) {
			printf( "Test failed with %d errors.\n", err );
		}
		else {
			printf( " No Errors\n" );
		}
	}
	else if (rank == size - 1) {
		partner = 0;
		/* Cancel succeeds for wait/waitall */
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );

		/* Cancel fails for test/testall */
		buf[0] = -1;
		MPI_Recv( buf, 3, MPI_INT, partner, 2, MPI_COMM_WORLD, &status );
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );

		if (buf[0] == -1) {
			printf( "Receive buffer did not change even though cancel should not have suceeded! (Test).\n" );
		}

		/* Cancel succeeds for waitany */
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );
		/* Cancel fails  for testany */
		buf[0] = -1;
		MPI_Recv( buf, 3, MPI_INT, partner, 2, MPI_COMM_WORLD, &status );
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );
		if (buf[0] == -1) {
			printf( "Receive buffer did not change even though cancel should not have suceeded! (Testany).\n" );
		}

		/* Cancel succeeds for waitsome */
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );
		/* Cancel fails for testsome */
		buf[0] = -1;
		MPI_Recv( buf, 3, MPI_INT, partner, 2, MPI_COMM_WORLD, &status );
		MPI_Sendrecv( MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_BOTTOM, 0, MPI_INT, partner, 1,
			MPI_COMM_WORLD, &status );

		if (buf[0] == -1) {
			printf( "Receive buffer did not change even though cancel should not have succeeded! (Test).\n" );
		}

	}
	printf("[%i]: Calling MPI_Finalize\n",rank);fflush(stdout);
	MPI_Finalize();
	printf("[%i]: finish\n",rank);fflush(stdout);
	return 0;
}


