/*
 *  $Id: comm_rank.c,v 1.2 2003/06/05 11:50:33 rainer Exp $
 *
 *  (C) 1993 by Argonne National Laboratory and Mississipi State University.
 *      See COPYRIGHT in top-level directory.
 */


#include "mpiimpl.h"

#ifdef HAVE_WEAK_SYMBOLS

#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_Comm_rank = PMPI_Comm_rank
#elif defined(HAVE_ATTRIBUTE_WEAK)
EXPORT_MPI_API int MPI_Comm_rank ( MPI_Comm comm, int *rank ) __attribute__ ((weak, alias ("PMPI_Comm_rank")));
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_Comm_rank  MPI_Comm_rank
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_Comm_rank as PMPI_Comm_rank
/* end of weak pragmas */
#endif

/* Include mapping from MPI->PMPI */
#define MPI_BUILD_PROFILING
#include "mpiprof.h"
/* Insert the prototypes for the PMPI routines */
#undef __MPI_BINDINGS
#include "binding.h"
#endif

/*@

MPI_Comm_rank - Determines the rank of the calling process in the communicator

Input Parameters:
. comm - communicator (handle) 

Output Parameter:
. rank - rank of the calling process in group of  'comm'  (integer) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
@*/
EXPORT_MPI_API int MPI_Comm_rank ( MPI_Comm comm, int *rank )
{
    struct MPIR_COMMUNICATOR *comm_ptr;
    static char myname[] = "MPI_COMM_RANK";
    int mpi_errno;

    TR_PUSH(myname);

    comm_ptr = MPIR_GET_COMM_PTR(comm);
    MPIR_TEST_MPI_COMM(comm,comm_ptr,comm_ptr,myname );

    (*rank) = comm_ptr->local_group->local_rank;

    TR_POP;
    return (MPI_SUCCESS);
}
