/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include "mpiimpl.h"

/* Added by Abu Naser(an16e@my.fsu.edu) */
#if 1 //ENABLE_SECURE_MPI  
struct isend_req nonblock_req_handler[ISEND_REQ+5];
int max_send_data_size = 0;
unsigned char Iciphertext[NON_BLOCK_SEND][NON_BLOCK_SEND2];;
int nonBlockCounter = 0;
long pendingIsendRequestCount[MAX_RANKS_LIMIT];
#endif
/* End of add by Abu Naser */

/* -- Begin Profiling Symbol Block for routine MPI_Isend */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_Isend = PMPI_Isend
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_Isend  MPI_Isend
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_Isend as PMPI_Isend
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag,
              MPI_Comm comm, MPI_Request * request) __attribute__ ((weak, alias("PMPI_Isend")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_Isend
#define MPI_Isend PMPI_Isend

#endif

#undef FUNCNAME
#define FUNCNAME MPI_Isend
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
/*@
    MPI_Isend - Begins a nonblocking send

Input Parameters:
+ buf - initial address of send buffer (choice)
. count - number of elements in send buffer (integer)
. datatype - datatype of each send buffer element (handle)
. dest - rank of destination (integer)
. tag - message tag (integer)
- comm - communicator (handle)

Output Parameters:
. request - communication request (handle)

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
int MPI_Isend_original(const void *buf, int count, MPI_Datatype datatype, int dest, int tag,
              MPI_Comm comm, MPI_Request * request)
{
#if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Isend rank = %d host = %s count = %d SA=%d] Func: MPI_Isend_original\n", init_rank,hostname,count,security_approach);fflush(stdout);
	}
#endif 
    int mpi_errno = MPI_SUCCESS;
    MPIR_Comm *comm_ptr = NULL;
    MPIR_Request *request_ptr = NULL;
    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_ISEND);

    MPIR_ERRTEST_INITIALIZED_ORDIE();

    MPID_THREAD_CS_ENTER(VNI_GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    MPIR_FUNC_TERSE_PT2PT_ENTER_FRONT(MPID_STATE_MPI_ISEND);

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

    /* Validate parameters if err checking is enabled */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPIR_Comm_valid_ptr(comm_ptr, mpi_errno, FALSE);
            if (mpi_errno)
                goto fn_fail;

            MPIR_ERRTEST_COUNT(count, mpi_errno);
            MPIR_ERRTEST_SEND_RANK(comm_ptr, dest, mpi_errno);
            MPIR_ERRTEST_SEND_TAG(tag, mpi_errno);
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

    mpi_errno = MPID_Isend(buf, count, datatype, dest, tag, comm_ptr,
                           MPIR_CONTEXT_INTRA_PT2PT, &request_ptr);
    if (mpi_errno != MPI_SUCCESS)
        goto fn_fail;

    MPII_SENDQ_REMEMBER(request_ptr, dest, tag, comm_ptr->context_id);

    /* return the handle of the request to the user */
    /* MPIU_OBJ_HANDLE_PUBLISH is unnecessary for isend, lower-level access is
     * responsible for its own consistency, while upper-level field access is
     * controlled by the completion counter */
    *request = request_ptr->handle;

    /* ... end of body of routine ... */

  fn_exit:
    MPIR_FUNC_TERSE_PT2PT_EXIT(MPID_STATE_MPI_ISEND);
    MPID_THREAD_CS_EXIT(VNI_GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    return mpi_errno;

  fn_fail:
    /* --BEGIN err HANDLING-- */
#ifdef HAVE_ERROR_CHECKING
    {
        mpi_errno =
            MPIR_Err_create_code(mpi_errno, MPIR_ERR_RECOVERABLE, FCNAME, __LINE__, MPI_ERR_OTHER,
                                 "**mpi_isend", "**mpi_isend %p %d %D %i %t %C %p", buf, count,
                                 datatype, dest, tag, comm, request);
    }
#endif
    mpi_errno = MPIR_Err_return_comm(comm_ptr, FCNAME, mpi_errno);
    goto fn_exit;
    /* --END err HANDLING-- */
}


int MPIR_Sec_GCM_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request * request)
{
#if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Send rank = %d host = %s count = %d SA=%d] Func: MPIR_Sec_GCM_Isend\n", init_rank,hostname,count,security_approach);fflush(stdout);
	}
#endif    
    int mpi_errno = MPI_SUCCESS;
    unsigned long max_out_len, ciphertext_len=0;
    int  sendtype_sz, totaldata, m, start, pos, i,s; 
    MPIR_Comm *comm_ptr = NULL;
    MPIR_Comm_get_ptr(comm, comm_ptr);         
    MPI_Type_size(datatype, &sendtype_sz);      
    totaldata = count * sendtype_sz;
    /*unsigned int tempdata = (unsigned int)totaldata;
    
    Iciphertext[nonBlockCounter][0] = (tempdata >> 24) & 0xFF;
    Iciphertext[nonBlockCounter][1] = (tempdata >> 16) & 0xFF;
    Iciphertext[nonBlockCounter][2] = (tempdata >> 8) & 0xFF;
    Iciphertext[nonBlockCounter][3] = tempdata & 0xFF;*/

    if (TIMING_FLAG) start_time_header_comm();  

    Integer_to_Array(&Iciphertext[nonBlockCounter], totaldata);

    int request_counter = 0;
#if 0
    unsigned int adap_chop = (unsigned int)totaldata;
    Iciphertext[nonBlockCounter][21] = (adap_chop >> 24) & 0xFF;
    Iciphertext[nonBlockCounter][22] = (adap_chop >> 16) & 0xFF;
    Iciphertext[nonBlockCounter][23] = (adap_chop >> 8) & 0xFF;
    Iciphertext[nonBlockCounter][24] = adap_chop & 0xFF;
#endif    
    mpi_errno = MPI_Isend_original(&Iciphertext[nonBlockCounter][0], MSG_HEADER_SIZE, MPI_CHAR, dest, tag, comm,
    &nonblock_req_handler[nonBlockCounter].request[request_counter++]);

    if (TIMING_FLAG) stop_time_header_comm();  

    max_out_len = totaldata+16; 
    pos = MSG_HEADER_SIZE;

    if (TIMING_FLAG) start_time_enc();

    RAND_bytes(&Iciphertext[nonBlockCounter][pos], 12);        
    int enc_data = totaldata;
    
    if(!EVP_AEAD_CTX_seal(global_ctx, &Iciphertext[nonBlockCounter][pos+12],
                            &ciphertext_len, max_out_len,
                            &Iciphertext[nonBlockCounter][pos], 12,
                            buf,  (unsigned long)(enc_data),
                            NULL, 0)){
            printf("[T = %d] openmp-isend err in encryption: segment_counter=%d\n",omp_get_thread_num(),0);
            //fflush(stdout);
        }

    if (TIMING_FLAG) stop_time_enc();


    mpi_errno = MPI_Isend_original(&Iciphertext[nonBlockCounter][MSG_HEADER_SIZE], totaldata+((12+16)), MPI_CHAR, dest, tag, comm,
    &nonblock_req_handler[nonBlockCounter].request[request_counter++]);

    nonblock_req_handler[nonBlockCounter].source = comm_ptr->rank;
    nonblock_req_handler[nonBlockCounter].dest = dest;
    nonblock_req_handler[nonBlockCounter].tag = tag;
    nonblock_req_handler[nonBlockCounter].totaldata = totaldata;
    nonblock_req_handler[nonBlockCounter].req_type = 1;
    nonblock_req_handler[nonBlockCounter].buffer = buf; 
    nonblock_req_handler[nonBlockCounter].total_request = request_counter; 
    *request = nonBlockCounter;
  //  printf("isend nonBlockCounter=%d\n",nonBlockCounter);fflush(stdout);
    nonBlockCounter++;
    if(nonBlockCounter == ISEND_REQ)
        nonBlockCounter = 0;

    return mpi_errno;
}




int MPIR_Sec_OCB_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request * request)
{
#if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Send rank = %d host = %s count = %d SA=%d] Func: MPIR_Sec_OCB_Isend\n", init_rank,hostname,count,security_approach);fflush(stdout);
	}
#endif    
    int mpi_errno = MPI_SUCCESS;
    unsigned long max_out_len, ciphertext_len=0;
    int  sendtype_sz, totaldata, m, start, pos, i,s; 
    MPIR_Comm *comm_ptr = NULL;
    MPIR_Comm_get_ptr(comm, comm_ptr);         
    MPI_Type_size(datatype, &sendtype_sz);      
    totaldata = count * sendtype_sz;
/*
    unsigned int tempdata = (unsigned int)totaldata;
    
    Iciphertext[nonBlockCounter][0] = (tempdata >> 24) & 0xFF;
    Iciphertext[nonBlockCounter][1] = (tempdata >> 16) & 0xFF;
    Iciphertext[nonBlockCounter][2] = (tempdata >> 8) & 0xFF;
    Iciphertext[nonBlockCounter][3] = tempdata & 0xFF;
*/    
    if (TIMING_FLAG) start_time_header_comm();  
    Integer_to_Array(&Iciphertext[nonBlockCounter], totaldata);
    int request_counter = 0;

    mpi_errno = MPI_Isend_original(&Iciphertext[nonBlockCounter][0], MSG_HEADER_SIZE, MPI_CHAR, dest, tag, comm,
    &nonblock_req_handler[nonBlockCounter].request[request_counter++]);

    if (TIMING_FLAG) stop_time_header_comm();  

    pos = MSG_HEADER_SIZE;

    if (TIMING_FLAG) start_time_enc();

    ocb_encrypt(ocb_enc_ctx, ocb_enc_ctx2, ocb_enc_ctx3, ocb_enc_ctx4, buf, totaldata, &Iciphertext[nonBlockCounter][pos], 1);

    if (TIMING_FLAG) stop_time_enc;

    mpi_errno = MPI_Isend_original(&Iciphertext[nonBlockCounter][MSG_HEADER_SIZE], totaldata+OCB_TAG_NONCE, MPI_CHAR, dest, tag, comm,
    &nonblock_req_handler[nonBlockCounter].request[request_counter++]);

    nonblock_req_handler[nonBlockCounter].source = comm_ptr->rank;
    nonblock_req_handler[nonBlockCounter].dest = dest;
    nonblock_req_handler[nonBlockCounter].tag = tag;
    nonblock_req_handler[nonBlockCounter].totaldata = totaldata;
    nonblock_req_handler[nonBlockCounter].req_type = 1;
    nonblock_req_handler[nonBlockCounter].buffer = buf; 
    nonblock_req_handler[nonBlockCounter].total_request = request_counter; 
    *request = nonBlockCounter;
  
    nonBlockCounter++;
    if(nonBlockCounter == ISEND_REQ)
        nonBlockCounter = 0;

    return mpi_errno;
}



int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag,
              MPI_Comm comm, MPI_Request * request)
{
#if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Send rank = %d host = %s count = %d SA=%d] Func: MPI_Isend\n", init_rank,hostname,count,security_approach);fflush(stdout);
	}
#endif 
    if (TIMING_FLAG) start_time_all_comm();       
    int mpi_errno = MPI_SUCCESS;

    if (security_approach == 401 && init_phase==0)  {            
                mpi_errno = MPIR_Sec_GCM_Isend(buf, count, datatype, dest, tag, comm,request);
    } else if (security_approach == 402 && init_phase==0)  {            
                mpi_errno = MPIR_Sec_OCB_Isend(buf, count, datatype, dest, tag, comm,request);
    } else      mpi_errno = MPI_Isend_original(buf, count, datatype, dest, tag, comm,request);

    if (TIMING_FLAG) stop_time_all_comm();
    return mpi_errno;
   
}