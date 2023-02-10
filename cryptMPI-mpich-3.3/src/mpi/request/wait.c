/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include "mpiimpl.h"

/* -- Begin Profiling Symbol Block for routine MPI_Wait */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_Wait = PMPI_Wait
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_Wait  MPI_Wait
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_Wait as PMPI_Wait
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_Wait(MPI_Request * request, MPI_Status * status) __attribute__ ((weak, alias("PMPI_Wait")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_Wait
#define MPI_Wait PMPI_Wait

#undef FUNCNAME
#define FUNCNAME MPIR_Wait_impl
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
int MPIR_Wait_impl(MPIR_Request * request_ptr, MPI_Status * status)
{
    int mpi_errno = MPI_SUCCESS;
    MPID_Progress_state progress_state;
    if (request_ptr == NULL)
        goto fn_exit;

    MPID_Progress_start(&progress_state);
    while (!MPIR_Request_is_complete(request_ptr)) {
        mpi_errno = MPID_Progress_wait(&progress_state);
        if (mpi_errno) {
            /* --BEGIN ERROR HANDLING-- */
            MPID_Progress_end(&progress_state);
            MPIR_ERR_POP(mpi_errno);
            /* --END ERROR HANDLING-- */
        }

        if (unlikely(MPIR_Request_is_anysrc_mismatched(request_ptr))) {
            mpi_errno = MPIR_Request_handle_proc_failed(request_ptr);
            goto fn_fail;
        }
    }
    MPID_Progress_end(&progress_state);

  fn_exit:
    return mpi_errno;
  fn_fail:
    goto fn_exit;
}

#undef FUNCNAME
#define FUNCNAME MPIR_Wait
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
int MPIR_Wait(MPI_Request * request, MPI_Status * status)
{
    int mpi_errno = MPI_SUCCESS;
    int active_flag;
    MPIR_Request *request_ptr = NULL;

    /* If this is a null request handle, then return an empty status */
    if (*request == MPI_REQUEST_NULL) {
        MPIR_Status_set_empty(status);
        goto fn_exit;
    }

    MPIR_Request_get_ptr(*request, request_ptr);
    MPIR_Assert(request_ptr != NULL);

    if (!MPIR_Request_is_complete(request_ptr)) {
        /* If this is an anysource request including a communicator with
         * anysource disabled, convert the call to an MPI_Test instead so we
         * don't get stuck in the progress engine. */
        if (unlikely(MPIR_Request_is_anysrc_mismatched(request_ptr))) {
            mpi_errno = MPIR_Test(request, &active_flag, status);
            goto fn_exit;
        }

        if (MPIR_Request_has_poll_fn(request_ptr)) {
            while (!MPIR_Request_is_complete(request_ptr)) {
                mpi_errno = MPIR_Grequest_poll(request_ptr, status);
                if (mpi_errno)
                    MPIR_ERR_POP(mpi_errno);

                /* Avoid blocking other threads since I am inside an infinite loop */
                MPID_THREAD_CS_YIELD(GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
            }
        } else {
            mpi_errno = MPID_Wait(request_ptr, status);            
            if (mpi_errno)
                MPIR_ERR_POP(mpi_errno);
        }
    }

    mpi_errno = MPIR_Request_completion_processing(request_ptr, status, &active_flag);
    if (!MPIR_Request_is_persistent(request_ptr)) {
        MPIR_Request_free(request_ptr);
        *request = MPI_REQUEST_NULL;
    }
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);

  fn_exit:    
    return mpi_errno;
  fn_fail:
    goto fn_exit;
}

#endif

#undef FUNCNAME
#define FUNCNAME MPI_Wait
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
/*@
    MPI_Wait - Waits for an MPI request to complete

Input Parameters:
. request - request (handle)

Output Parameters:
. status - status object (Status).  May be 'MPI_STATUS_IGNORE'.

.N waitstatus

.N ThreadSafe

.N Fortran

.N FortranStatus

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_REQUEST
.N MPI_ERR_ARG
@*/

int MPI_Wait_original(MPI_Request * request, MPI_Status * status)
{
    #if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Wait rank = %d host = %s SA=%d] Func: MPI_Wait_original\n", init_rank,hostname,security_approach);fflush(stdout);
	}
#endif 
    MPIR_Request *request_ptr = NULL;
    int mpi_errno = MPI_SUCCESS;
    MPIR_Comm *comm_ptr = NULL;
    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_WAIT);

    MPIR_ERRTEST_INITIALIZED_ORDIE();

    MPID_THREAD_CS_ENTER(VNI_GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    MPIR_FUNC_TERSE_REQUEST_ENTER(MPID_STATE_MPI_WAIT);

    /* Check the arguments */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPIR_ERRTEST_ARGNULL(request, "request", mpi_errno);
            /* NOTE: MPI_STATUS_IGNORE != NULL */
            MPIR_ERRTEST_ARGNULL(status, "status", mpi_errno);
            MPIR_ERRTEST_REQUEST_OR_NULL(*request, mpi_errno);
        }
        MPID_END_ERROR_CHECKS;
    }
#endif /* HAVE_ERROR_CHECKING */    

    /* If this is a null request handle, then return an empty status */
    if (*request == MPI_REQUEST_NULL) {
        MPIR_Status_set_empty(status);
        goto fn_exit;
    }

    /* Convert MPI request handle to a request object pointer */
    MPIR_Request_get_ptr(*request, request_ptr);

    /* Validate object pointers if error checking is enabled */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPIR_Request_valid_ptr(request_ptr, mpi_errno);
            if (mpi_errno)
                goto fn_fail;
        }
        MPID_END_ERROR_CHECKS;
    }
#endif /* HAVE_ERROR_CHECKING */

    /* ... body of routine ... */

    /* save copy of comm because request will be freed */
    if (request_ptr)
        comm_ptr = request_ptr->comm;

    mpi_errno = MPIR_Wait(request, status); 

    if (mpi_errno)
        goto fn_fail;

    /* ... end of body of routine ... */

  fn_exit:
    MPIR_FUNC_TERSE_REQUEST_EXIT(MPID_STATE_MPI_WAIT);
    MPID_THREAD_CS_EXIT(VNI_GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);

    return mpi_errno;

  fn_fail:
    /* --BEGIN ERROR HANDLING-- */
#ifdef HAVE_ERROR_CHECKING
    mpi_errno = MPIR_Err_create_code(mpi_errno, MPIR_ERR_RECOVERABLE,
                                     FCNAME, __LINE__, MPI_ERR_OTHER,
                                     "**mpi_wait", "**mpi_wait %p %p", request, status);
#endif
    mpi_errno = MPIR_Err_return_comm(comm_ptr, FCNAME, mpi_errno);
    goto fn_exit;
    /* --END ERROR HANDLING-- */
}
// ==============================================================================

int MPI_SEC_Wait_Isend(MPI_Request *request, MPI_Status *status)
{
#if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Wait rank = %d host = %s SA=%d] Func: MPI_SEC_Wait_Isend\n", init_rank,hostname,security_approach);fflush(stdout);
	}
#endif    
    int mpi_errno = MPI_SUCCESS;
    MPI_Status sta;
    MPI_Request req;

    int index, i;
    index = *request;

    if (nonblock_req_handler[index].source != nonblock_req_handler[index].dest)
    {        
        for (i = 0; i < nonblock_req_handler[index].total_request; i++)
        {
            mpi_errno = MPI_Wait_original(&nonblock_req_handler[index].request[i], &sta);
            pendingIsendRequestCount[nonblock_req_handler[index].dest] -= 1;
        }
        nonblock_req_handler[index].req_type = 0;
    }
    else
    {
        mpi_errno = MPI_Wait_original(request, status);
    }

    return mpi_errno;
}

// ==============================================================================

int MPI_SEC_GCM_Wait_Irecv(MPI_Request *req, MPI_Status *status)
{
#if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Wait rank = %d host = %s SA=%d] Func: MPI_SEC_GCM_Wait_Irecv\n", init_rank,hostname,security_approach);fflush(stdout);
	}
#endif       
    int mpi_errno = MPI_SUCCESS;

    int recvtype_sz, totaldata;
    int index, source, dest, tag;
    char *buf;
    MPI_Status sta;
    

    index = *req;
    source = nonblock_req_handler[index].source;
    dest = nonblock_req_handler[index].dest;
    // tag = nonblock_req_handler[index].tag;
    nonblock_req_handler[index].req_type = 0;
    buf = nonblock_req_handler[index].buffer;
    int comm = MPI_COMM_WORLD;
    int segment_counter;
    unsigned long count;

    mpi_errno = MPI_Wait_original(&nonblock_req_handler[index].request[0], &sta);

    totaldata = Array_to_Integer (Ideciphertext[index]);
       
    mpi_errno = MPI_Wait_original(&nonblock_req_handler[index].request[1], &sta);
    
    unsigned long max_out_len = totaldata + 16;
    int pos = MSG_HEADER_SIZE; 
    int enc_data = totaldata;

    if (TIMING_FLAG) start_time_dec();        
    
    if (!EVP_AEAD_CTX_open(global_ctx, buf, 
                            &count, enc_data,
                            &Ideciphertext[index][pos], 12,
                            &Ideciphertext[index][pos + 12], (unsigned long)(max_out_len),
                            NULL, 0))
    {
        printf("Err in Decryption GCM: MPI_Wait\n");fflush(stdout);
    }

    if (TIMING_FLAG) stop_time_dec();        

    return mpi_errno;
}

// ==============================================================================

int MPI_SEC_OCB_Wait_Irecv(MPI_Request *req, MPI_Status *status)
{
#if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Wait rank = %d host = %s SA=%d] Func: MPI_SEC_OCB_Wait_Irecv\n", init_rank,hostname,security_approach);fflush(stdout);
	}
#endif       
    int mpi_errno = MPI_SUCCESS;

    int recvtype_sz, totaldata;
    int index, source, dest, tag;
    char *buf;
    MPI_Status sta;
    

    index = *req;
    source = nonblock_req_handler[index].source;
    dest = nonblock_req_handler[index].dest;
    // tag = nonblock_req_handler[index].tag;
    nonblock_req_handler[index].req_type = 0;
    buf = nonblock_req_handler[index].buffer;
    int comm = MPI_COMM_WORLD;
    int segment_counter;
    unsigned long count;

    mpi_errno = MPI_Wait_original(&nonblock_req_handler[index].request[0], &sta);

    totaldata = Array_to_Integer (Ideciphertext[index]);
       
    mpi_errno = MPI_Wait_original(&nonblock_req_handler[index].request[1], &sta);
    
    int pos = MSG_HEADER_SIZE; 
    if (TIMING_FLAG) start_time_dec();        
    int res = ocb_decrypt(ocb_enc_ctx, ocb_enc_ctx2, ocb_enc_ctx3, ocb_enc_ctx4, &Ideciphertext[index][pos], totaldata, buf, 1);  
    if (TIMING_FLAG) stop_time_dec();        
            
    if (res == -1)  printf("Authentication error on Wait\n");   

    return mpi_errno;
}

// ==============================================================================


int MPI_Wait(MPI_Request * request, MPI_Status * status)
{
#if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Wait rank = %d host = %s SA=%d] Func: MPI_Wait\n", init_rank,hostname,security_approach);fflush(stdout);
	}
#endif

    if (TIMING_FLAG) start_time_wait();
    int mpi_errno = MPI_SUCCESS;
    int req_index = *request;

    if (security_approach == 401 && init_phase==0 && req_index >= 0 && req_index < ISEND_REQ)  {            
        if (nonblock_req_handler[req_index].req_type == 1)  
            mpi_errno = MPI_SEC_Wait_Isend(request, status);
        else if (nonblock_req_handler[req_index].req_type == 2) 
            mpi_errno = MPI_SEC_GCM_Wait_Irecv(request, status);
    } else if (security_approach == 402 && init_phase==0 && req_index >= 0 && req_index < ISEND_REQ)  {            
        if (nonblock_req_handler[req_index].req_type == 1)  
            mpi_errno = MPI_SEC_Wait_Isend(request, status);
        else if (nonblock_req_handler[req_index].req_type == 2) 
            mpi_errno = MPI_SEC_OCB_Wait_Irecv(request, status);
    } else      
        mpi_errno = MPI_Wait_original(request, status);
    
    if (TIMING_FLAG) stop_time_wait();
    return mpi_errno;
}