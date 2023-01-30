/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include "mpiimpl.h"

/* Added by Abu Naser(an16e@my.fsu.edu) */
unsigned char Ideciphertext[NON_BLOCKING_SND_RCV_1][NON_BLOCKING_SND_RCV_2];
int ranksIrecvS[MAX_RANKS_LIMIT];
/* End of add by Abu Naser */


/* -- Begin Profiling Symbol Block for routine MPI_Irecv */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_Irecv = PMPI_Irecv
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_Irecv  MPI_Irecv
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_Irecv as PMPI_Irecv
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag,
              MPI_Comm comm, MPI_Request * request) __attribute__ ((weak, alias("PMPI_Irecv")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_Irecv
#define MPI_Irecv PMPI_Irecv

#endif

#undef FUNCNAME
#define FUNCNAME MPI_Irecv
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
/*@
    MPI_Irecv - Begins a nonblocking receive

Input Parameters:
+ buf - initial address of receive buffer (choice)
. count - number of elements in receive buffer (integer)
. datatype - datatype of each receive buffer element (handle)
. source - rank of source (integer)
. tag - message tag (integer)
- comm - communicator (handle)

Output Parameters:
. request - communication request (handle)

.N ThreadSafe

.N Fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_TAG
.N MPI_ERR_RANK
.N MPI_ERR_EXHAUSTED
@*/
int MPI_Irecv_original(void *buf, int count, MPI_Datatype datatype, int source,
              int tag, MPI_Comm comm, MPI_Request * request)
{
#if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Recv rank = %d host = %s count = %d SA=%d] Func: MPI_Irecv_original\n", init_rank,hostname,count,security_approach);fflush(stdout);
	}
#endif 
    int mpi_errno = MPI_SUCCESS;
    MPIR_Comm *comm_ptr = NULL;
    MPIR_Request *request_ptr = NULL;
    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_IRECV);

    MPIR_ERRTEST_INITIALIZED_ORDIE();

    MPID_THREAD_CS_ENTER(VNI_GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    MPIR_FUNC_TERSE_PT2PT_ENTER_BACK(MPID_STATE_MPI_IRECV);

    /* Validate handle parameters needing to be converted */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPIR_ERRTEST_COMM(comm, mpi_errno);
        }
        MPID_END_ERROR_CHECKS;
    }
#endif /* HAVE_ERROR_CHECKING */

    /* Convert MPI object handles to object pointers */
    MPIR_Comm_get_ptr(comm, comm_ptr);

    /* Validate parameters if error checking is enabled */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPIR_Comm_valid_ptr(comm_ptr, mpi_errno, FALSE);
            if (mpi_errno)
                goto fn_fail;

            MPIR_ERRTEST_COUNT(count, mpi_errno);
            MPIR_ERRTEST_RECV_RANK(comm_ptr, source, mpi_errno);
            MPIR_ERRTEST_RECV_TAG(tag, mpi_errno);
            MPIR_ERRTEST_ARGNULL(request, "request", mpi_errno);

            /* Validate datatype handle */
            MPIR_ERRTEST_DATATYPE(datatype, "datatype", mpi_errno);

            /* Validate datatype object */
            if (HANDLE_GET_KIND(datatype) != HANDLE_KIND_BUILTIN) {
                MPIR_Datatype *datatype_ptr = NULL;

                MPIR_Datatype_get_ptr(datatype, datatype_ptr);
                MPIR_Datatype_valid_ptr(datatype_ptr, mpi_errno);
                if (mpi_errno)
                    goto fn_fail;
                MPIR_Datatype_committed_ptr(datatype_ptr, mpi_errno);
                if (mpi_errno)
                    goto fn_fail;
            }

            /* Validate buffer */
            MPIR_ERRTEST_USERBUFFER(buf, count, datatype, mpi_errno);
        }
        MPID_END_ERROR_CHECKS;
    }
#endif /* HAVE_ERROR_CHECKING */

    /* ... body of routine ...  */

    mpi_errno = MPID_Irecv(buf, count, datatype, source, tag, comm_ptr,
                           MPIR_CONTEXT_INTRA_PT2PT, &request_ptr);
    /* return the handle of the request to the user */
    /* MPIU_OBJ_HANDLE_PUBLISH is unnecessary for irecv, lower-level access is
     * responsible for its own consistency, while upper-level field access is
     * controlled by the completion counter */
    *request = request_ptr->handle;

    /* Put this part after setting the request so that if the request is
     * pending (which is still considered an error), it will still be set
     * correctly here. For real error cases, the user might get garbage as
     * their request value, but that's fine since the definition is
     * undefined anyway. */
    if (mpi_errno != MPI_SUCCESS)
        goto fn_fail;

    /* ... end of body of routine ... */

  fn_exit:
    MPIR_FUNC_TERSE_PT2PT_EXIT_BACK(MPID_STATE_MPI_IRECV);
    MPID_THREAD_CS_EXIT(VNI_GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    return mpi_errno;

  fn_fail:
    /* --BEGIN ERROR HANDLING-- */
#ifdef HAVE_ERROR_CHECKING
    {
        mpi_errno =
            MPIR_Err_create_code(mpi_errno, MPIR_ERR_RECOVERABLE, FCNAME, __LINE__, MPI_ERR_OTHER,
                                 "**mpi_irecv", "**mpi_irecv %p %d %D %i %t %C %p", buf, count,
                                 datatype, source, tag, comm, request);
    }
#endif
    mpi_errno = MPIR_Err_return_comm(comm_ptr, FCNAME, mpi_errno);
    goto fn_exit;
    /* --END ERROR HANDLING-- */
}


int MPI_SEC_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request * request)
{
#if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Recv rank = %d host = %s count = %d SA=%d] Func: MPI_SEC_Irecv\n", init_rank,hostname,count,security_approach);fflush(stdout);
	}
#endif     

    int mpi_errno = MPI_SUCCESS;
    MPI_Status sta;
    MPI_Request req;     

    unsigned long ciphertext_len=0;
    int  recvtype_sz; //, m, segments_no, totaldata;   
    // int th_data, th_start,i;

    MPIR_Comm *comm_ptr = NULL;
    MPIR_Comm_get_ptr(comm, comm_ptr);    

    MPI_Type_size(datatype, &recvtype_sz);    
    
    nonblock_req_handler[nonBlockCounter].source = source;
    nonblock_req_handler[nonBlockCounter].dest = comm_ptr->rank;;
    nonblock_req_handler[nonBlockCounter].tag = tag;
    nonblock_req_handler[nonBlockCounter].req_type = 2;
    nonblock_req_handler[nonBlockCounter].buffer = buf; 
    *request = nonBlockCounter;
    
   
   int req_counter = 0;
   int totaldata = recvtype_sz*count;
    mpi_errno = MPI_Irecv_original(&Ideciphertext[nonBlockCounter][0],  totaldata+MSG_HEADER_SIZE+16 , MPI_UNSIGNED_CHAR, source, tag, comm,  &nonblock_req_handler[nonBlockCounter].request[req_counter++]);
 
    
    if (security_approach == 401) 
        mpi_errno = MPI_Irecv_original(&Ideciphertext[nonBlockCounter][MSG_HEADER_SIZE], totaldata+(16+12), MPI_CHAR, source,
                                                tag, comm, &nonblock_req_handler[nonBlockCounter].request[req_counter++]);
    else if (security_approach == 402) 
        mpi_errno = MPI_Irecv_original(&Ideciphertext[nonBlockCounter][MSG_HEADER_SIZE], totaldata+(OCB_TAG_NONCE), MPI_CHAR, source,
                                                tag, comm, &nonblock_req_handler[nonBlockCounter].request[req_counter++]);                                            
  
    nonblock_req_handler[nonBlockCounter].segment_number = 1;
    nonblock_req_handler[nonBlockCounter].total_request = 2;

    nonBlockCounter++;
    if(nonBlockCounter == ISEND_REQ)
        nonBlockCounter = 0;
    
    return mpi_errno;

}



int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request * request)
{
#if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Recv rank = %d host = %s count = %d SA=%d] Func: MPI_Irecv\n", init_rank,hostname,count,security_approach);fflush(stdout);
	}
#endif     
    int mpi_errno = MPI_SUCCESS;

    if ((security_approach == 401  || security_approach == 402) && init_phase==0)  {            
                mpi_errno = MPI_SEC_Irecv(buf, count, datatype, source, tag, comm,request);
    } else      mpi_errno = MPI_Irecv_original(buf, count, datatype, source, tag, comm,request);

    return mpi_errno;
   
}