/* 
 *   $Id: fsyncf.c 3732 2005-07-14 14:11:52Z tobias $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "mpio.h"
#include "adio.h"


#if defined(MPIO_BUILD_PROFILING) || defined(HAVE_WEAK_SYMBOLS)
#ifdef FORTRANCAPS
#define mpi_file_sync_ PMPI_FILE_SYNC
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_file_sync_ pmpi_file_sync__
#elif !defined(FORTRANUNDERSCORE)
#define mpi_file_sync_ pmpi_file_sync
#else
#define mpi_file_sync_ pmpi_file_sync_
#endif

#if defined(HAVE_WEAK_SYMBOLS)
#if defined(HAVE_PRAGMA_WEAK)
#if defined(FORTRANCAPS)
#pragma weak MPI_FILE_SYNC = PMPI_FILE_SYNC
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma weak mpi_file_sync__ = pmpi_file_sync__
#elif !defined(FORTRANUNDERSCORE)
#pragma weak mpi_file_sync = pmpi_file_sync
#else
#pragma weak mpi_file_sync_ = pmpi_file_sync_
#endif

#elif defined(HAVE_ATTRIBUTE_WEAK)
#if defined(FORTRANCAPS)
void FORTRAN_API MPI_FILE_SYNC (MPI_Fint *fh, int *ierr ) __attribute__ ((weak, alias ("PMPI_FILE_SYNC")));
#elif defined(FORTRANDOUBLEUNDERSCORE)
void FORTRAN_API mpi_file_sync__ (MPI_Fint *fh, int *ierr ) __attribute__ ((weak, alias ("pmpi_file_sync__")));
#elif !defined(FORTRANUNDERSCORE)
void FORTRAN_API mpi_file_sync (MPI_Fint *fh, int *ierr ) __attribute__ ((weak, alias ("pmpi_file_sync")));
#else
void FORTRAN_API mpi_file_sync_ (MPI_Fint *fh, int *ierr ) __attribute__ ((weak, alias ("pmpi_file_sync_")));
#endif

#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#if defined(FORTRANCAPS)
#pragma _HP_SECONDARY_DEF PMPI_FILE_SYNC MPI_FILE_SYNC
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_file_sync__ mpi_file_sync__
#elif !defined(FORTRANUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_file_sync mpi_file_sync
#else
#pragma _HP_SECONDARY_DEF pmpi_file_sync_ mpi_file_sync_
#endif

#elif defined(HAVE_PRAGMA_CRI_DUP)
#if defined(FORTRANCAPS)
#pragma _CRI duplicate MPI_FILE_SYNC as PMPI_FILE_SYNC
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _CRI duplicate mpi_file_sync__ as pmpi_file_sync__
#elif !defined(FORTRANUNDERSCORE)
#pragma _CRI duplicate mpi_file_sync as pmpi_file_sync
#else
#pragma _CRI duplicate mpi_file_sync_ as pmpi_file_sync_
#endif

/* end of weak pragmas */
#endif
/* Include mapping from MPI->PMPI */
#include "mpioprof.h"
#endif

#else

#ifdef FORTRANCAPS
#define mpi_file_sync_ MPI_FILE_SYNC
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_file_sync_ mpi_file_sync__
#elif !defined(FORTRANUNDERSCORE)
#define mpi_file_sync_ mpi_file_sync
#endif
#endif

void FORTRAN_API mpi_file_sync_(MPI_Fint *fh, int *ierr )
{
    MPI_File fh_c;
    
    fh_c = MPI_File_f2c(*fh);
    *ierr = MPI_File_sync(fh_c);
}
