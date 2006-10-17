/*
* This program should be run with at least 8 nodes just to (un)fair
*
* Patrick Bridges * bridges@mcs.anl.gov * patrick@CS.MsState.Edu 
*/

#include <stdio.h>
#include "test.h"
#include "mpi.h"
#include "mpe.h"

int main(argc, argv)
int argc;
char **argv;
{
	int rank, size, an_int; 
	char *Current_Test = NULL;
	int *num_array, i, j;
	MPI_Status Status;

	MPI_Init(&argc, &argv);
	MPE_Init_log();
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	Test_Init("fairness2m", rank);

	/* Wait for everyone to be ready */

	if (rank == 0) { 
		/* Initialize an array to keep statistics in */
		num_array = (int *)malloc((size - 1) * sizeof(int));

		/* Make sure everyone is ready */
		MPI_Barrier(MPI_COMM_WORLD);

		/* Wait for all of the senders to send all of their messages */
		Test_Message("Waiting for all of the senders to say they're through.");
		for (i = 0 ; i < size - 1; i++) {
			MPI_Recv(&an_int, 1, MPI_INT, MPI_ANY_SOURCE, 5000,
				MPI_COMM_WORLD, &Status);
			MPE_Log_receive(Status.MPI_SOURCE, 5000, sizeof(int));
		}
		Test_Message("Starting to dequeue messages...");
		/* Now start dequeuing messages */
		for (i = 0; i < size - 1; i++) {
			/* Clear the buffer of counts */
			memset(num_array, 0, (size - 1) * sizeof(int));
			for (j = 0; j < 200; j++) {
				MPI_Recv(&an_int, 1, MPI_INT, MPI_ANY_SOURCE, 2000, 
					MPI_COMM_WORLD, &Status);
				MPE_Log_receive(Status.MPI_SOURCE, 2000, sizeof(int));
				num_array[Status.MPI_SOURCE - 1]++;
			}
			Test_Printf("Statistics for message group %d:\n", i + 1);
			for (j = 0; j < size -1 ; j++)
				Test_Printf("%f%% of last 200 messages received \
							were from source %d.\n",
							num_array[j]/2.0, j + 1);
		}

		free(num_array);
		(void)Summarize_Test_Results();
		Print_Filecontent(rank,TRUE);

#ifdef WIN32
		MPE_Finish_log("./fairness2m.log");
#else
		MPE_Finish_log("/home/bridges/fairness2m.log");
#endif
		MPI_Finalize();
		printf("No Errors\n");

	} else {
		MPI_Request ReqArray[200];
		MPI_Status StatArray[200];

		MPI_Barrier(MPI_COMM_WORLD);
		an_int = rank;

		Test_Message("About to send all of the little messages.");
		/* Send 200 tiny messages - nonblocking so we don't deadlock */
		for (i = 0; i < 200; i++) {
			MPI_Isend(&an_int, 1, MPI_INT, 0, 2000, MPI_COMM_WORLD, 
				&ReqArray[i]);
			MPE_Log_send(0, 2000, sizeof(int));
		}
		Test_Message("Sending the final message.");
		/* Tell receiver we've sent all of our messages */
		MPI_Send(&an_int, 1, MPI_INT, 0, 5000, MPI_COMM_WORLD);
		MPE_Log_send(0, 5000, sizeof(int));

		Test_Message("Waiting on the nonblocking requests.");
		MPI_Waitall(200,ReqArray,StatArray);
		(void)Summarize_Test_Results();

#ifdef WIN32
		MPE_Finish_log("./fairness2m.log");
#else
		MPE_Finish_log("/home/bridges/fairness2m.log");
#endif
		Delete_Outfile(rank);
		MPI_Finalize();
	}

	return 0;
}




