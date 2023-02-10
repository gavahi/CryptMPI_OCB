/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include "mpiimpl.h"

/* Added by Abu Naser */
unsigned char large_send_buffer[COMMON_LARGE_SEND_BUFFER_SIZE];
unsigned char large_recv_buffer[COMMON_LARGE_RECV_BUFFER_SIZE];

// double omp_t2 = 0;
struct CryptHandleProbe Crypthandle_probe[2048]; 
/* end of add */

/* -- Begin Profiling Symbol Block for routine MPI_Send */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_Send = PMPI_Send
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_Send  MPI_Send
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_Send as PMPI_Send
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag,
             MPI_Comm comm) __attribute__ ((weak, alias("PMPI_Send")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_Send
#define MPI_Send PMPI_Send

#endif

#undef FUNCNAME
#define FUNCNAME MPI_Send
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
/*@
    MPI_Send - Performs a blocking send

Input Parameters:
+ buf - initial address of send buffer (choice)
. count - number of elements in send buffer (nonnegative integer)
. datatype - datatype of each send buffer element (handle)
. dest - rank of destination (integer)
. tag - message tag (integer)
- comm - communicator (handle)

Notes:
This routine may block until the message is received by the destination
process.

.N ThreadSafe

.N Fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_TAG
.N MPI_ERR_RANK

.seealso: MPI_Isend, MPI_Bsend
@*/
int MPIR_Original_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
#if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Send rank = %d host = %s count = %d SA=%d] Func: MPIR_Original_Send\n", init_rank,hostname,count,security_approach);fflush(stdout);
	}
#endif 
    int mpi_errno = MPI_SUCCESS;
    MPIR_Comm *comm_ptr = NULL;
    MPIR_Request *request_ptr = NULL;
    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_SEND);

    MPIR_ERRTEST_INITIALIZED_ORDIE();

    MPID_THREAD_CS_ENTER(VNI_GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    MPIR_FUNC_TERSE_PT2PT_ENTER_FRONT(MPID_STATE_MPI_SEND);

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
            MPIR_ERRTEST_SEND_RANK(comm_ptr, dest, mpi_errno);
            MPIR_ERRTEST_SEND_TAG(tag, mpi_errno);

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

    mpi_errno = MPID_Send(buf, count, datatype, dest, tag, comm_ptr,
                          MPIR_CONTEXT_INTRA_PT2PT, &request_ptr);
    if (mpi_errno != MPI_SUCCESS)
        goto fn_fail;

    if (request_ptr == NULL) {
        goto fn_exit;
    }

    mpi_errno = MPID_Wait(request_ptr, MPI_STATUS_IGNORE);
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);

    mpi_errno = request_ptr->status.MPI_ERROR;
    MPIR_Request_free(request_ptr);

    if (mpi_errno != MPI_SUCCESS)
        goto fn_fail;

    /* ... end of body of routine ... */

  fn_exit:
    MPIR_FUNC_TERSE_PT2PT_EXIT(MPID_STATE_MPI_SEND);
    MPID_THREAD_CS_EXIT(VNI_GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    return mpi_errno;

  fn_fail:
    /* --BEGIN ERROR HANDLING-- */
#ifdef HAVE_ERROR_CHECKING
    {
        mpi_errno =
            MPIR_Err_create_code(mpi_errno, MPIR_ERR_RECOVERABLE, FCNAME, __LINE__, MPI_ERR_OTHER,
                                 "**mpi_send", "**mpi_send %p %d %D %i %t %C", buf, count, datatype,
                                 dest, tag, comm);
    }
#endif
    mpi_errno = MPIR_Err_return_comm(comm_ptr, FCNAME, mpi_errno);
    goto fn_exit;
    /* --END ERROR HANDLING-- */
}

int Array_to_Integer(unsigned char array[]){

    int totaldata = ((unsigned char)array[3] << 0) | ((unsigned char)array[2] << 8) | ((unsigned char)array[1] << 16) | ((unsigned char)array[0] << 24);
    return totaldata;
}


void Integer_to_Array(unsigned char array[] , int totaldata){

     unsigned int temp_data = (unsigned int) totaldata;

    /* setting the size of the data */
    array[0] = (temp_data >> 24) & 0xFF;
    array[1] = (temp_data >> 16) & 0xFF;
    array[2] = (temp_data >> 8) & 0xFF;
    array[3] = temp_data & 0xFF;
}

void start_time_enc(){
    gettimeofday(&omp_tv1, NULL);
}

void stop_time_enc(){
    gettimeofday(&omp_tv2, NULL);
    omp_t1 += (double) (omp_tv2.tv_usec - omp_tv1.tv_usec)/1000000  + (double) (omp_tv2.tv_sec - omp_tv1.tv_sec); 
}

void start_time_dec(){
    gettimeofday(&omp_tv3, NULL);
}

void stop_time_dec(){
    gettimeofday(&omp_tv4, NULL);
    omp_t2 += (double) (omp_tv4.tv_usec - omp_tv3.tv_usec)/1000000  + (double) (omp_tv4.tv_sec - omp_tv3.tv_sec);   
    if (PRINT_TIMING && (init_rank==0)) fprintf(stderr,"%lf %lf %lf %lf\n", total_comm_plus_enc_time, omp_t1,omp_t2,omp_t3); 
}

void start_time_header_comm(){
    gettimeofday(&omp_tv5, NULL);
}

void stop_time_header_comm(){
    gettimeofday(&omp_tv6, NULL);
    omp_t3 += (double) (omp_tv6.tv_usec - omp_tv5.tv_usec)/1000000  + (double) (omp_tv6.tv_sec - omp_tv5.tv_sec);
}

void start_time_all_comm(){
    gettimeofday(&comm_start_time, NULL);
}

void stop_time_all_comm(){
    gettimeofday(&comm_end_time, NULL);
    total_comm_plus_enc_time += (double) (comm_end_time.tv_usec - comm_start_time.tv_usec)/1000000  + (double) (comm_end_time.tv_sec - comm_start_time.tv_sec);
}

void start_time_wait(){
    gettimeofday(&wait_start_time, NULL);
}

void stop_time_wait(){
    gettimeofday(&wait_end_time, NULL);
    total_wait_time += (double) (wait_end_time.tv_usec - wait_start_time.tv_usec)/1000000  + (double) (wait_end_time.tv_sec - wait_start_time.tv_sec);
}

void start_time_all_prog(){
    gettimeofday(&prog_start_time, NULL);
}

void stop_time_all_prog(){
    gettimeofday(&prog_end_time, NULL);
    prog_exec_time = (double) (prog_end_time.tv_usec - prog_start_time.tv_usec)/1000000  + (double) (prog_end_time.tv_sec - prog_start_time.tv_sec);
}



int MPIR_Naive_Sec_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag,
	     MPI_Comm comm)
{
#if P2P_PRINT_FUN
	char hostname[100];
    gethostname(hostname, MAX_HOSTNAME_LEN);
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){	
    printf("[%d] Send host = %s count = %d  SA = %d  dest=%d] Func: MPIR_Naive_Sec_Send\n",
    init_rank,hostname,count,security_approach,dest);fflush(stdout);}
#endif  

    int mpi_errno = MPI_SUCCESS;
    int  sendtype_sz, totaldata; //, m, start, pos, i,s; 
    
    MPI_Request request1, request2;
    MPI_Status status;
    
    MPIR_Comm *comm_ptr = NULL;
    MPIR_Comm_get_ptr(comm, comm_ptr);         
    
    MPI_Type_size(datatype, &sendtype_sz);  
    totaldata = count * sendtype_sz;

    if (TIMING_FLAG) start_time_header_comm();  
    Integer_to_Array(&large_send_buffer, totaldata);

    mpi_errno = MPI_Isend_original(large_send_buffer, MSG_HEADER_SIZE , MPI_UNSIGNED_CHAR, dest, tag, comm, &request1);

    if (TIMING_FLAG) stop_time_header_comm();

    if (TIMING_FLAG) start_time_enc();


    RAND_bytes(&large_send_buffer[MSG_HEADER_SIZE], 12);

    unsigned long ciphertext_len=0 , max_out_len = totaldata + 16;
    int pos = MSG_HEADER_SIZE; 

    if (!EVP_AEAD_CTX_seal(global_ctx, &large_send_buffer[pos + 12],
                            &ciphertext_len, max_out_len,
                            &large_send_buffer[pos], 12,
                            buf , (unsigned long)(totaldata),
                            NULL, 0))
    {
        printf("Err in encryption GCM: Send\n");
        fflush(stdout);
    }

    if (TIMING_FLAG) stop_time_enc();

    mpi_errno = MPI_Isend_original(large_send_buffer+MSG_HEADER_SIZE, totaldata +  (12 + 16) , MPI_UNSIGNED_CHAR, dest, tag, comm, &request2);

    mpi_errno = MPI_Wait_original(&request1, &status);
    mpi_errno = MPI_Wait_original(&request2, &status);
                                      
    return mpi_errno;
}


int MPIR_OCB_Naive_Sec_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag,
	     MPI_Comm comm)
{
#if P2P_PRINT_FUN
	char hostname[100];
    gethostname(hostname, MAX_HOSTNAME_LEN);
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){	
    printf("[%d] Send host = %s count = %d  SA = %d  dest=%d] Func: MPIR_OCB_Naive_Sec_Send\n",
    init_rank,hostname,count,security_approach,dest);fflush(stdout);}
#endif  

    int mpi_errno = MPI_SUCCESS;
    int  sendtype_sz, totaldata; 
    
    MPI_Request request1, request2;
    MPI_Status status;
    
    MPIR_Comm *comm_ptr = NULL;
    MPIR_Comm_get_ptr(comm, comm_ptr);         
    
    MPI_Type_size(datatype, &sendtype_sz);  
    totaldata = count * sendtype_sz;

    if (TIMING_FLAG) start_time_header_comm();  
    Integer_to_Array(&large_send_buffer, totaldata);

    mpi_errno = MPI_Isend_original(large_send_buffer, MSG_HEADER_SIZE , MPI_UNSIGNED_CHAR, dest, tag, comm, &request1);
    if (TIMING_FLAG) stop_time_header_comm();

    int pos = MSG_HEADER_SIZE; 
    if (TIMING_FLAG) start_time_enc();
    ocb_encrypt(ocb_enc_ctx, ocb_enc_ctx2, ocb_enc_ctx3, ocb_enc_ctx4, buf, totaldata, &large_send_buffer[pos], 1);
    if (TIMING_FLAG) stop_time_enc();

    mpi_errno = MPI_Isend_original(large_send_buffer+MSG_HEADER_SIZE, totaldata + OCB_TAG_NONCE , MPI_UNSIGNED_CHAR, dest, tag, comm, &request2);

    mpi_errno = MPI_Wait_original(&request1, &status);
    mpi_errno = MPI_Wait_original(&request2, &status);
                                      
    return mpi_errno;
}


int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
#if P2P_PRINT_FUN
   if (PRINT_FUN_NAME || DEBUG_INIT_FILE){
		char hostname[100];
		gethostname(hostname, MAX_HOSTNAME_LEN);
		printf("[Send rank = %d host = %s count = %d SA=%d] Func: MPI_Send\n", init_rank,hostname,count,security_approach);fflush(stdout);
	}
#endif
    if (TIMING_FLAG) start_time_all_comm();   
    int mpi_errno = MPI_SUCCESS;

    if (security_approach == 401 && init_phase==0)  {            
                mpi_errno = MPIR_Naive_Sec_Send(buf, count, datatype, dest, tag, comm);
    } else if (security_approach == 402 && init_phase==0)  {            
                mpi_errno = MPIR_OCB_Naive_Sec_Send(buf, count, datatype, dest, tag, comm);
    } else      mpi_errno = MPIR_Original_Send(buf, count, datatype, dest, tag, comm);

    if (TIMING_FLAG) stop_time_all_comm();       

    return mpi_errno;
   
}
