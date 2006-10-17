/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id: ad_ntfs_resize.c,v 1.1.1.1 2005/03/10 16:07:12 mhs Exp $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_ntfs.h"

void ADIOI_NTFS_Resize(ADIO_File fd, ADIO_Offset size, int *error_code)
{
	DWORD dwTemp;
    int err;
#if defined(MPICH2) || !defined(PRINT_ERR_MSG)
    static char myname[] = "ADIOI_NTFS_RESIZE";
#endif

	dwTemp = DWORDHIGH(size);
	err = SetFilePointer(fd->fd_sys, DWORDLOW(size), &dwTemp, FILE_BEGIN);
    err = SetEndOfFile(fd->fd_sys);
    if (err == FALSE) {
#ifdef MPICH2 
			*error_code = MPIR_Err_create_code(MPI_SUCCESS, MPIR_ERR_RECOVERABLE, myname, __LINE__, MPI_ERR_IO, "**io",
							"**io %s", strerror(errno));
			return;
#elif defined(PRINT_ERR_MSG)
			*error_code = MPI_ERR_UNKNOWN;
#else
	*error_code = MPIR_Err_setmsg(MPI_ERR_IO, MPIR_ADIO_ERROR,
			      myname, "I/O Error", "%s", strerror(errno));
	ADIOI_Error(fd, *error_code, myname);	    
#endif
    }
    else *error_code = MPI_SUCCESS;
}
