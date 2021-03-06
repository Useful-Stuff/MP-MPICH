// Copyright 1997-2000, University of Notre Dame.
// Authors: Jeremy G. Siek, Jeffery M. Squyres, Michael P. McNally, and
//          Andrew Lumsdaine
// 
// This file is part of the Notre Dame C++ bindings for MPI.
// 
// You should have received a copy of the License Agreement for the Notre
// Dame C++ bindings for MPI along with the software; see the file
// LICENSE.  If not, contact Office of Research, University of Notre
// Dame, Notre Dame, IN 46556.
// 
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
// 
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
// 
// Additional copyrights may follow.
/****************************************************************************

 MESSAGE PASSING INTERFACE TEST CASE SUITE

 Copyright IBM Corp. 1995

 IBM Corp. hereby grants a non-exclusive license to use, copy, modify, and
 distribute this software for any purpose and without fee provided that the
 above copyright notice and the following paragraphs appear in all copies.

 IBM Corp. makes no representation that the test cases comprising this
 suite are correct or are an accurate representation of any standard.

 In no event shall IBM be liable to any party for direct, indirect, special
 incidental, or consequential damage arising out of the use of this software
 even if IBM Corp. has been advised of the possibility of such damage.

 IBM CORP. SPECIFICALLY DISCLAIMS ANY WARRANTIES INCLUDING, BUT NOT LIMITED
 TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS AND IBM
 CORP. HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 ENHANCEMENTS, OR MODIFICATIONS.

****************************************************************************

 These test cases reflect an interpretation of the MPI Standard.  They are
 are, in most cases, unit tests of specific MPI behaviors.  If a user of any
 test case from this set believes that the MPI Standard requires behavior
 different than that implied by the test case we would appreciate feedback.

 Comments may be sent to:
    Richard Treumann
    treumann@kgn.ibm.com

****************************************************************************
*/
#include "mpi2c++_test.h"

void
testsome()
{
  char msg[150];
  int data[2000];
  int i;
  int index[2000];
  int outcount;
  MPI::Request req[2000];
  MPI::Status status[2000];

  for(i = 0; i < 2000; i++) {
    data[i] = -1;
    index[i] = -1;
    req[i] = MPI::REQUEST_NULL;
  }

  Testing("Testsome w/ Status");

  {
    if(my_rank > 0) 
      MPI::COMM_WORLD.Send(&my_rank, 1, MPI::INT, 0, 1);
    else {
      for(i = 1; i < comm_size; i++)  
	req[i] = MPI::COMM_WORLD.Irecv(&data[i], 1, MPI::INT, i, 1);
      
      outcount = MPI::Request::Testsome(comm_size, req, index, status);  
      if(outcount != 0)
	for(i = 0; i < outcount; i++)  {
	  if(index[i] == MPI::UNDEFINED) {
	    sprintf(msg, "NODE %d - ) ERROR in MPI::Testsome: index[%d] == MPI::UNDEFINED", my_rank, i);
	    Fail(msg);
	  }
	  if(req[index[i]] != MPI::REQUEST_NULL) {
	    sprintf(msg, "NODE %d - ) ERROR in MPI::Testsome: request[%d] not set to MPI::REQUEST_NULL", my_rank, index[i]);
	    Fail(msg);
	  }
	  if(data[index[i]] != index[i]) {
	    sprintf(msg, "NODE %d - ) ERROR in MPI::Testsome: data[%d] is %d, should be %d", my_rank, index[i], data[index[i]], index[i]);
	    Fail(msg);
	  }
	  if(status[i].Get_tag() != 1) {
	    sprintf(msg, "NODE %d - ) ERROR in MPI::Testsome, status.Get_tag() = %d should be 1", my_rank, status[i].Get_tag());
	    Fail(msg);
	  }
	}
    }
    Pass(); // Testsome w/ Status
  }

  for (i = 0; i < 2000; i++) {
    data[i] = -1;
    index[i] = -1;
    if (req[i] != MPI::REQUEST_NULL) {
      req[i].Cancel();
      req[i].Wait();
    }
  }
  
  Testing("Testsome w/o Status");

  {
    if(my_rank > 0) 
      MPI::COMM_WORLD.Send(&my_rank, 1, MPI::INT, 0, 2);
    else {
      for(i = 1; i < comm_size; i++)  
	req[i] = MPI::COMM_WORLD.Irecv(&data[i], 1, MPI::INT, i, 2);
      
      outcount = MPI::Request::Testsome(comm_size, req, index);  
      if(outcount != 0)
	for(i = 0; i < outcount; i++)  {
	  if(index[i] == MPI::UNDEFINED) {
	    sprintf(msg, "NODE %d - ) ERROR in MPI::Testsome: index[%d] == MPI::UNDEFINED", my_rank, i);
	    Fail(msg);
	  }
	  if(req[index[i]] != MPI::REQUEST_NULL) {
	    sprintf(msg, "NODE %d - ) ERROR in MPI::Testsome: request[%d] not set to MPI::REQUEST_NULL", my_rank, index[i]);
	    Fail(msg);
	  }
	  if(data[index[i]] != index[i]) {
	    sprintf(msg, "NODE %d - ) ERROR in MPI::Testsome: data[%d] is %d, should be %d", my_rank, index[i], data[index[i]], index[i]);
	    Fail(msg);
	  }
	}
    }
    Pass(); // Testsome w/o Status
  }

  for (i = 0; i < 2000; i++)
    if (req[i] != MPI::REQUEST_NULL) {
      req[i].Cancel();
      req[i].Wait();
    }
}
