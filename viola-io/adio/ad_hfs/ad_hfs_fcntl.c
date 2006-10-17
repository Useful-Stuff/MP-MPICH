/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id: ad_hfs_fcntl.c,v 1.1.1.1 2005/03/10 16:07:12 mhs Exp $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_hfs.h"
#include "adio_extern.h"

void ADIOI_HFS_Fcntl(ADIO_File fd, int flag, ADIO_Fcntl_t *fcntl_struct, int *error_code)
{
    int  i, ntimes, err;
    ADIO_Offset curr_fsize, alloc_size, size, len, done;
    ADIO_Status status;
    char *buf;
#ifndef PRINT_ERR_MSG
    static char myname[] = "ADIOI_HFS_FCNTL";
#endif

    switch(flag) {
    case ADIO_FCNTL_GET_FSIZE:
	fcntl_struct->fsize = lseek64(fd->fd_sys, 0, SEEK_END);
#ifdef HPUX
	if (fd->fp_sys_posn != -1) 
	     lseek64(fd->fd_sys, fd->fp_sys_posn, SEEK_SET);
/* not required in SPPUX since there we use pread/pwrite */
#endif
	if (fcntl_struct->fsize == -1) {
#ifdef MPICH2
	    *error_code = MPIR_Err_create_code(MPI_SUCCESS, MPIR_ERR_RECOVERABLE, myname, __LINE__, MPI_ERR_IO, "**io", 
		"**io %s", strerror(errno));
#elif defined(PRINT_ERR_MSG)
		*error_code = MPI_ERR_UNKNOWN;
#else /* MPICH-1 */
		*error_code = MPIR_Err_setmsg(MPI_ERR_IO, MPIR_ADIO_ERROR,
				myname, "I/O Error", "%s", strerror(errno));
		ADIOI_Error(fd, *error_code, myname);	    
#endif
	}
	else *error_code = MPI_SUCCESS;
	break;

    case ADIO_FCNTL_SET_DISKSPACE:
	/* will be called by one process only */

#ifdef HPUX
	err = prealloc64(fd->fd_sys, fcntl_struct->diskspace);
	/* prealloc64 works only if file is of zero length */
	if (err && (errno != ENOTEMPTY)) {
#ifdef MPICH2
	    *error_code = MPIR_Err_create_code(MPI_SUCCESS, MPIR_ERR_RECOVERABLE, myname, __LINE__, MPI_ERR_IO, "**io", 
		"**io %s", strerror(errno));
#elif defined(PRINT_ERR_MSG)
	    *error_code = MPI_ERR_UNKNOWN;
#else
	    *error_code = MPIR_Err_setmsg(MPI_ERR_IO, MPIR_ADIO_ERROR,
			      myname, "I/O Error", "%s", strerror(errno));
	    ADIOI_Error(fd, *error_code, myname);
#endif
	    return;
	}
	if (err && (errno == ENOTEMPTY)) {
#endif

#ifdef SPPUX
	/* SPPUX has no prealloc64. therefore, use prealloc
           if size < (2GB - 1), otherwise use long method. */
        if (fcntl_struct->diskspace <= 2147483647) {
	    err = prealloc(fd->fd_sys, (off_t) fcntl_struct->diskspace);
	    if (err && (errno != ENOTEMPTY)) {
#ifdef MPICH2
		*error_code = MPIR_Err_create_code(MPI_SUCCESS, MPIR_ERR_RECOVERABLE, myname, __LINE__, MPI_ERR_IO, "**io",
		    "**io %s", strerror(errno));
#elif defined(PRINT_ERR_MSG)
    	        *error_code = MPI_ERR_UNKNOWN;
#else
		*error_code = MPIR_Err_setmsg(MPI_ERR_IO, MPIR_ADIO_ERROR,
			      myname, "I/O Error", "%s", strerror(errno));
		ADIOI_Error(fd, *error_code, myname);
#endif
	        return;
	    }
	}    

	if ((fcntl_struct->diskspace > 2147483647) || 
	    (err && (errno == ENOTEMPTY))) {
#endif

        
	/* Explicitly write to allocate space. Since there could be
	   holes in the file, I need to read up to the current file
	   size, write it back, and then write beyond that depending
	   on how much preallocation is needed.
           read/write in sizes of no more than ADIOI_PREALLOC_BUFSZ */

	    curr_fsize = lseek64(fd->fd_sys, 0, SEEK_END);
	    alloc_size = fcntl_struct->diskspace;

	    size = ADIOI_MIN(curr_fsize, alloc_size);
	    
	    ntimes = (size + ADIOI_PREALLOC_BUFSZ - 1)/ADIOI_PREALLOC_BUFSZ;
	    buf = (char *) ADIOI_Malloc(ADIOI_PREALLOC_BUFSZ);
	    done = 0;

	    for (i=0; i<ntimes; i++) {
		len = ADIOI_MIN(size-done, ADIOI_PREALLOC_BUFSZ);
		ADIO_ReadContig(fd, buf, len, MPI_BYTE, ADIO_EXPLICIT_OFFSET, 
                      done, &status, error_code);
		if (*error_code != MPI_SUCCESS) {
#ifdef MPICH2
		    *error_code = MPIR_Err_create_code(MPI_SUCCESS, MPIR_ERR_RECOVERABLE, myname, __LINE__, MPI_ERR_IO, 
			"**iopreallocrdwr", 0);
#elif defined(PRINT_ERR_MSG)
		    FPRINTF(stderr, "ADIOI_HFS_Fcntl: To preallocate disk space, ROMIO needs to read the file and write it back, but is unable to read the file. Please give the file read permission and open it with MPI_MODE_RDWR.\n");
		    MPI_Abort(MPI_COMM_WORLD, 1);
#else /* MPICH-1 */
		    *error_code = MPIR_Err_setmsg(MPI_ERR_IO, MPIR_PREALLOC_PERM,
			      myname, (char *) 0, (char *) 0);
		    ADIOI_Error(fd, *error_code, myname);
#endif
		    return;
		}
		ADIO_WriteContig(fd, buf, len, MPI_BYTE, ADIO_EXPLICIT_OFFSET,
                         done,  &status, error_code);
		if (*error_code != MPI_SUCCESS) return;
		done += len;
	    }

	    if (alloc_size > curr_fsize) {
		memset(buf, 0, ADIOI_PREALLOC_BUFSZ); 
		size = alloc_size - curr_fsize;
		ntimes = (size + ADIOI_PREALLOC_BUFSZ - 1)/ADIOI_PREALLOC_BUFSZ;
		for (i=0; i<ntimes; i++) {
		    len = ADIOI_MIN(alloc_size-done, ADIOI_PREALLOC_BUFSZ);
		    ADIO_WriteContig(fd, buf, len, MPI_BYTE, ADIO_EXPLICIT_OFFSET, 
				     done, &status, error_code);
		    if (*error_code != MPI_SUCCESS) return;
		    done += len;  
		}
	    }
	    ADIOI_Free(buf);
#ifdef HPUX
	    if (fd->fp_sys_posn != -1) 
		lseek64(fd->fd_sys, fd->fp_sys_posn, SEEK_SET);
	    /* not required in SPPUX since there we use pread/pwrite */
#endif
	}
	*error_code = MPI_SUCCESS;
	break;

    case ADIO_FCNTL_SET_IOMODE:
        /* for implementing PFS I/O modes. will not occur in MPI-IO
           implementation.*/
	if (fd->iomode != fcntl_struct->iomode) {
	    fd->iomode = fcntl_struct->iomode;
	    MPI_Barrier(MPI_COMM_WORLD);
	}
	*error_code = MPI_SUCCESS;
	break;

    case ADIO_FCNTL_SET_ATOMICITY:
	fd->atomicity = (fcntl_struct->atomicity == 0) ? 0 : 1;
	*error_code = MPI_SUCCESS;
	break;

    default:
	FPRINTF(stderr, "Unknown flag passed to ADIOI_HFS_Fcntl\n");
	MPI_Abort(MPI_COMM_WORLD, 1);
    }
}
