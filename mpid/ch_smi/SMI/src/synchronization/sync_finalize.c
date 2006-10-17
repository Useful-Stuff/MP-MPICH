/* $Id: sync_finalize.c,v 1.1 2004/03/19 22:14:23 joachim Exp $ */

#include "env/general_definitions.h"
#include "sync_finalize.h"
#include "barrier.h"
#include "mutex.h"

/*********************************************************************************/
/*** This function destroies all ellements of the arraies all_barrier          ***/
/*** and all_mutex.                                                            ***/
/*********************************************************************************/ 
int _smi_synchronization_finalize(void)
{
  _smi_signal_finalize();
  _smi_mutex_module_finalize();
  _smi_barrier_module_finalize();
  
  return(SMI_SUCCESS);
}
