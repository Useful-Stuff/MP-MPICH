/*
 *  $Id$
 *
 */

#include "mpid.h"
#include "mpiddev.h"
#include "shdef.h"
#include "mpimem.h"
#include "reqalloc.h"
#include "shpackets.h"
#include "ntshmemdebug.h"
#include "../util/queue.h"

/*
 * This file contains the routines to handle canceling a message
 * Special for the ntshmem device. Each device has its own version of this
 */

extern int expect_cancel_ack;
/* 
 * CancelSend 
 * This is fairly hard.  We need to send a "please_cancel_send", 
 * which, if the message is found in the unexpected queue, removes it.
 * However, if the message is being received at the "same" moment, the
 * ok_to_send and cancel_send messages could cross.  To handle this, the
 * receiver must ack the cancel_send message (making the success of the
 * cancel non-local).  There are even more complex protocols, but we won't
 * bother.
 * 
 * Don't forget to update MPID_n_pending as needed.
 */

/* This routine is called from MPI_Cancel.  Its purpose is to send an    
   anti-send packet to the calling process's partner.  If successful,
   the error_code will return MPI_SUCCESS, otherwise the error_code will
   return MPI_ERR_OTHER */
int MPID_SHMEM_SendCancelPacket( shandle )
MPIR_SHANDLE *shandle;

{  /* begin MPID_SendCancelPacket */
	
	
	MPID_PKT_ANTI_SEND_TSH cancel_pkt; /* type -> typeSH */
	MPID_PKT_ANTI_SEND_TSH *pkt = &cancel_pkt; /* type -> typeSH */
	
	pkt->mode    = MPID_PKT_ANTI_SENDSH; /* type -> typeSH */
	pkt->lrank   = MPID_MyWorldRank;
/*	pkt->to      = shandle->partner;*/
	MPID_AINT_SET(pkt->send_id,shandle); 
	
	MPID_SHMEM_SendControl( (MPID_PKT_TSH*)pkt, sizeof(MPID_PKT_ANTI_SEND_TSH), shandle->partner ); /* type -> typeSH */
	expect_cancel_ack++; 
	return 0;
}  /* end MPID_SendCancelPacket */


/* This routine is called when a process receives an anti-send pkt.  Its 
   purpose is to search for the request found in the pkt in the unexpected
   queue.  If found, set the pkt.cancel to 1, otherwise, set pkt.cancel to 
   0.  Send this information back in an anti-send-ok pkt. */
void MPID_SHMEM_SendCancelOkPacket( in_pkt, from )
MPID_PKT_TSH  *in_pkt;/* type -> typeSH */
int  from;

{  /* begin MPID_SendCancelOkPacket */

  MPID_PKT_ANTI_SEND_TSH *pkt = (MPID_PKT_ANTI_SEND_TSH *)in_pkt; /* type -> typeSH */
  MPID_PKT_ANTI_SEND_TSH new_pkt; /* type -> typeSH */

  int error_code;
  int found = 0;

  MPIR_SHANDLE *shandle=0;
  MPIR_RHANDLE *rhandle;


  MPID_AINT_GET(shandle, pkt->send_id); 

  /* Look for request, if found, delete it */
  error_code = MPID_Search_unexpected_for_request(shandle, &rhandle, &found);

  if ( (error_code != MPI_SUCCESS) || (found == 0) || !rhandle->cancel ) {
	  DEBUG_PRINT_MSG2("Could not find cancel function, shandle == %p",shandle);
      new_pkt.cancel = 0;
  } else {
	  DEBUG_PRINT_MSG("Calling rhandle->cancel");
	  (rhandle->cancel)(rhandle);  
      new_pkt.cancel = 1;
  }

  new_pkt.mode = MPID_PKT_ANTI_SEND_OKSH;/* type -> typeSH */
  new_pkt.lrank = MPID_myid;
  /*new_pkt.to = from; */
  new_pkt.send_id = pkt->send_id;  

  MPID_PKT_READY_CLR(&(pkt->ready));

  MPID_SHMEM_SendControl( (MPID_PKT_TSH*)&new_pkt, sizeof(MPID_PKT_ANTI_SEND_TSH), from ); /* type -> typeSH */

}  /* end MPID_SendCancelOkPacket */

/* This routine is called when a process receives an anti-send-ok packet.
   If pkt->cancel = 1, then set the request found in the pkt as
   cancelled and complete.  If pkt->cancel = 0, do nothing. */
void MPID_SHMEM_RecvCancelOkPacket( in_pkt, from )
MPID_PKT_TSH  *in_pkt;/* type -> typeSH */
int  from;

{  /* begin MPID_RecvCancelOkPacket */

   MPID_PKT_ANTI_SEND_TSH *pkt = (MPID_PKT_ANTI_SEND_TSH *)in_pkt;/* type -> typeSH */

   MPIR_SHANDLE *shandle=0;
  

  MPID_AINT_GET(shandle, pkt->send_id);

  DEBUG_PRINT_BASIC_SEND_PKT("R Receive anti-send ok message\n", pkt);  

  if (pkt->cancel) {   /* begin if pkt->cancel */
      /* Mark the request as cancelled */
      shandle->s.MPI_TAG = MPIR_MSG_CANCELLED; 
      /* Mark the request as complete */
      shandle->is_complete = 1;
      shandle->is_cancelled = 1;
      MPID_n_pending--;  
	  if(shandle->cancel)
		  (shandle->cancel)(shandle);
      DEBUG_PRINT_MSG("Request has been successfully cancelled");
  }   /* end if pkt->cancel */
  else {
      shandle->is_cancelled = 0;
      DEBUG_PRINT_MSG("Unable to cancel request");
  }

  MPID_PKT_READY_CLR(&(pkt->ready));
  
  shandle->s.MPI_ERROR = MPI_SUCCESS;
  shandle->cancel_complete = 1;

  expect_cancel_ack--;

}  /* end MPID_RecvCancelOkPacket */



