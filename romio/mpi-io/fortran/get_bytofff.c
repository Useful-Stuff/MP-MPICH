/* 
 *   $Id: get_bytofff.c 3732 2005-07-14 14:11:52Z tobias $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "mpio.h"
#include "adio.h"


#if defined(MPIO_BUILD_PROFILING) || defined(HAVE_WEAK_SYMBOLS)
#ifdef FORTRANCAPS
#define mpi_file_get_byte_offset_ PMPI_FILE_GET_BYTE_OFFSET
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_file_get_byte_offset_ pmpi_file_get_byte_offset__
#elif !defined(FORTRANUNDERSCORE)
#define mpi_file_get_byte_offset_ pmpi_file_get_byte_offset
#else
#define mpi_file_get_byte_offset_ pmpi_file_get_byte_offset_
#endif

#if defined(HAVE_WEAK_SYMBOLS)
#if defined(HAVE_PRAGMA_WEAK)
#if defined(FORTRANCAPS)
#pragma weak MPI_FILE_GET_BYTE_OFFSET = PMPI_FILE_GET_BYTE_OFFSET
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma weak mpi_file_get_byte_offset__ = pmpi_file_get_byte_offset__
#elif !defined(FORTRANUNDERSCORE)
#pragma weak mpi_file_get_byte_offset = pmpi_file_get_byte_offset
#else
#pragma weak mpi_file_get_byte_offset_ = pmpi_file_get_byte_offset_
#endif

#elif defined(HAVE_ATTRIBUTE_WEAK)
#if defined(FORTRANCAPS)
void FORTRAN_API MPI_FILE_GET_BYTE_OFFSET (MPI_Fint *fh,MPI_Offset *offset,MPI_Offset *disp, int *ierr ) __attribute__ ((weak, alias ("PMPI_FILE_GET_BYTE_OFFSET")));
#elif defined(FORTRANDOUBLEUNDERSCORE)
void FORTRAN_API mpi_file_get_byte_offset__ (MPI_Fint *fh,MPI_Offset *offset,MPI_Offset *disp, int *ierr ) __attribute__ ((weak, alias ("pmpi_file_get_byte_offset__")));
#elif !defined(FORTRANUNDERSCORE)
void FORTRAN_API mpi_file_get_byte_offset (MPI_Fint *fh,MPI_Offset *offset,MPI_Offset *disp, int *ierr ) __attribute__ ((weak, alias ("pmpi_file_get_byte_offset")));
#else
void FORTRAN_API mpi_file_get_byte_offset_ (MPI_Fint *fh,MPI_Offset *offset,MPI_Offset *disp, int *ierr ) __attribute__ ((weak, alias ("pmpi_file_get_byte_offset_")));
#endif

#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#if defined(FORTRANCAPS)
#pragma _HP_SECONDARY_DEF PMPI_FILE_GET_BYTE_OFFSET MPI_FILE_GET_BYTE_OFFSET
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_file_get_byte_offset__ mpi_file_get_byte_offset__
#elif !defined(FORTRANUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_file_get_byte_offset mpi_file_get_byte_offset
#else
#pragma _HP_SECONDARY_DEF pmpi_file_get_byte_offset_ mpi_file_get_byte_offset_
#endif

#elif defined(HAVE_PRAGMA_CRI_DUP)
#if defined(FORTRANCAPS)
#pragma _CRI duplicate MPI_FILE_GET_BYTE_OFFSET as PMPI_FILE_GET_BYTE_OFFSET
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _CRI duplicate mpi_file_get_byte_offset__ as pmpi_file_get_byte_offset__
#elif !defined(FORTRANUNDERSCORE)
#pragma _CRI duplicate mpi_file_get_byte_offset as pmpi_file_get_byte_offset
#else
#pragma _CRI duplicate mpi_file_get_byte_offset_ as pmpi_file_get_byte_offset_
#endif

/* end of weak pragmas */
#endif
/* Include mapping from MPI->PMPI */
#include "mpioprof.h"
#endif

#else

#ifdef FORTRANCAPS
#define mpi_file_get_byte_offset_ MPI_FILE_GET_BYTE_OFFSET
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_file_get_byte_offset_ mpi_file_get_byte_offset__
#elif !defined(FORTRANUNDERSCORE)
#define mpi_file_get_byte_offset_ mpi_file_get_byte_offset
#endif
#endif

void FORTRAN_API mpi_file_get_byte_offset_(MPI_Fint *fh,MPI_Offset *offset,MPI_Offset *disp, int *ierr )
{
    MPI_File fh_c;
    
    fh_c = MPI_File_f2c(*fh);
    *ierr = MPI_File_get_byte_offset(fh_c,*offset,disp);
}
