/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id$    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_ntfs.h"

#if (_MSC_VER >=1400)
#pragma warning (disable:4996)
#endif

void ADIOI_NTFS_Close(ADIO_File fd, int *error_code)
{
    int err;
    static char myname[] = "ADIOI_NTFS_CLOSE";
    
    err = CloseHandle(fd->fd_sys);
    if (err == FALSE) {
	*error_code = MPIO_Err_create_code(MPI_SUCCESS, MPIR_ERR_RECOVERABLE,
					   myname, __LINE__, MPI_ERR_IO,
					   "**io",
					   "**io %s", strerror(errno));
	return;
    }
    else *error_code = MPI_SUCCESS;
}
