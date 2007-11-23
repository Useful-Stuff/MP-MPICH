/* 
 *   $Id: iwrite_atf.c 3744 2005-07-18 10:55:08Z georg $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "mpio.h"
#include "adio.h"


#if defined(MPIO_BUILD_PROFILING) || defined(HAVE_WEAK_SYMBOLS)
#ifdef FORTRANCAPS
#define mpi_file_iwrite_at_ PMPI_FILE_IWRITE_AT
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_file_iwrite_at_ pmpi_file_iwrite_at__
#elif !defined(FORTRANUNDERSCORE)
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF pmpi_file_iwrite_at pmpi_file_iwrite_at_
#endif
#define mpi_file_iwrite_at_ pmpi_file_iwrite_at
#else
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF pmpi_file_iwrite_at_ pmpi_file_iwrite_at
#endif
#define mpi_file_iwrite_at_ pmpi_file_iwrite_at_
#endif

#if defined(HAVE_WEAK_SYMBOLS)
#if defined(HAVE_PRAGMA_WEAK)
#if defined(FORTRANCAPS)
#pragma weak MPI_FILE_IWRITE_AT = PMPI_FILE_IWRITE_AT
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma weak mpi_file_iwrite_at__ = pmpi_file_iwrite_at__
#elif !defined(FORTRANUNDERSCORE)
#pragma weak mpi_file_iwrite_at = pmpi_file_iwrite_at
#else
#pragma weak mpi_file_iwrite_at_ = pmpi_file_iwrite_at_
#endif

#elif defined(HAVE_ATTRIBUTE_WEAK)
#if defined(FORTRANCAPS)
void FORTRAN_API MPI_FILE_IWRITE_AT (MPI_Fint *fh,MPI_Offset *offset,void *buf,
                       int *count,MPI_Fint *datatype,
                       MPI_Fint *request, int *ierr ) __attribute__ ((weak, alias
		        ("PMPI_FILE_IWRITE_AT")));
#elif defined(FORTRANDOUBLEUNDERSCORE)
void FORTRAN_API mpi_file_iwrite_at__ (MPI_Fint *fh,MPI_Offset *offset,void *buf,
                       int *count,MPI_Fint *datatype,
                       MPI_Fint *request, int *ierr ) __attribute__ ((weak, alias
		        ("pmpi_file_iwrite_at__")));
#elif !defined(FORTRANUNDERSCORE)
void FORTRAN_API mpi_file_iwrite_at (MPI_Fint *fh,MPI_Offset *offset,void *buf,
                       int *count,MPI_Fint *datatype,
                       MPI_Fint *request, int *ierr ) __attribute__ ((weak, alias
		        ("pmpi_file_iwrite_at")));
#else
void FORTRAN_API mpi_file_iwrite_at_ (MPI_Fint *fh,MPI_Offset *offset,void *buf,
                       int *count,MPI_Fint *datatype,
                       MPI_Fint *request, int *ierr ) __attribute__ ((weak, alias
		        ("pmpi_file_iwrite_at_")));
#endif

#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#if defined(FORTRANCAPS)
#pragma _HP_SECONDARY_DEF PMPI_FILE_IWRITE_AT MPI_FILE_IWRITE_AT
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_file_iwrite_at__ mpi_file_iwrite_at__
#elif !defined(FORTRANUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_file_iwrite_at mpi_file_iwrite_at
#else
#pragma _HP_SECONDARY_DEF pmpi_file_iwrite_at_ mpi_file_iwrite_at_
#endif

#elif defined(HAVE_PRAGMA_CRI_DUP)
#if defined(FORTRANCAPS)
#pragma _CRI duplicate MPI_FILE_IWRITE_AT as PMPI_FILE_IWRITE_AT
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _CRI duplicate mpi_file_iwrite_at__ as pmpi_file_iwrite_at__
#elif !defined(FORTRANUNDERSCORE)
#pragma _CRI duplicate mpi_file_iwrite_at as pmpi_file_iwrite_at
#else
#pragma _CRI duplicate mpi_file_iwrite_at_ as pmpi_file_iwrite_at_
#endif

/* end of weak pragmas */
#endif
/* Include mapping from MPI->PMPI */
#include "mpioprof.h"
#endif

#else

#ifdef FORTRANCAPS
#define mpi_file_iwrite_at_ MPI_FILE_IWRITE_AT
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_file_iwrite_at_ mpi_file_iwrite_at__
#elif !defined(FORTRANUNDERSCORE)
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF mpi_file_iwrite_at mpi_file_iwrite_at_
#endif
#define mpi_file_iwrite_at_ mpi_file_iwrite_at
#else
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF mpi_file_iwrite_at_ mpi_file_iwrite_at
#endif
#endif
#endif

#if defined(MPIHP) || defined(MPILAM)
void mpi_file_iwrite_at_(MPI_Fint *fh,MPI_Offset *offset,void *buf,
                       int *count,MPI_Fint *datatype,
                       MPI_Fint *request, int *ierr )
{
    MPI_File fh_c;
    MPIO_Request req_c;
    MPI_Datatype datatype_c;
    
    fh_c = MPI_File_f2c(*fh);
    datatype_c = MPI_Type_f2c(*datatype);

    *ierr = MPI_File_iwrite_at(fh_c,*offset,buf,*count,datatype_c,&req_c);
    *request = MPIO_Request_c2f(req_c);
}
#else
void FORTRAN_API mpi_file_iwrite_at_(MPI_Fint *fh,MPI_Offset *offset,void *buf,
                       int *count,MPI_Datatype *datatype,
                       MPI_Fint *request, int *ierr )
{
    MPI_File fh_c;
    MPIO_Request req_c;
    
    fh_c = MPI_File_f2c(*fh);
    *ierr = MPI_File_iwrite_at(fh_c,*offset,buf,*count,*datatype,&req_c);
    *request = MPIO_Request_c2f(req_c);
}
#endif
