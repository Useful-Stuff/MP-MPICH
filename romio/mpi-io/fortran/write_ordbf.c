/* 
 *   $Id: write_ordbf.c 3744 2005-07-18 10:55:08Z georg $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "mpio.h"
#include "adio.h"


#if defined(MPIO_BUILD_PROFILING) || defined(HAVE_WEAK_SYMBOLS)
#ifdef FORTRANCAPS
#define mpi_file_write_ordered_begin_ PMPI_FILE_WRITE_ORDERED_BEGIN
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_file_write_ordered_begin_ pmpi_file_write_ordered_begin__
#elif !defined(FORTRANUNDERSCORE)
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF pmpi_file_write_ordered_begin pmpi_file_write_ordered_begin_
#endif
#define mpi_file_write_ordered_begin_ pmpi_file_write_ordered_begin
#else
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF pmpi_file_write_ordered_begin_ pmpi_file_write_ordered_begin
#endif
#define mpi_file_write_ordered_begin_ pmpi_file_write_ordered_begin_
#endif

#if defined(HAVE_WEAK_SYMBOLS)
#if defined(HAVE_PRAGMA_WEAK)
#if defined(FORTRANCAPS)
#pragma weak MPI_FILE_WRITE_ORDERED_BEGIN = PMPI_FILE_WRITE_ORDERED_BEGIN
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma weak mpi_file_write_ordered_begin__ = pmpi_file_write_ordered_begin__
#elif !defined(FORTRANUNDERSCORE)
#pragma weak mpi_file_write_ordered_begin = pmpi_file_write_ordered_begin
#else
#pragma weak mpi_file_write_ordered_begin_ = pmpi_file_write_ordered_begin_
#endif

#elif defined(HAVE_ATTRIBUTE_WEAK)
#if defined(FORTRANCAPS)
void FORTRAN_API MPI_FILE_WRITE_ORDERED_BEGIN (MPI_Fint *fh,void *buf,int *count,
                       MPI_Fint *datatype, int *ierr ) 
		       __attribute__ ((weak, alias ("PMPI_FILE_WRITE_ORDERED_BEGIN")));
#elif defined(FORTRANDOUBLEUNDERSCORE)
void FORTRAN_API mpi_file_write_ordered_begin__ (MPI_Fint *fh,void *buf,int *count,
                       MPI_Fint *datatype, int *ierr ) 
		       __attribute__ ((weak, alias ("pmpi_file_write_ordered_begin__")));
#elif !defined(FORTRANUNDERSCORE)
void FORTRAN_API mpi_file_write_ordered_begin (MPI_Fint *fh,void *buf,int *count,
                       MPI_Fint *datatype, int *ierr ) 
		       __attribute__ ((weak, alias ("pmpi_file_write_ordered_begin")));
#else
void FORTRAN_API mpi_file_write_ordered_begin_ (MPI_Fint *fh,void *buf,int *count,
                       MPI_Fint *datatype, int *ierr ) 
		       __attribute__ ((weak, alias ("pmpi_file_write_ordered_begin_")));
#endif

#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#if defined(FORTRANCAPS)
#pragma _HP_SECONDARY_DEF PMPI_FILE_WRITE_ORDERED_BEGIN MPI_FILE_WRITE_ORDERED_BEGIN
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_file_write_ordered_begin__ mpi_file_write_ordered_begin__
#elif !defined(FORTRANUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_file_write_ordered_begin mpi_file_write_ordered_begin
#else
#pragma _HP_SECONDARY_DEF pmpi_file_write_ordered_begin_ mpi_file_write_ordered_begin_
#endif

#elif defined(HAVE_PRAGMA_CRI_DUP)
#if defined(FORTRANCAPS)
#pragma _CRI duplicate MPI_FILE_WRITE_ORDERED_BEGIN as PMPI_FILE_WRITE_ORDERED_BEGIN
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _CRI duplicate mpi_file_write_ordered_begin__ as pmpi_file_write_ordered_begin__
#elif !defined(FORTRANUNDERSCORE)
#pragma _CRI duplicate mpi_file_write_ordered_begin as pmpi_file_write_ordered_begin
#else
#pragma _CRI duplicate mpi_file_write_ordered_begin_ as pmpi_file_write_ordered_begin_
#endif

/* end of weak pragmas */
#endif
/* Include mapping from MPI->PMPI */
#include "mpioprof.h"
#endif

#else

#ifdef FORTRANCAPS
#define mpi_file_write_ordered_begin_ MPI_FILE_WRITE_ORDERED_BEGIN
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_file_write_ordered_begin_ mpi_file_write_ordered_begin__
#elif !defined(FORTRANUNDERSCORE)
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF mpi_file_write_ordered_begin mpi_file_write_ordered_begin_
#endif
#define mpi_file_write_ordered_begin_ mpi_file_write_ordered_begin
#else
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF mpi_file_write_ordered_begin_ mpi_file_write_ordered_begin
#endif
#endif
#endif

#if defined(MPIHP) || defined(MPILAM)
void mpi_file_write_ordered_begin_(MPI_Fint *fh,void *buf,int *count,
                       MPI_Fint *datatype, int *ierr ){
    MPI_File fh_c;
    MPI_Datatype datatype_c;
    
    fh_c = MPI_File_f2c(*fh);
    datatype_c = MPI_Type_f2c(*datatype);

    *ierr = MPI_File_write_ordered_begin(fh_c,buf,*count,datatype_c);
}
#else
void FORTRAN_API mpi_file_write_ordered_begin_(MPI_Fint *fh,void *buf,int *count,
                       MPI_Datatype *datatype, int *ierr ){
    MPI_File fh_c;
    
    fh_c = MPI_File_f2c(*fh);
    *ierr = MPI_File_write_ordered_begin(fh_c,buf,*count,*datatype);
}
#endif
