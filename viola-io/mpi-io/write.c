/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id: write.c,v 1.2 2005/03/30 11:52:32 mhs Exp $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "mpioimpl.h"

#ifdef HAVE_WEAK_SYMBOLS

#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_File_write = PMPI_File_write
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_File_write MPI_File_write
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_File_write as PMPI_File_write
/* end of weak pragmas */
#endif

/* Include mapping from MPI->PMPI */
#define MPIO_BUILD_PROFILING
#include "mpioprof.h"
#endif

/* status object not filled currently */

/*@
    MPI_File_write - Write using individual file pointer

Input Parameters:
. fh - file handle (handle)
. buf - initial address of buffer (choice)
. count - number of elements in buffer (nonnegative integer)
. datatype - datatype of each buffer element (handle)

Output Parameters:
. status - status object (Status)

.N fortran
@*/
int MPI_File_write(MPI_File fh, void *buf, int count, 
                   MPI_Datatype datatype, MPI_Status *status)
{
    int error_code, bufsize, buftype_is_contig, filetype_is_contig;
#if defined(MPICH2) || !defined(PRINT_ERR_MSG)
    static char myname[] = "MPI_FILE_WRITE";
#endif
    int datatype_size;
    ADIO_Offset off = 0;
#ifdef MPI_hpux
    int fl_xmpi;

    HPMP_IO_START(fl_xmpi, BLKMPIFILEWRITE, TRDTBLOCK, fh, datatype, count);
#endif /* MPI_hpux */

#ifdef PRINT_ERR_MSG
    if ((fh <= (MPI_File) 0) || (fh->cookie != ADIOI_FILE_COOKIE)) {
	FPRINTF(stderr, "MPI_File_write: Invalid file handle\n");
	MPI_Abort(MPI_COMM_WORLD, 1);
    }
#else
    ADIOI_TEST_FILE_HANDLE(fh, myname);
#endif

    if (count < 0) {
#ifdef MPICH2
	error_code = MPIR_Err_create_code(MPI_SUCCESS, MPIR_ERR_RECOVERABLE, myname, __LINE__, MPI_ERR_ARG, 
	    "**iobadcount", 0);
	return MPIR_Err_return_file(fh, myname, error_code);
#elif defined(PRINT_ERR_MSG)
	FPRINTF(stderr, "MPI_File_write: Invalid count argument\n");
	MPI_Abort(MPI_COMM_WORLD, 1);
#else /* MPICH-1 */
	error_code = MPIR_Err_setmsg(MPI_ERR_ARG, MPIR_ERR_COUNT_ARG,
				     myname, (char *) 0, (char *) 0);
	return ADIOI_Error(fh, error_code, myname);
#endif
    }

    if (datatype == MPI_DATATYPE_NULL) {
#ifdef MPICH2
	error_code = MPIR_Err_create_code(MPI_SUCCESS, MPIR_ERR_RECOVERABLE, myname, __LINE__, MPI_ERR_TYPE, 
	    "**dtypenull", 0);
	return MPIR_Err_return_file(fh, myname, error_code);
#elif defined(PRINT_ERR_MSG)
        FPRINTF(stderr, "MPI_File_write: Invalid datatype\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
#else /* MPICH-1 */
	error_code = MPIR_Err_setmsg(MPI_ERR_TYPE, MPIR_ERR_TYPE_NULL,
				     myname, (char *) 0, (char *) 0);
	return ADIOI_Error(fh, error_code, myname);	    
#endif
}

    MPI_Type_size(datatype, &datatype_size);
    if (count*datatype_size == 0) {
#ifdef MPI_hpux
	HPMP_IO_END(fl_xmpi, fh, datatype, count);
#endif /* MPI_hpux */
#ifdef HAVE_STATUS_SET_BYTES
       MPIR_Status_set_bytes(status, datatype, 0);
#endif
	return MPI_SUCCESS;
    }

    if ((count*datatype_size) % fh->etype_size != 0) {
#ifdef MPICH2
	error_code = MPIR_Err_create_code(MPI_SUCCESS, MPIR_ERR_RECOVERABLE, myname, __LINE__, MPI_ERR_IO, 
	    "**ioetype", 0);
	return MPIR_Err_return_file(fh, myname, error_code);
#elif defined(PRINT_ERR_MSG)
	FPRINTF(stderr, "MPI_File_write: Only an integral number of etypes can be accessed\n");
	MPI_Abort(MPI_COMM_WORLD, 1);
#else /* MPICH-1 */
	error_code = MPIR_Err_setmsg(MPI_ERR_IO, MPIR_ERR_ETYPE_FRACTIONAL,
				     myname, (char *) 0, (char *) 0);
	return ADIOI_Error(fh, error_code, myname);	    
#endif
    }

    if (fh->access_mode & MPI_MODE_SEQUENTIAL) {
#ifdef MPICH2
	error_code = MPIR_Err_create_code(MPI_SUCCESS, MPIR_ERR_RECOVERABLE, myname, __LINE__, MPI_ERR_UNSUPPORTED_OPERATION,
	    "**ioamodeseq", 0);
	return MPIR_Err_return_file(fh, myname, error_code);
#elif defined(PRINT_ERR_MSG)
	FPRINTF(stderr, "MPI_File_write: Can't use this function because file was opened with MPI_MODE_SEQUENTIAL\n");
	MPI_Abort(MPI_COMM_WORLD, 1);
#else /* MPICH-1 */
	error_code = MPIR_Err_setmsg(MPI_ERR_UNSUPPORTED_OPERATION, 
                        MPIR_ERR_AMODE_SEQ, myname, (char *) 0, (char *) 0);
	return ADIOI_Error(fh, error_code, myname);
#endif
    }

    if (fh->access_mode & MPI_MODE_RDONLY) {
#ifdef MPICH2
	error_code = MPIR_Err_create_code(MPI_SUCCESS, MPIR_ERR_RECOVERABLE, myname, __LINE__, MPI_ERR_READ_ONLY,
	    "**filerdonly", "**filerdonly %s", fh->filename );
	return MPIR_Err_return_file(fh, myname, error_code);
#elif defined(PRINT_ERR_MSG)
	FPRINTF(stderr, "MPI_File_write: Can't use this function because file was opened with MPI_MODE_RDONLY\n");
	MPI_Abort(MPI_COMM_WORLD, 1);
#else /* MPICH-1 */
	error_code = MPIR_Err_setmsg(MPI_ERR_READ_ONLY, 
                        0, myname, (char *) 0, (char *) 0);
	return ADIOI_Error(fh, error_code, myname);
#endif
    }

    ADIOI_Datatype_iscontig(datatype, &buftype_is_contig);
    ADIOI_Datatype_iscontig(fh->filetype, &filetype_is_contig);
    
    ADIOI_TEST_DEFERRED(fh, "MPI_File_write", &error_code);

    /* contiguous or strided? */

    if (buftype_is_contig && filetype_is_contig) {
	bufsize = datatype_size * count;
        /* if atomic mode requested, lock (exclusive) the region, because there
           could be a concurrent noncontiguous request. Locking doesn't 
           work on PIOFS and PVFS, and on NFS it is done in the ADIO_WriteContig.*/
        /*off = fh->fp_ind;*/
        ADIOI_Get_position(fh, &off);
        if ((fh->atomicity) && (fh->file_system != ADIO_PIOFS) && 
            (fh->file_system != ADIO_PVFS) && (fh->file_system != ADIO_NFS)
            && (fh->file_system != ADIO_PVFS2) 
            && (fh->file_system != ADIO_TUNNELFS)
            && (fh->file_system != ADIO_MEMFS))
            ADIOI_WRITE_LOCK(fh, off, SEEK_SET, bufsize);

        if (fh->file_system == ADIO_TUNNELFS)
            ADIO_WriteContig(fh, buf, count, datatype, ADIO_INDIVIDUAL, off,
                             status, &error_code);
        else
            ADIO_WriteContig(fh, buf, count, datatype, ADIO_INDIVIDUAL, 0, 
                             status, &error_code);

        if ((fh->atomicity) && (fh->file_system != ADIO_PIOFS) && 
            (fh->file_system != ADIO_PVFS) && (fh->file_system != ADIO_NFS) 
            && (fh->file_system != ADIO_PVFS2)
            && (fh->file_system != ADIO_TUNNELFS)
            && (fh->file_system != ADIO_MEMFS))
            ADIOI_UNLOCK(fh, off, SEEK_SET, bufsize);
    }
    else
    {
        if (fh->file_system != ADIO_TUNNELFS)
            ADIO_WriteStrided(fh, buf, count, datatype, ADIO_INDIVIDUAL, 0, 
                              status, &error_code);
        else
        {
            ADIOI_Get_position(fh, &off);
            /* if buffer is contiguous, we can use the contiguous transfer to
             * tunnelfs server. If not, we have to do some data sieving first. */
            if (buftype_is_contig)
                ADIO_WriteContig(fh, buf, count, datatype, ADIO_INDIVIDUAL,
                                 off, status, &error_code);
            else
                ADIO_WriteStrided(fh, buf, count, datatype, ADIO_INDIVIDUAL,
                                  off, status, &error_code);
        }
    }
    /* For strided and atomic mode, locking is done in ADIO_WriteStrided */

#ifdef MPI_hpux
    HPMP_IO_END(fl_xmpi, fh, datatype, count);
#endif /* MPI_hpux */
    return error_code;
}
