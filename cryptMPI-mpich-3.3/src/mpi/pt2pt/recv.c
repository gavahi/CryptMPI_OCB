/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include "mpiimpl.h"

/* -- Begin Profiling Symbol Block for routine MPI_Recv */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_Recv = PMPI_Recv
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_Recv  MPI_Recv
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_Recv as PMPI_Recv
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag,
             MPI_Comm comm, MPI_Status * status) __attribute__ ((weak, alias("PMPI_Recv")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_Recv
#define MPI_Recv PMPI_Recv

#endif

#undef FUNCNAME
#define FUNCNAME MPI_Recv
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
/*@
    MPI_Recv - Blocking receive for a message

Output Parameters:
+ buf - initial address of receive buffer (choice)
- status - status object (Status)

Input Parameters:
+ count - maximum number of elements in receive buffer (integer)
. datatype - datatype of each receive buffer element (handle)
. source - rank of source (integer)
. tag - message tag (integer)
- comm - communicator (handle)

Notes:
The 'count' argument indicates the maximum length of a message; the actual
length of the message can be determined with 'MPI_Get_count'.

.N ThreadSafe

.N Fortran

.N FortranStatus

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_TYPE
.N MPI_ERR_COUNT
.N MPI_ERR_TAG
.N MPI_ERR_RANK

@*/
int MPI_Recv_original(void *buf, int count, MPI_Datatype datatype, int source, int tag,
             MPI_Comm comm, MPI_Status * status)
{
#if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Recv rank = %d host = %s count = %d SA=%d] Func: MPI_Recv_original\n", init_rank,hostname,count,security_approach);fflush(stdout);
	}
#endif 
    int mpi_errno = MPI_SUCCESS;
    MPIR_Comm *comm_ptr = NULL;
    MPIR_Request *request_ptr = NULL;
    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_RECV);

    MPIR_ERRTEST_INITIALIZED_ORDIE();

    MPID_THREAD_CS_ENTER(VNI_GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    MPIR_FUNC_TERSE_PT2PT_ENTER_BACK(MPID_STATE_MPI_RECV);

    /* Validate handle parameters needing to be converted */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPIR_ERRTEST_COMM(comm, mpi_errno);
            /* NOTE: MPI_STATUS_IGNORE != NULL */
            MPIR_ERRTEST_ARGNULL(status, "status", mpi_errno);
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

    /* MT: Note that MPID_Recv may release the SINGLE_CS if it
     * decides to block internally.  MPID_Recv in that case will
     * re-aquire the SINGLE_CS before returnning */
    mpi_errno = MPID_Recv(buf, count, datatype, source, tag, comm_ptr,
                          MPIR_CONTEXT_INTRA_PT2PT, status, &request_ptr);
    if (mpi_errno != MPI_SUCCESS)
        goto fn_fail;

    if (request_ptr == NULL) {
        goto fn_exit;
    }

    mpi_errno = MPID_Wait(request_ptr, MPI_STATUS_IGNORE);
    if (mpi_errno != MPI_SUCCESS)
        goto fn_fail;

    mpi_errno = request_ptr->status.MPI_ERROR;
    MPIR_Request_extract_status(request_ptr, status);
    MPIR_Request_free(request_ptr);

    if (mpi_errno != MPI_SUCCESS)
        goto fn_fail;

    /* ... end of body of routine ... */

  fn_exit:
    MPIR_FUNC_TERSE_PT2PT_EXIT_BACK(MPID_STATE_MPI_RECV);
    MPID_THREAD_CS_EXIT(VNI_GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    return mpi_errno;

  fn_fail:
    /* --BEGIN ERROR HANDLING-- */
#ifdef HAVE_ERROR_CHECKING
    {
        mpi_errno =
            MPIR_Err_create_code(mpi_errno, MPIR_ERR_RECOVERABLE, FCNAME, __LINE__, MPI_ERR_OTHER,
                                 "**mpi_recv", "**mpi_recv %p %d %D %i %t %C %p", buf, count,
                                 datatype, source, tag, comm, status);
    }
#endif
    mpi_errno = MPIR_Err_return_comm(comm_ptr, FCNAME, mpi_errno);
    goto fn_exit;
    /* --END ERROR HANDLING-- */
}



int MPIR_Naive_Sec_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag,
                                     MPI_Comm comm, MPI_Status *status)
{
#if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Recv rank = %d host = %s count = %d SA=%d] Func: MPIR_Naive_Sec_Recv\n", init_rank,hostname,count,security_approach);fflush(stdout);
	}
#endif       
    int mpi_errno = MPI_SUCCESS;
    int recvtype_sz; 
    int totaldata;
    unsigned int temp_data;
    MPI_Type_size(datatype, &recvtype_sz);

    if (TIMING_FLAG) start_time_header_comm();         

    mpi_errno = MPI_Recv_original(large_recv_buffer, MSG_HEADER_SIZE, MPI_CHAR, source, tag, comm, status);

    /*temp_data = ((unsigned char)large_recv_buffer[3] << 0) | ((unsigned char)large_recv_buffer[2] << 8) | ((unsigned char)large_recv_buffer[1] << 16) | ((unsigned char)large_recv_buffer[0] << 24);
    totaldata = (int)temp_data;*/

    totaldata = Array_to_Integer (large_recv_buffer);

    if (TIMING_FLAG) stop_time_header_comm();


    mpi_errno = MPI_Recv_original(large_recv_buffer, totaldata + (12 + 16), MPI_CHAR, source, tag, comm, status);
    
    if (TIMING_FLAG) start_time_dec();        
    
    unsigned long max_out_len = totaldata + 16;
    
    if (!EVP_AEAD_CTX_open(global_ctx, buf ,
                            &count, totaldata,
                            &large_recv_buffer, 12,
                            large_recv_buffer + 12, (unsigned long)(max_out_len),
                            NULL, 0))
    {
        printf("Err in Decryption GCM: Recv\n");
        fflush(stdout);
    }
    
    if (TIMING_FLAG) stop_time_dec();

    return mpi_errno;
}



int MPIR_OCB_Naive_Sec_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag,
                                     MPI_Comm comm, MPI_Status *status)
{
#if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Recv rank = %d host = %s count = %d SA=%d] Func: MPIR_OCB_Naive_Sec_Recv\n", init_rank,hostname,count,security_approach);fflush(stdout);
	}
#endif       
    int mpi_errno = MPI_SUCCESS;
    int recvtype_sz; 
    int totaldata;
    unsigned int temp_data;
    MPI_Type_size(datatype, &recvtype_sz);

    if (TIMING_FLAG) start_time_header_comm();  
    mpi_errno = MPI_Recv_original(large_recv_buffer, MSG_HEADER_SIZE, MPI_CHAR, source, tag, comm, status);

    totaldata = Array_to_Integer (large_recv_buffer);
    if (TIMING_FLAG) stop_time_header_comm();

    mpi_errno = MPI_Recv_original(large_recv_buffer, totaldata + OCB_TAG_NONCE, MPI_CHAR, source, tag, comm, status);
    
    if (TIMING_FLAG) start_time_dec();        
    int res = ocb_decrypt(ocb_enc_ctx, ocb_enc_ctx2, ocb_enc_ctx3, ocb_enc_ctx4,  &large_recv_buffer, totaldata, buf , 1);  
    if (TIMING_FLAG) stop_time_dec();
    
    if (res == -1)  printf("Authentication error on Recv OCB\n");
    
    return mpi_errno;
}


int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status * status)
{
#if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Recv rank = %d host = %s count = %d SA=%d] Func: MPI_Recv\n", init_rank,hostname,count,security_approach);fflush(stdout);
	}
#endif     
    if (TIMING_FLAG) start_time_all_comm();   
    int mpi_errno = MPI_SUCCESS;

    if (security_approach == 401 && init_phase==0)  {            
                mpi_errno = MPIR_Naive_Sec_Recv(buf, count, datatype, source, tag, comm,status);
    } else if (security_approach == 402 && init_phase==0)  {            
                mpi_errno = MPIR_OCB_Naive_Sec_Recv(buf, count, datatype, source, tag, comm,status);
    } else      mpi_errno = MPI_Recv_original(buf, count, datatype, source, tag, comm,status);

    if (TIMING_FLAG) stop_time_all_comm();
    return mpi_errno;
   
}