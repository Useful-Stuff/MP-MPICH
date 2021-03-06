/* $Id$ */

#ifndef _SWITCH_TO_REPLICATION_H_
#define _SWITCH_TO_REPLICATION_H_

#include "env/general_definitions.h"

#ifdef __cplusplus
extern "C" {
#endif


smi_error_t _smi_id_to_region_struct(int, region_t**);
smi_error_t SMI_Switch_to_replication(int, int, int, int, int);

#ifdef __cplusplus
}
#endif


#endif
