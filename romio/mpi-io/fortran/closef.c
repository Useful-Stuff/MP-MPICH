/* 
 *   $Id: closef.c 3732 2005-07-14 14:11:52Z tobias $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "mpio.h"
#include "adio.h"


#if defined(MPIO_BUILD_PROFILING) || defined(HAVE_WEAK_SYMBOLS)
#ifdef FORTRANCAPS
#define mpi_file_close_ PMPI_FILE_CLOSE
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_file_close_ pmpi_file_close__
#elif !defined(FORTRANUNDERSCORE)
#define mpi_file_close_ pmpi_file_close
#else
#define mpi_file_close_ pmpi_file_close_
#endif

#if defined(HAVE_WEAK_SYMBOLS)
#if defined(HAVE_PRAGMA_WEAK)
#if defined(FORTRANCAPS)
#pragma weak MPI_FILE_CLOSE = PMPI_FILE_CLOSE
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma weak mpi_file_close__ = pmpi_file_close__
#elif !defined(FORTRANUNDERSCORE)
#pragma weak mpi_file_close = pmpi_file_close
#else
#pragma weak mpi_file_close_ = pmpi_file_close_
#endif

#elif defined(HAVE_ATTRIBUTE_WEAK)
#if defined(FORTRANCAPS)
void FORTRAN_API MPI_FILE_CLOSE (MPI_Fint *fh, int *ierr ) __attribute__ ((weak, alias ("PMPI_FILE_CLOSE")));
#elif defined(FORTRANDOUBLEUNDERSCORE)
void FORTRAN_API mpi_file_close__ (MPI_Fint *fh, int *ierr ) __attribute__ ((weak, alias ("pmpi_file_close__")));
#elif !defined(FORTRANUNDERSCORE)
void FORTRAN_API mpi_file_close (MPI_Fint *fh, int *ierr ) __attribute__ ((weak, alias ("pmpi_file_close")));
#else
void FORTRAN_API mpi_file_close_ (MPI_Fint *fh, int *ierr ) __attribute__ ((weak, alias ("pmpi_file_close_")));
#endif

#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#if defined(FORTRANCAPS)
#pragma _HP_SECONDARY_DEF PMPI_FILE_CLOSE MPI_FILE_CLOSE
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_file_close__ mpi_file_close__
#elif !defined(FORTRANUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_file_close mpi_file_close
#else
#pragma _HP_SECONDARY_DEF pmpi_file_close_ mpi_file_close_
#endif

#elif defined(HAVE_PRAGMA_CRI_DUP)
#if defined(FORTRANCAPS)
#pragma _CRI duplicate MPI_FILE_CLOSE as PMPI_FILE_CLOSE
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _CRI duplicate mpi_file_close__ as pmpi_file_close__
#elif !defined(FORTRANUNDERSCORE)
#pragma _CRI duplicate mpi_file_close as pmpi_file_close
#else
#pragma _CRI duplicate mpi_file_close_ as pmpi_file_close_
#endif

/* end of weak pragmas */
#endif
/* Include mapping from MPI->PMPI */
#include "mpioprof.h"
#endif

#else

#ifdef FORTRANCAPS
#define mpi_file_close_ MPI_FILE_CLOSE
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_file_close_ mpi_file_close__
#elif !defined(FORTRANUNDERSCORE)
#define mpi_file_close_ mpi_file_close
#else
#endif
#endif

void FORTRAN_API mpi_file_close_(MPI_Fint *fh, int *ierr )
{
    MPI_File fh_c;

    fh_c = MPI_File_f2c(*fh);
    *ierr = MPI_File_close(&fh_c);
    *fh = MPI_File_c2f(fh_c);
}
