/* -*- Mode**: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include "mpiimpl.h"
#include "mpi_init.h"

#include <strings.h>
// ocb
#include <ae.h>
#if __GNUC__
#define ALIGN(n)      __attribute__ ((aligned(n))) 
#elif _MSC_VER
#define ALIGN(n)      __declspec(align(n))
#else
#define ALIGN(n)
#endif
/************ end of ocb *************/


// if (DEBUG_INIT_FILE) fprintf(stderr,"[%d] Initeee 02\n",init_rank);
// Add By Mohsen
int node_cnt=1;
int init_phase=1;
int super_node=0;
int PRINT_FUN_NAME=0;
int PRINT_TIMING=0;
int ALLGATHER_PRINT_FUN_NAME=0;
int ENABLE_SECURE_DEBUG=0;
int DEBUG_INIT_FILE=0;
int SHOW_FINAL_RESULT=0;
int PRINT_Ring_FUN_NAME=0;
int PRINT_RSA_FUN_NAME=0;
int UNSEC_ALLREDUCE_MULTI_LEADER=0;
int PRINT_SUPER=0;
int is_blocked_backup=0;

int CONCUR_INTER_METHOD=3;
// int ALLREDUCE_CONCUR_ENABLE=0;
int CONCUR_RS_METHOD=1;
int ALLGATHER_METHOD=1;
int leader_cnt=1;
int SHMEM_BCAST=0;
int Allgather_Reduce=0;
int enc_choping_sz=0;
int *comm_rank_list;
int *comm_rank_list_back;
int init_rank=0;

int inter_scatter_tuning=0;
int inter_alltoall_tuning=-1;
int mv2_user_allgather_inter=0;
/*** Mehran Added ***/
int bcast_tuning = 0;
/********************/
int inter_gather_tuning=0;
int inter_allreduce_tuning=0;
// int is_uniform=1;
int *node_sizes;


/********* OCB *********/

int OCB_MAX_ITER=1024;
ALIGN(16) char ocb_pt[8*1024] = {0};
ALIGN(16) char ocb_tag[16];
ALIGN(16) unsigned char ocb_key[] = "abcdefghijklmnop";
ALIGN(16) unsigned char gcm_nonce[16];

// unsigned char *ocb_nonce;
int ocb_len;
int ocb_iter_list[2048]; /* Populate w/ test lengths, -1 terminated */

ae_ctx* ocb_enc_ctx = NULL;
ae_ctx* ocb_enc_ctx2 = NULL;
ae_ctx* ocb_enc_ctx3 = NULL;
ae_ctx* ocb_enc_ctx4 = NULL;
ae_ctx* ocb_dec_ctx = NULL;
ae_ctx* ocb_dec_ctx2 = NULL;
ae_ctx* ocb_dec_ctx3 = NULL;
ae_ctx* ocb_dec_ctx4 = NULL;
	
ae_ctx* ae_allocate  (void *misc);  /* Allocate ae_ctx, set optional ptr   */
void    ae_free      (ae_ctx *ctx); /* Deallocate ae_ctx struct            */
int     ae_clear     (ae_ctx *ctx); /* Undo initialization                 */
int     ae_ctx_sizeof(void);        /* Return sizeof(ae_ctx)               */
/* ae_allocate() allocates an ae_ctx structure, but does not initialize it.
 * ae_free() deallocates an ae_ctx structure, but does not zero it.
 * ae_clear() zeroes sensitive values associated with an ae_ctx structure
 * and deallocates any auxiliary structures allocated during ae_init().
 * ae_ctx_sizeof() returns sizeof(ae_ctx), to aid in any static allocations.
 */
/********** Timing Variables **************/
struct timeval  omp_tv1, omp_tv2, omp_tv3, omp_tv4, omp_tv5, omp_tv6;
double omp_t1 = 0 , omp_t2 = 0 , omp_t3 = 0;

#if ENC_DEC_TIME_DEBUG || ALL_COMM_PLUS_ENC_TIME
struct timeval prog_start_time, prog_end_time;
double  prog_exec_time;
#endif

#if ALL_COMM_PLUS_ENC_TIME
struct timeval comm_start_time, comm_end_time, wait_start_time, wait_end_time;
double  total_comm_plus_enc_time = 0, total_wait_time = 0;
double total_comm_plus_enc_time_long_msg = 0;
double total_comm_plus_enc_time_small_msg = 0;

double inter_less_than_128K =0;
double inter_128K_256K = 0;
double inter_256K_512K = 0;
double inter_512K_1M = 0;
double inter_more_than_1M = 0;

 double intra_less_than_128K = 0;
 double intra_128K_256K = 0;
 double intra_256K_512K = 0;
 double intra_512K_1M = 0;
 double intra_more_than_1M = 0;
#endif
int long_msg_flag = 0;

#if COND_LOCK
int cond_variable[MY_MAX_NO_THREADS+10];
pthread_mutex_t myp_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t myp_cond = PTHREAD_COND_INITIALIZER;
#endif

/*******************************/

// End of Add 

/* Added by Abu Naser */
#include <openssl/rsa.h>
#include <openssl/pem.h>

int choping_sz = PIPELINE_SIZE;

void MPI_SEC_Initial_Key_Aggrement();
void init_crypto();
unsigned char symmetric_key[300];
int symmetric_key_size = 16;

EVP_AEAD_CTX *ctx = NULL;
// EVP_AEAD_CTX *local_ctx = NULL;
EVP_CIPHER_CTX *ctx_enc = NULL;
EVP_AEAD_CTX * global_openmp_ctx = NULL;
EVP_AEAD_CTX * global_small_msg_ctx = NULL;
EVP_AEAD_CTX * global_coll_msg_ctx = NULL;
EVP_AEAD_CTX * global_ctx = NULL;
EVP_CIPHER_CTX * global_counter_ctx = NULL;
EVP_CIPHER_CTX * local_counter_ctx[MAX_OMP_THREADS_LIMIT]; 
EVP_CIPHER_CTX * base_counter_ctx = NULL;

int openmp_active_thread_no = 1;
int cryptmpi_process_id = 0;
char cryptmpi_process_name[MPI_MAX_PROCESSOR_NAME];
char all_p_names[2048*MPI_MAX_PROCESSOR_NAME];
int sameNode[2048];
int total_process_number = 0;
int no_of_max_omp_threads = 0;
int noCommThreads = 2;
int cyptmpi_series_thread=0;

int cryptmpi_local_process =0;
int cryptmpi_local_rank;
int cryptmpi_own_rank;
int cryptmpi_init_done = 0;
int security_approach=0;

unsigned char Send_common_IV[32], Recv_common_IV[MAX_PROCESS_SIZE*32];
unsigned char enc_common_buffer[MAX_COMMON_COUNTER_SZ], dec_common_buffer[MAX_COMMON_COUNTER_SZ];
unsigned int enc_common_start, enc_common_end;
unsigned long enc_common_counter = 0; 
unsigned long counter_needto_send = 0; 
unsigned long counter_needto_send_large_msg = 0;
unsigned long enc_common_counter_long_msg = 0;
unsigned long base_global_counter;
unsigned char  zeros[MAX_COMMON_COUNTER_SZ];
int common_compute_size =0;


/* End of add */

/*
=== BEGIN_MPI_T_CVAR_INFO_BLOCK ===

categories:
    - name        : THREADS
      description : multi-threading cvars

cvars:
    - name        : MPIR_CVAR_ASYNC_PROGRESS
      category    : THREADS
      type        : boolean
      default     : false
      class       : device
      verbosity   : MPI_T_VERBOSITY_USER_BASIC
      scope       : MPI_T_SCOPE_ALL_EQ
      description : >-
        If set to true, MPICH will initiate an additional thread to
        make asynchronous progress on all communication operations
        including point-to-point, collective, one-sided operations and
        I/O.  Setting this variable will automatically increase the
        thread-safety level to MPI_THREAD_MULTIPLE.  While this
        improves the progress semantics, it might cause a small amount
        of performance overhead for regular MPI operations.  The user
        is encouraged to leave one or more hardware threads vacant in
        order to prevent contention between the application threads
        and the progress thread(s).  The impact of oversubscription is
        highly system dependent but may be substantial in some cases,
        hence this recommendation.

    - name        : MPIR_CVAR_DEFAULT_THREAD_LEVEL
      category    : THREADS
      type        : string
      default     : "MPI_THREAD_SINGLE"
      class       : device
      verbosity   : MPI_T_VERBOSITY_USER_BASIC
      scope       : MPI_T_SCOPE_ALL_EQ
      description : >-
        Sets the default thread level to use when using MPI_INIT. This variable
        is case-insensitive.

=== END_MPI_T_CVAR_INFO_BLOCK ===
*/

/* -- Begin Profiling Symbol Block for routine MPI_Init */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_Init = PMPI_Init
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_Init  MPI_Init
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_Init as PMPI_Init
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_Init(int *argc, char ***argv) __attribute__ ((weak, alias("PMPI_Init")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_Init
#define MPI_Init PMPI_Init

/* Fortran logical values. extern'd in mpiimpl.h */
/* MPI_Fint MPII_F_TRUE, MPII_F_FALSE; */

/* Any internal routines can go here.  Make them static if possible */

/* must go inside this #ifdef block to prevent duplicate storage on darwin */
int MPIR_async_thread_initialized = 0;
#endif

#undef FUNCNAME
#define FUNCNAME MPI_Init
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
/*@
   MPI_Init - Initialize the MPI execution environment

Input Parameters:
+  argc - Pointer to the number of arguments
-  argv - Pointer to the argument vector

Thread and Signal Safety:
This routine must be called by one thread only.  That thread is called
the `main thread` and must be the thread that calls 'MPI_Finalize'.

Notes:
   The MPI standard does not say what a program can do before an 'MPI_INIT' or
   after an 'MPI_FINALIZE'.  In the MPICH implementation, you should do
   as little as possible.  In particular, avoid anything that changes the
   external state of the program, such as opening files, reading standard
   input or writing to standard output.

Notes for C:
    As of MPI-2, 'MPI_Init' will accept NULL as input parameters. Doing so
    will impact the values stored in 'MPI_INFO_ENV'.

Notes for Fortran:
The Fortran binding for 'MPI_Init' has only the error return
.vb
    subroutine MPI_INIT(ierr)
    integer ierr
.ve

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_INIT

.seealso: MPI_Init_thread, MPI_Finalize
@*/
int MPI_Init(int *argc, char ***argv)
{
    if (TIMING_FLAG) start_time_all_prog();
    int mpi_errno = MPI_SUCCESS;
    int rc ATTRIBUTE((unused));
    int threadLevel, provided;
    MPIR_FUNC_TERSE_INIT_STATE_DECL(MPID_STATE_MPI_INIT);
	MPIR_CHKPMEM_DECL(3);

    /************ Mohsen *************/  
        
    char *m_tc = getenv("MV2_PRINT_FUN_NAME");
	if (m_tc && (strncmp(m_tc, "1",1) == 0))	PRINT_FUN_NAME = 1;
		
	m_tc = getenv("MV2_DEBUG_INIT_FILE");
	if (m_tc && (strncmp(m_tc, "1",1) == 0))	DEBUG_INIT_FILE = 1;

    m_tc = getenv("MV2_PRINT_TIMING");
	if (m_tc && (strncmp(m_tc, "1",1) == 0))	PRINT_TIMING = 1;    

    /* *********** End *********** */

    rc = MPID_Wtime_init();
#ifdef MPL_USE_DBG_LOGGING
    MPL_dbg_pre_init(argc, argv, rc);
#endif

    MPIR_FUNC_TERSE_INIT_ENTER(MPID_STATE_MPI_INIT);
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            if (OPA_load_int(&MPIR_Process.mpich_state) != MPICH_MPI_STATE__PRE_INIT) {
                mpi_errno =
                    MPIR_Err_create_code(MPI_SUCCESS, MPIR_ERR_RECOVERABLE, FCNAME, __LINE__,
                                         MPI_ERR_OTHER, "**inittwice", NULL);
            }
            if (mpi_errno)
                goto fn_fail;
        }
        MPID_END_ERROR_CHECKS;
    }
#endif /* HAVE_ERROR_CHECKING */

    /* ... body of routine ... */

    /* Temporarily disable thread-safety.  This is needed because the
     * mutexes are not initialized yet, and we don't want to
     * accidentally use them before they are initialized.  We will
     * reset this value once it is properly initialized. */
#if defined MPICH_IS_THREADED
    MPIR_ThreadInfo.isThreaded = 0;
#endif /* MPICH_IS_THREADED */

    MPIR_T_env_init();

    if (!strcasecmp(MPIR_CVAR_DEFAULT_THREAD_LEVEL, "MPI_THREAD_MULTIPLE"))
        threadLevel = MPI_THREAD_MULTIPLE;
    else if (!strcasecmp(MPIR_CVAR_DEFAULT_THREAD_LEVEL, "MPI_THREAD_SERIALIZED"))
        threadLevel = MPI_THREAD_SERIALIZED;
    else if (!strcasecmp(MPIR_CVAR_DEFAULT_THREAD_LEVEL, "MPI_THREAD_FUNNELED"))
        threadLevel = MPI_THREAD_FUNNELED;
    else if (!strcasecmp(MPIR_CVAR_DEFAULT_THREAD_LEVEL, "MPI_THREAD_SINGLE"))
        threadLevel = MPI_THREAD_SINGLE;
    else {
        MPL_error_printf("Unrecognized thread level %s\n", MPIR_CVAR_DEFAULT_THREAD_LEVEL);
        exit(1);
    }

    /* If the user requested for asynchronous progress, request for
     * THREAD_MULTIPLE. */
    if (MPIR_CVAR_ASYNC_PROGRESS)
        threadLevel = MPI_THREAD_MULTIPLE;

    mpi_errno = MPIR_Init_thread(argc, argv, threadLevel, &provided);
    if (mpi_errno != MPI_SUCCESS)
        goto fn_fail;

    if (MPIR_CVAR_ASYNC_PROGRESS) {
#if MPL_THREAD_PACKAGE_NAME == MPL_THREAD_PACKAGE_ARGOBOTS
        printf("WARNING: Asynchronous progress is not supported with Argobots\n");
        goto fn_fail;
#else
        if (provided == MPI_THREAD_MULTIPLE) {
            mpi_errno = MPID_Init_async_thread();
            if (mpi_errno)
                goto fn_fail;

            MPIR_async_thread_initialized = 1;
        } else {
            printf("WARNING: No MPI_THREAD_MULTIPLE support (needed for async progress)\n");
        }
#endif
    }

    
/* Added by Abu Naser */

	MPIR_Comm *comm_ptr = NULL;
    MPIR_Comm_get_ptr(MPI_COMM_WORLD, comm_ptr);

    // printf("init 1\n");fflush(stdout);

    MPIR_Errflag_t errflag = MPIR_ERR_NONE;
    int max_node_id,ppn;
	mpi_errno = MPID_Get_max_node_id(comm_ptr, &max_node_id);
	node_cnt = max_node_id +1;
    ppn = (comm_ptr->local_size)/(node_cnt);

    comm_ptr->is_blocked = 0;
    comm_ptr->cyclic_hostfil = 1;
    

    int node_id, left_node_id=-1, right_node_id=-1 ,i;  
	init_rank = comm_ptr->rank;
    int CONCUR_IN;
	int CONCUR_OUT=-2;

    /************ Mohsen (Check Hostfile Type) *************/
    
    mpi_errno = host_checking_comm(comm_ptr->handle, &comm_ptr->node_sizes , &comm_ptr->comm_rank_list , &comm_ptr->comm_rank_list_back);

    if (comm_ptr->rank == 0){
            // printf(COLOR_GREEN" TEST CryptMPI: This is node_size[] = ");
            // for (i=0; i<node_cnt; i++)
            //     printf(" %d ",comm_ptr->node_sizes[i]);            
            // printf("\n");

            // printf("pof2 is %d and roots=%d \n",comm_ptr->pof2,comm_ptr->node_roots_comm->pof2);
            
            if (comm_ptr->is_blocked==1) printf("CryptMPI: Hostfile is Blocked.\n");
            else if (comm_ptr->cyclic_hostfil==1) printf("CryptMPI: Hostfile is Cyclic.\n");
            else printf(COLOR_RED"CryptMPI: Hostfile type is unknown.\n");

            if (comm_ptr->is_uniform) printf("CryptMPI: Hostfile is uniform.\n"); 
            else printf(COLOR_RED"CryptMPI: Hostfile is NOT uniform.\n");

            // printf(COLOR_RESET);
        }
    
    MPI_Barrier(MPI_COMM_WORLD);


    /* *********** End by Mohsen *********** */
	
    security_approach = 0;
     if (DEBUG_INIT_FILE) fprintf(stderr,"[%d] Initeee 01\n",init_rank);
    MPI_SEC_Initial_Key_Aggrement();    

    char *s_value, *o_value, *t_value, *sml_value, *a_value, *c_value, *cb_value, *b_value, *ob_value, *sct_value;
    char *rl_value;
    allocated_shmem = 0;
    if ((s_value = getenv("MV2_SECURITY_APPROACH")) != NULL) {   // Mohsen: "MV2_" is appended to make Flags uniform
        security_approach = (atoi(s_value));        
        // if (security_approach == 333) allocated_shmem = 2; // Sec_Bacst
    }

   /****************************** Added by Mehran *****************************/    

     overlap_decryption = 0;
    if ((o_value = getenv("MV2_OVERLAP_DECRYPTION")) != NULL) {  // Mohsen: "MV2_" is appended to make Flags uniform
        overlap_decryption = (atoi(o_value));
    }

     if (DEBUG_INIT_FILE) fprintf(stderr,"[%d] Initeee 04\n",init_rank);
 
    
    if ((t_value = getenv("MV2_INTER_ALLGATHER_TUNING")) != NULL) {
        int alg = (atoi(t_value));
        mv2_user_allgather_inter = alg;
        if(alg == 14 || alg == 18 || alg == 20){
            allocated_shmem = 1;
            if(security_approach!=0){
                allocated_shmem = 2;
            }
        }
    }

    if (DEBUG_INIT_FILE) fprintf(stderr,"[%d] Initeee 04-1\n",init_rank);

    int initialize_rank_list = 0;
    if ((a_value = getenv("MV2_ALLTOALL_TUNING")) != NULL) {
        int alg = (atoi(a_value));
		inter_alltoall_tuning = alg;
        if(alg == 5){
            if(allocated_shmem == 0){
                allocated_shmem = 1;
            }
            initialize_rank_list = 1;
			concurrent_comm = 1;
        }
    }

    if (DEBUG_INIT_FILE) fprintf(stderr,"[%d] Initeee 04-2\n",init_rank);
    
    if ((rl_value = getenv("MV2_INIT_RANK_LIST")) != NULL) {
        initialize_rank_list = (atoi(rl_value));
    }
    
    comm_ptr->shmem_idx = 0;
    comm_ptr->ctx_shmem_idx = 0;
    /****************************** End by Mehran *********************/

    /********************** Added by Mohsen **************************/
	if (DEBUG_INIT_FILE) fprintf(stderr,"[%d] Initeee 06-1\n",init_rank);
	char *tc;
	char *value;
	
	if ((value = getenv("MV2_LEADER_CNT")) != NULL) 
    leader_cnt = atoi(value);

	if ((value = getenv("MV2_Allgather_Reduce")) != NULL) 
    Allgather_Reduce = atoi(value);

	if ((value = getenv("MV2_CHOPING_SIZE")) != NULL) 
    choping_sz = atoi(value);

	if ((value = getenv("MV2_SUPER_NODE")) != NULL) 
    super_node = atoi(value);

	tc = getenv("MV2_CONCUR_RS_METHOD");   // Reduce-Scatter in 1st step of Ring

	if (tc && (strncmp(tc, "2",2) == 0))	{	
		CONCUR_RS_METHOD = 2;	// Pt2pt-method for 1  &&  ShMem-method for 2
	}


    // CryptMPI: In MPICH, CONCUR_INTER_METHOD has been removed to favour of inter_allreduce_tuning (ckeck allreduc.c)

    inter_allreduce_tuning = 0;
    if ((sct_value = getenv("MV2_INTER_ALLREDUCE_TUNING")) != NULL) {  
        inter_allreduce_tuning = (atoi(sct_value));
    }  

    tc = getenv("MV2_UNSEC_ALLREDUCE_MULTI_LEADER");

	if (tc && (strncmp(tc, "1",1) == 0))	{	
		UNSEC_ALLREDUCE_MULTI_LEADER = 1;	
	}

    tc = getenv("MV2_SHOW_FINAL_RESULT");
	if (tc && (strncmp(tc, "1",1) == 0))	SHOW_FINAL_RESULT = 1;	
	
    if (DEBUG_INIT_FILE) fprintf(stderr,"[%d] Initeee 07\n",init_rank);

    if (comm_ptr->is_uniform==0 && inter_allreduce_tuning>5) {
        //ALLREDUCE_CONCUR_ENABLE=0;
        inter_allreduce_tuning=1;
        if (comm_ptr->rank==0) fprintf(stderr,COLOR_RED"CryptMPI warning: inter_allreduce_tuning set to 1 because of un-symmetric Hostfile arrangement."COLOR_RESET"\n");
    }

     if (comm_ptr->is_uniform==0 && mv2_user_allgather_inter==18) {
        mv2_user_allgather_inter=20;
        if (comm_ptr->rank==0) fprintf(stderr,COLOR_RED"CryptMPI warning: Allgather method is changed from 18 t0 20, due to un-uniform Hostfile arrangement."COLOR_RESET"\n");
    }

    if ((super_node == 1) || (inter_allreduce_tuning>5)) {
        allocated_shmem = 1;                
    }
	
	
	enc_choping_sz = choping_sz + ENC_MSG_TAG_SIZE;
	
    /****************************** End by Mohsen *****************************/

    shmem_leaders = 1;
    if ((sml_value = getenv("MV2_SHMEM_LEADERS")) != NULL) {    // Mohsen: "MV2_" is appended to make Flags uniform
        shmem_leaders = (atoi(sml_value));
    }

    concurrent_comm = 0;
    if ((c_value = getenv("MV2_CONCURRENT_COMM")) != NULL) {    // Mohsen: "MV2_" is appended to make Flags uniform
        concurrent_comm = (atoi(c_value));
    }

    concurrent_bcast = 0;
    if ((cb_value = getenv("MV2_CONCURRENT_BCAST")) != NULL) {  // Mohsen: "MV2_" is appended to make Flags uniform
        concurrent_bcast = (atoi(cb_value));
        if (concurrent_bcast == 1) {
            allocated_shmem = 2;
            concurrent_comm = 1;
            // fprintf(stderr,"[%d] BCAST concurrent_comm=%d  allocated_shmem=%d \n",init_rank,concurrent_comm,allocated_shmem);
        }
    }

    inter_scatter_tuning = 0;
    if ((sct_value = getenv("MV2_INTER_SCATTER_TUNING")) != NULL) {  
        inter_scatter_tuning = (atoi(sct_value));
        if (inter_scatter_tuning==6) {
            allocated_shmem = 1;
            concurrent_comm = 1;
        }
    }    

    inter_gather_tuning = 0;
    if ((sct_value = getenv("MV2_INTER_GATHER_TUNING")) != NULL) {  
        inter_gather_tuning = (atoi(sct_value));
        if (inter_gather_tuning==4) {
            allocated_shmem = 1;
            concurrent_comm = 1;
        }
    }   
	

    /********************** Added by Mohsen **************************/
    
    if (concurrent_comm == 1){
        int check_conc=1;
        if (comm_ptr->node_comm == NULL) check_conc=0;
        
        MPI_Allreduce(MPI_IN_PLACE,&check_conc, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD );
        
        if (check_conc==1){
            MPIR_Comm *comm_ptr = NULL;
            MPIR_Comm_get_ptr(MPI_COMM_WORLD, comm_ptr);     
            mpi_errno = create_concurrent_comm(comm_ptr->handle, comm_ptr->local_size, comm_ptr->rank);
            // mpi_errno = create_sub_comm(comm_ptr->handle, comm_ptr->local_size, comm_ptr->rank);            
            if(mpi_errno) {
                MPIR_ERR_POP(mpi_errno);
            }
            if(allocated_shmem==0 && security_approach==0) allocated_shmem = 1;
        } else {
            if (comm_ptr->rank==0) fprintf(stderr,COLOR_YELLOW"CryptMPI: Can not create concurrent_comm due to single process in node"COLOR_RESET"\n");
            mv2_user_allgather_inter =0 ;
            // ALLREDUCE_CONCUR_ENABLE=0;
            inter_allreduce_tuning=0;            
        }
        
    }

    /****************************** End by Mohsen *********************/

    /*********************** Added by Cong ******************************/
    if ((b_value = getenv("MV2_INTER_BCAST_TUNING")) != NULL) {
        bcast_tuning = (atoi(b_value));               
        if(bcast_tuning == 13) {
            allocated_shmem = 2; 
            if (comm_ptr->rank==0) fprintf(stderr,COLOR_GREEN"CryptMPI: Bcast shared memory creation..."COLOR_RESET"\n");
        }
    }  
    /****************************** End by Cong *****************************/

     if(allocated_shmem != 0){
        int check_shmem =1;
        if (comm_ptr->node_comm == NULL) check_shmem=0;	

        if (DEBUG_INIT_FILE) fprintf(stderr,"[%d] Initee 01 check_shmem=%d\n",init_rank,check_shmem);
        
        MPI_Allreduce(MPI_IN_PLACE,&check_shmem, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD );

        if (DEBUG_INIT_FILE) fprintf(stderr,"[%d] Initee 02 check_shmem=%d\n",init_rank,check_shmem);

        if (check_shmem) init_shmem();	
        else {
            if (comm_ptr->rank==0)  fprintf(stderr,COLOR_YELLOW"CryptMPI Warning: Can not create Shared Memory due to single process in some nodes."COLOR_RESET"\n");
            mv2_user_allgather_inter =0 ;
            // ALLREDUCE_CONCUR_ENABLE=0;
            inter_allreduce_tuning=0;     
            allocated_shmem=0;
        }
    }
    
    if (allocated_shmem == 0)
    {
        if ((t_value = getenv("MV2_SCATTER_SHRD_MEM")) != NULL) // Mohsen: "MV2_" is appended to make Flags uniform
        {
            int alg = (atoi(t_value));
            if (alg == 1)
            {
                allocated_shmem = 2;
                if (comm_ptr->node_comm != NULL) init_shmem();
                //printf("shared memory creation done\n");fflush(stdout);
            }
        }
    }

      if ((t_value = getenv("MV2_PIPELINE_SIZE")) != NULL)  // Mohsen: "MV2_" is appended to make Flags uniform
    {
        choping_sz = (atoi(t_value));
    }
   
    init_crypto();

    is_blocked_backup = comm_ptr->is_blocked;


    if (comm_ptr->rank==0) {
        fprintf(stderr,COLOR_GREEN"CryptMPI: Initialization is Completed!"COLOR_RESET"\n");
        //  if (is_blocked_backup==1) printf("CryptMPI: Hostfile is Blocked.\n"); else  printf("CryptMPI: Hostfile is Cyclic.\n");
         
    }
    init_phase = 0;
	// fprintf(stderr,COLOR_GREEN"[%d] CryptMPI: Inint Completed!"COLOR_RESET"\n",comm_ptr->rank);

    /* ... end of body of routine ... */
    MPIR_FUNC_TERSE_INIT_EXIT(MPID_STATE_MPI_INIT);
    return mpi_errno;

  fn_fail:
    /* --BEGIN ERROR HANDLING-- */
#ifdef HAVE_ERROR_REPORTING
    {
        mpi_errno =
            MPIR_Err_create_code(mpi_errno, MPIR_ERR_RECOVERABLE, FCNAME, __LINE__, MPI_ERR_OTHER,
                                 "**mpi_init", "**mpi_init %p %p", argc, argv);
    }
#endif
    mpi_errno = MPIR_Err_return_comm(0, FCNAME, mpi_errno);
    return mpi_errno;
    /* --END ERROR HANDLING-- */
}















/* Added by Abu Naser */

void init_openmp_key(){

    //if(OPENMP_MULTI_THREAD_ONLY)
    if(security_approach==600 || security_approach==601)
    {    
        global_openmp_ctx = EVP_AEAD_CTX_new(EVP_aead_aes_128_gcm(),
                                             symmetric_key,
                                             symmetric_key_size, 0);
        global_coll_msg_ctx = EVP_AEAD_CTX_new(EVP_aead_aes_128_gcm(),
                                             symmetric_key,
                                             symmetric_key_size, 0);
        global_ctx = EVP_AEAD_CTX_new(EVP_aead_aes_128_gcm(),
                                             symmetric_key,
                                             symmetric_key_size, 0);
        ctx = EVP_AEAD_CTX_new(EVP_aead_aes_128_gcm(),
                                             symmetric_key,
                                             symmetric_key_size, 0);                                                                          
    }
    //else if(OPENMP_PIPE_LINE)
    else if(security_approach==602)
    {
         global_small_msg_ctx = EVP_AEAD_CTX_new(EVP_aead_aes_128_gcm(),
                                             &symmetric_key[symmetric_key_size*2],
                                             symmetric_key_size, 0);
         global_coll_msg_ctx = EVP_AEAD_CTX_new(EVP_aead_aes_128_gcm(),
                                             &symmetric_key[symmetric_key_size*2],
                                             symmetric_key_size, 0);
        global_ctx = EVP_AEAD_CTX_new(EVP_aead_aes_128_gcm(),
                                             &symmetric_key[symmetric_key_size*2],
                                             symmetric_key_size, 0);
        ctx = EVP_AEAD_CTX_new(EVP_aead_aes_128_gcm(),
                                             &symmetric_key[symmetric_key_size*2],
                                             symmetric_key_size, 0);                                                                          
    }
#if 0 //CRYPTMPI_COUNTER_MODE    
    else if(CRYPTMPI_COUNTER_MODE){
        global_counter_ctx = EVP_CIPHER_CTX_new();                                     
        EVP_EncryptInit_ex(global_counter_ctx, EVP_aes_128_ctr(), NULL, symmetric_key, NULL);                                     
    }
#endif

    
    
    /* Get the number of threads */
    openmp_active_thread_no = 1;
    int i;
//#if OMP_DYNAMIC_THREADS_PIPELINE || OMP_DYNAMIC_THREADS_PIPELINE_INNOVATION
    if (security_approach == 602)
    {
        for (i = 0; i < MAX_RANKS_LIMIT; i++)
        {
            ranksIrecvS[i] = 0;
            pendingIsendRequestCount[i] = 0;
        }
    }
//#endif

    no_of_max_omp_threads = omp_get_num_procs();  
    MPI_Comm_size(MPI_COMM_WORLD, &total_process_number);
     MPIR_Comm *cryptmpi_comm_ptr = NULL;
   
    MPIR_Comm_get_ptr( MPI_COMM_WORLD, cryptmpi_comm_ptr );
    MPI_Comm_rank(MPI_COMM_WORLD, &cryptmpi_own_rank);

    cryptmpi_local_rank = cryptmpi_comm_ptr->intranode_table[cryptmpi_own_rank];
    int  num_local, local_rank, local_procs, num_external, external_rank, external_procs;

    // Changed Mohsen
    int mpi_errno = MPIR_Find_local_and_external(cryptmpi_comm_ptr,
                                                 &num_local, &local_rank, &local_procs,
                                                 &num_external, &external_rank, &external_procs,
                                                 &cryptmpi_comm_ptr->intranode_table, &cryptmpi_comm_ptr->internode_table, &cryptmpi_comm_ptr->node_sizes);
    // end
   
     cryptmpi_local_process = num_local;
     int len;
    MPI_Get_processor_name(cryptmpi_process_name, &len );
    for(i=0;i<total_process_number;i++){
         sameNode[i]= -1;
         Crypthandle_probe[i].no_tag = 0;
         //sendtag[i] = 0;
         //recvtag[i] = 0;
    }
     MPI_Allgather( cryptmpi_process_name, len, MPI_CHAR, all_p_names, len, MPI_CHAR, MPI_COMM_WORLD );
    
    for(i=0;i<total_process_number;i++){
        if(strncmp(cryptmpi_process_name, all_p_names+(i*len), len ) == 0 ){
            sameNode[i] = 1;
        }
    }



//#if OMP_DYNAMIC_THREADS_PIPELINE || OMP_DYNAMIC_THREADS_PIPELINE_INNOVATION || OMP_DYNAMIC_THREADS || OMP_DYNAMIC_THREADS_INNOVATION
    if (security_approach == 602)
    {
        int hybrid = 0;
        int spread = 0;
        char *t = getenv("MV2_CPU_BINDING_POLICY");
        if (t)
        {
            if ((strncmp(t, "hybrid", 6) == 0) || (strncmp(t, "HYBRID", 6) == 0))
                hybrid = 1;
        }

        t = getenv("MV2_HYBRID_BINDING_POLICY");
        if (t)
        {
            if ((strncmp(t, "spread", 6) == 0) || (strncmp(t, "SPREAD", 6) == 0))
                spread = 1;
        }

        //printf("hybrid =%d spread = %d\n",hybrid, spread);fflush(stdout);
        if (hybrid == 1 && spread == 1)
            cyptmpi_series_thread = no_of_max_omp_threads - noCommThreads; // liberal, in case of hybrid+spread
        else
            cyptmpi_series_thread = (no_of_max_omp_threads / 2) / cryptmpi_local_process; // liberal

        // printf("cyptmpi_series_thread =%d no_of_max_omp_threads = %d noCommThreads =%d cryptmpi_local_process=%d\n",cyptmpi_series_thread,no_of_max_omp_threads,noCommThreads,cryptmpi_local_process);fflush(stdout);
        //cyptmpi_series_thread = no_of_max_omp_threads - noCommThreads; // liberal, in case of hybrid+spread

        if (cyptmpi_series_thread < 1)
            cyptmpi_series_thread = 1;
    }
//#endif

    return;
}


/* Init counter mode keys */
void init_counter_mode_keys(){
   
    
    global_counter_ctx = EVP_CIPHER_CTX_new(); 
    base_counter_ctx =  EVP_CIPHER_CTX_new();                                    
    EVP_EncryptInit_ex(global_counter_ctx, EVP_aes_128_ctr(), NULL, symmetric_key, NULL);
                                      
    /* set init is done */
    cryptmpi_init_done = 1;
    
    /* Get the number of threads */ 
    openmp_active_thread_no = 1;
 int i;
//#if PRE_COMPUTE_COUNTER_MODE
 if (security_approach == 702)
 {
     for (i = 0; i < MAX_RANKS_LIMIT; i++)
     {
         ranksIrecvS[i] = 0;
         pendingIsendRequestCount[i] = 0;
     }
     memset(zeros, 0, MAX_COMMON_COUNTER_SZ);
 }
//#endif

 no_of_max_omp_threads = omp_get_num_procs();  
    MPI_Comm_size(MPI_COMM_WORLD, &total_process_number);
     MPIR_Comm *cryptmpi_comm_ptr = NULL;
   
    MPIR_Comm_get_ptr( MPI_COMM_WORLD, cryptmpi_comm_ptr );
    MPI_Comm_rank(MPI_COMM_WORLD, &cryptmpi_own_rank);

    cryptmpi_local_rank = cryptmpi_comm_ptr->intranode_table[cryptmpi_own_rank];
    int  num_local, local_rank, local_procs, num_external, external_rank, external_procs;

    // Changed Mohsen
    int mpi_errno = MPIR_Find_local_and_external(cryptmpi_comm_ptr,
                                                 &num_local, &local_rank, &local_procs,
                                                 &num_external, &external_rank, &external_procs,
                                                 &cryptmpi_comm_ptr->intranode_table, &cryptmpi_comm_ptr->internode_table, &cryptmpi_comm_ptr->node_sizes);
    // end
   
     cryptmpi_local_process = num_local;
     int len;
    MPI_Get_processor_name(cryptmpi_process_name, &len );
    for(i=0;i<total_process_number;i++){
         sameNode[i]= -1;
         Crypthandle_probe[i].no_tag = 0;
    }
     MPI_Allgather( cryptmpi_process_name, len, MPI_CHAR, all_p_names, len, MPI_CHAR, MPI_COMM_WORLD );
    
    for(i=0;i<total_process_number;i++){
        if(strncmp(cryptmpi_process_name, all_p_names+(i*len), len ) == 0 ){
            sameNode[i] = 1;
        }
    }

//#if PRE_COMPUTE_COUNTER_MODE
    if (security_approach == 702)
    {
        /* init common counter array */
        //RAND_bytes(Send_common_IV, 16);
        RAND_bytes(Send_common_IV, 32);
        if (INITIAL_COMMON_COUNTER_SZ)
        {
            EVP_EncryptInit_ex(global_counter_ctx, NULL, NULL, NULL, Send_common_IV);
            EVP_EncryptUpdate(global_counter_ctx, enc_common_buffer, &len, zeros, INITIAL_COMMON_COUNTER_SZ);
            enc_common_counter = INITIAL_COMMON_COUNTER_SZ / 16;
            common_compute_size = INITIAL_COMMON_COUNTER_SZ;
            enc_common_start = 0;
            enc_common_end = INITIAL_COMMON_COUNTER_SZ;
            counter_needto_send = 0;
        }
        else
        {
            enc_common_counter = 0;
            common_compute_size = 0;
            enc_common_start = 0;
            enc_common_end = 0;
            counter_needto_send = 0;
        }
        mpi_errno = MPI_Allgather(Send_common_IV, 32, MPI_UNSIGNED_CHAR, Recv_common_IV, 32, MPI_UNSIGNED_CHAR, MPI_COMM_WORLD);
    }
//#elif BASE_COUNTER_MODE
    else // 700 and 701
    {
        RAND_bytes(Send_common_IV, 16);
        base_global_counter = 0;
        mpi_errno = MPI_Allgather(Send_common_IV, 16, MPI_UNSIGNED_CHAR, Recv_common_IV, 16, MPI_UNSIGNED_CHAR, MPI_COMM_WORLD);
    }
//#endif

//#if PRE_COMPUTE_COUNTER_MODE
    if (security_approach == 702)
    {
        int hybrid = 0;
        int spread = 0;
        char *t = getenv("MV2_CPU_BINDING_POLICY");
        if (t)
        {
            if ((strncmp(t, "hybrid", 6) == 0) || (strncmp(t, "HYBRID", 6) == 0))
                hybrid = 1;
        }

        t = getenv("MV2_HYBRID_BINDING_POLICY");
        if (t)
        {
            if ((strncmp(t, "spread", 6) == 0) || (strncmp(t, "SPREAD", 6) == 0))
                spread = 1;
        }

        //printf("hybrid =%d spread = %d\n",hybrid, spread);fflush(stdout);
        if (hybrid == 1 && spread == 1)
            cyptmpi_series_thread = no_of_max_omp_threads - noCommThreads; // liberal, in case of hybrid+spread
        else
            cyptmpi_series_thread = (no_of_max_omp_threads / 2) / cryptmpi_local_process; // liberal

        if (cyptmpi_series_thread < 1)
            cyptmpi_series_thread = 1;

        for (i = 0; i < cyptmpi_series_thread; i++)
        {
            local_counter_ctx[i] = EVP_CIPHER_CTX_new();
            EVP_EncryptInit_ex(local_counter_ctx[i], EVP_aes_128_ctr(), NULL, symmetric_key, NULL);
        }
    }
//#endif
    return;
    }

//void init_crypto(unsigned char *key){
void init_crypto(){    
   
    ctx_enc = EVP_CIPHER_CTX_new();
	
    if(symmetric_key_size == 16){
	    EVP_EncryptInit_ex(ctx_enc, EVP_aes_128_ecb(), NULL, symmetric_key, NULL);
    }
	else{
        EVP_EncryptInit_ex(ctx_enc, EVP_aes_256_ecb(), NULL, symmetric_key, NULL); 
    }
	
	/********* OCB *********/

	
	// ocb_ctx = ae_allocate(NULL);
	ocb_enc_ctx = ae_allocate(NULL);
	ocb_enc_ctx2 = ae_allocate(NULL);
	ocb_enc_ctx3 = ae_allocate(NULL);
	ocb_enc_ctx4 = ae_allocate(NULL);
	ocb_dec_ctx = ae_allocate(NULL);
	ocb_dec_ctx2 = ae_allocate(NULL);
	ocb_dec_ctx3 = ae_allocate(NULL);
	ocb_dec_ctx4 = ae_allocate(NULL);
	
    // ae_init(ocb_ctx, ocb_key, 16, 12, 16);
    ae_enc_init(ocb_enc_ctx, ocb_key, 16,1);
    ae_enc_init(ocb_enc_ctx2, ocb_key,  16, 0);
    ae_enc_init(ocb_enc_ctx3, ocb_key,  16, 0);
    ae_enc_init(ocb_enc_ctx4, ocb_key,  16, 0);

	ae_init(ocb_dec_ctx, ocb_key, 16, 12, 16);   
	ae_init(ocb_dec_ctx2, ocb_key, 16, 12, 16);   
	ae_init(ocb_dec_ctx3, ocb_key, 16, 12, 16);   
	ae_init(ocb_dec_ctx4, ocb_key, 16, 12, 16);   
	
	/********* End of OCB *********/	


if (security_approach == 600 || security_approach == 601 || security_approach == 602)
    {
         init_openmp_key();         // will initialize cryptMPI-AesGcm
    }
    else if(security_approach == 700 || security_approach == 701 || security_approach == 702)
    {
         init_counter_mode_keys();   // will initialize counter-mode
    }
    else 
    {
        global_ctx = EVP_AEAD_CTX_new(EVP_aead_aes_128_gcm(),
                                             symmetric_key,
                                             symmetric_key_size, 0);
        ctx = EVP_AEAD_CTX_new(EVP_aead_aes_128_gcm(),
                                             symmetric_key,
                                             symmetric_key_size, 0);                                     
        
        global_coll_msg_ctx = EVP_AEAD_CTX_new(EVP_aead_aes_128_gcm(),
                                             symmetric_key,
                                             symmetric_key_size, 0);


		// if (init_rank==0) fprintf(stderr,COLOR_GREEN"CryptoMPI: Crypto variable is created with security_approach = %d"COLOR_RESET"\n",security_approach);
    }

    return;                          
}


void MPI_SEC_Initial_Key_Aggrement(){    
    int wrank, wsize;
    int mpi_errno = MPI_SUCCESS;   
    MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
    MPI_Comm_size(MPI_COMM_WORLD, &wsize);

   
    int keylen, i, ret;
    unsigned char  *root_public_key, *public_key, *all_process_public_key;
    unsigned char  *encrypted_text;
    unsigned char recv_buf[3000];
    int encrypted_len, decrypted_len, pub_key_size, next;
    MPI_Status status;
    BIGNUM *bn;
    BIGNUM *bnPublic = BN_new();
    BIGNUM *exponent = BN_new();
    BIGNUM *bnPrivate = BN_new();
 

    bn = BN_new();
    BN_set_word(bn, RSA_F4);

    RSA *rsaKey, *temprsa;
    rsaKey = RSA_new();
    temprsa = RSA_new();

    /* Generate rsa keypair */
    RSA_generate_key_ex(rsaKey,  2048, bn,  NULL);

    /* Get the public key and exponent */
    RSA_get0_key(rsaKey, &bnPublic, &exponent, &bnPrivate);

   // Chnaged by Mohsen
    // all_process_public_key = (unsigned char *)MPIU_Malloc(wsize*256+10);
    // encrypted_text = (unsigned char *)MPIU_Malloc(wsize*256+10);

    all_process_public_key = (unsigned char *)MPL_malloc(wsize*256+10, MPL_MEM_USER );
    encrypted_text = (unsigned char *)MPL_malloc(wsize*256+10, MPL_MEM_USER );

    // end

    pub_key_size = BN_num_bytes(bnPublic);
    public_key = (unsigned char *) malloc(256+10);
    ret = BN_bn2bin(bnPublic, public_key);

    /* send the public key to root process */ 
    /*mpi_errno = MPI_Gather(public_key, 256, MPI_UNSIGNED_CHAR,
               all_process_public_key, 256, MPI_UNSIGNED_CHAR,
               0, MPI_COMM_WORLD);*/
    
    int *disp = (int *)malloc(wsize*sizeof(int));
    int *sendcnt = (int *)malloc(wsize*sizeof(int));
    int *recvcnt = (int *)malloc(wsize*sizeof(int));   
    
    for(i=0;i<wsize;i++)
    {
        disp[i] = i*256;
        recvcnt[i] = 256;
    }
    mpi_errno = MPI_Gatherv(public_key, 256, MPI_UNSIGNED_CHAR,
               all_process_public_key, recvcnt, disp, MPI_UNSIGNED_CHAR,
               0, MPI_COMM_WORLD);     


    /* set the key size */
    if( SYMMETRIC_KEY_SIZE == 32)
          symmetric_key_size = 32;

    if( wrank ==0 ){  
        BIGNUM *bnOthPubkey = BN_new();
        
        /* Generate a random key */
        RAND_bytes(symmetric_key, (symmetric_key_size*2));
        
        symmetric_key[symmetric_key_size*2] = '\0';
       

        int next;
        /* Encrypt random key with the public key of other process */
          for(i=1; i<wsize; i++){  
            next = (i*256);
            
            bnOthPubkey = BN_bin2bn((all_process_public_key+next), 256, NULL );
          
            temprsa = NULL;
            temprsa = RSA_new();
            if(RSA_set0_key(temprsa, bnOthPubkey, exponent, NULL)){  
                next = i* 256;
                ret = RSA_public_encrypt((symmetric_key_size*2), (unsigned char*)symmetric_key, (unsigned char*)(encrypted_text+next), 
                                        temprsa, RSA_PKCS1_OAEP_PADDING); 

                if(ret!=-1){
                #if ENABLE_SECURE_MPI_DEBUG                  
                    printf("[Rank %d] Encrypted %d bytes for %d\n",wrank, ret, i); fflush(stdout);
                #endif                    
                }
                else{
                     printf("[Rank %d] Encryption failed for for %d\n",wrank,  i); fflush(stdout);   
                }                         
            }
            else{
                printf("RSA_set0_key: Failed in %d for %d\n",wrank, i); fflush(stdout);
            }

        }
        
    }
   
    /* send/recv encrypted symmetric key from/to processes */
    /*mpi_errno = MPI_Scatter(encrypted_text, 256, MPI_UNSIGNED_CHAR, recv_buf, 256, 
                                    MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);*/

     for(i=0;i<wsize;i++)
    {
        disp[i] = i*256;
        sendcnt[i] = 256;
    } 

     mpi_errno = MPI_Scatterv(encrypted_text, sendcnt, disp, MPI_UNSIGNED_CHAR, recv_buf, 256, 
                                    MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);                               
                           

     if( wrank != 0 ){

        /* Now decrypt the key */
         ret = RSA_private_decrypt(256, (unsigned char*)recv_buf, (unsigned char*)symmetric_key,
                       rsaKey, RSA_PKCS1_OAEP_PADDING);

        if(ret!=-1){
            #if ENABLE_SECURE_MPI_DEBUG             
            printf("[Rank %d] Decrypted size is %d\n",wrank, ret); 
            //symmetric_key[ret] = '\0';
            //printf("[%d] symmetric key is: %s\n",wrank, symmetric_key);
            fflush(stdout);
            #endif            
        } 
        else{
                printf("RSA_private_decrypt: Failed in %d\n",wrank);
              
                fflush(stdout);
            }              
                                   

    }
    /* initialize with key */
    //init_crypto(symmetric_key);
    // Chnage by Mohsen
    //MPIU_Free(encrypted_text);
    MPL_free(encrypted_text);
    //MPIU_Free(all_process_public_key);
    MPL_free(all_process_public_key);
    // end
    free(sendcnt);
    free(recvcnt);
    free(disp);
    return;
}

int init_shmem(){
    static const char function_name[] = "init_shmem";
    int mpi_errno = MPI_SUCCESS;
    // int security_approach, overlap_decryption;
	int rank;
    //printf("Hello from init_shmem\n");
    MPIR_Comm *comm_ptr = NULL;
    MPIR_Comm_get_ptr(MPI_COMM_WORLD, comm_ptr);

    if (DEBUG_INIT_FILE) fprintf(stderr,"[%d] SHM 01 \n",init_rank);  
       //TODO: Allocate Shmem
    int  SHM_size = 8 * 1024 *1024;
    size_t shmem_size = (comm_ptr->local_size) * SHM_size; //* 4 * 4 * 1024 *1024;
    size_t ciphertext_shmem_size = (comm_ptr->local_size) * (SHM_size + 16 + 12) ;// (1024 * 1024 * 4 + 16 + 12);

    if (0){
        shmem_key = 12345; //32984;
        ciphertext_shmem_key = 67890; //56982;
    } else {
        shmem_key = 13131;
        ciphertext_shmem_key = 24242;
    }

    if(comm_ptr->node_comm->rank == 0){
        
        shmid = shmget(shmem_key, shmem_size, IPC_CREAT | 0666);

        if(allocated_shmem==2){
            ciphertext_shmid = shmget(ciphertext_shmem_key, ciphertext_shmem_size, IPC_CREAT | 0666);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (DEBUG_INIT_FILE) fprintf(stderr,"[%d] SHM 03 \n",init_rank);  
    
    if(comm_ptr->node_comm->rank > 0){
        shmid = shmget(shmem_key, shmem_size, 0666);

        if(allocated_shmem==2){
            ciphertext_shmid = shmget(ciphertext_shmem_key, ciphertext_shmem_size, 0666);

        }
    }

    if (DEBUG_INIT_FILE) fprintf(stderr,"[%d] SHM 04 \n",init_rank);  
    
    if (shmid < 0) {
        printf("%s",strerror(errno));
        printf(" SHM ERROR 1\n");
        goto fn_fail;
    }
    
    // attach shared memory 

    shmem_buffer = (void *) shmat(shmid, NULL, 0);
    if (shmem_buffer == (void *) -1) {
        printf(" SHM ERROR 2\n");
        goto fn_fail;
    }
    if(allocated_shmem==2){
        ciphertext_shmem_buffer = (void *) shmat(ciphertext_shmid, NULL, 0);
        if (ciphertext_shmem_buffer == (void *) -1) {
            printf(" SHM ERROR 3\n");
            goto fn_fail;
        }

    }

    if (comm_ptr->rank==0) fprintf(stderr,COLOR_GREEN"CryptMPI: Shared Memory with size of %d Bytes (index: %d) is allocated with security_approach: %d"COLOR_RESET"\n",SHM_size,allocated_shmem,security_approach);

    return mpi_errno;
    
    fn_fail:
        mpi_errno = MPIR_Err_return_comm( 0, function_name, mpi_errno );
    
    return mpi_errno;
    
}

#if 1 
int host_checking_comm (MPI_Comm comm, int **node_sizes_p, int **comm_rank_list_p, int **comm_rank_list_back_p)
{
    static const char function_name[] = "host_checking_comm";
    int mpi_errno = MPI_SUCCESS;
    MPIR_Comm* comm_ptr = NULL;
    MPIR_CHKPMEM_DECL(3);

    int *comm_rank_list;
    int *comm_rank_list_back;

    MPIR_Comm_get_ptr( comm, comm_ptr );
    if (comm_ptr->local_size <= 1) {
        return mpi_errno;
    }

    int node_id,left_node_id,right_node_id;

    int max_node_id,ppn;
	mpi_errno = MPID_Get_max_node_id(comm_ptr, &max_node_id);
	node_cnt = max_node_id +1;
    ppn = (comm_ptr->local_size)/(node_cnt);
    mpi_errno = MPID_Get_max_node_id(comm_ptr, &max_node_id);
	int node_cnt = max_node_id +1;
    
    /************ Mohsen (Check Hostfile Type) *************/  

    MPID_Get_node_id(comm_ptr, comm_ptr->rank, &node_id);
    if (comm_ptr->rank < (comm_ptr->local_size - 1)) MPID_Get_node_id(comm_ptr, comm_ptr->rank +1 , &right_node_id);    
    if (comm_ptr->rank > 0) MPID_Get_node_id(comm_ptr, comm_ptr->rank -1 , &left_node_id);
    
	
	if (node_cnt==comm_ptr->local_size) comm_ptr->cyclic_hostfil = 1;
    else if ((comm_ptr->rank < (comm_ptr->local_size - 1)) && (node_id == right_node_id)) comm_ptr->is_blocked = 1;
    else if ((comm_ptr->rank > 0) && (node_id == left_node_id)) comm_ptr->is_blocked = 1;

    MPI_Allreduce(MPI_IN_PLACE, &(comm_ptr->is_blocked), 1, MPI_INT, MPI_MAX, comm );

    if (comm_ptr->is_blocked==1) comm_ptr->cyclic_hostfil=0;


    /************ Mohsen (Prepare Node_sizes Arrary) ****************
        In Root (rank=0) comm_ptr->node_roots_comm->rank is NULL 
        If only 1 process exists in a node, comm_ptr->node_comm is NULL
    */  
         
    comm_ptr->is_uniform=1;
    
    
    if (comm_ptr->local_size % node_cnt !=0) {
        
        comm_ptr->is_uniform=0;
    }
    else if (comm_ptr->node_comm != NULL){                       
        if (((comm_ptr->node_comm->local_size)*node_cnt) != (comm_ptr->local_size)) {
            
            comm_ptr->is_uniform=0;
        }
    }
		
    MPI_Allreduce(MPI_IN_PLACE, &(comm_ptr->is_uniform), 1, MPI_INT, MPI_MIN, comm );

     /************ Mohsen (Prepare Node_sizes Arrary) ****************/

    MPIR_Errflag_t errflag = MPIR_ERR_NONE;

    int *node_sizes,i;

    MPIR_CHKPMEM_MALLOC(node_sizes, int *, sizeof(int) * node_cnt, mpi_errno,
                        "node_sizes", MPL_MEM_COMM);

    if (node_cnt==comm_ptr->local_size) {
        for (i=0; i<node_cnt; i++)
            node_sizes[i]=1;
    } else if (comm_ptr->node_roots_comm != NULL) {
        MPIR_Comm *leader_commptr = NULL;
        MPIR_Comm *shmem_commptr = NULL;
        leader_commptr = comm_ptr->node_roots_comm;
        shmem_commptr = comm_ptr->node_comm;

        if (comm_ptr->node_comm == NULL && comm_ptr->node_roots_comm != NULL) {
            node_sizes[leader_commptr->rank] = 1;
        } 
        else if (shmem_commptr->local_size != NULL && leader_commptr->rank != NULL) {
            node_sizes[leader_commptr->rank] = shmem_commptr->local_size;        
        } else if (shmem_commptr->local_size != NULL && comm_ptr->rank == 0) {
            node_sizes[0] = shmem_commptr->local_size;        
        }
        
        mpi_errno = MPIR_Allgather_intra_ring(MPI_IN_PLACE, 1, MPI_INT, node_sizes, 1, MPI_INT, comm_ptr->node_roots_comm, &errflag);

        /*if (comm_ptr->rank == 0){
            printf("CryptMPI: *** is_uniform = %d This is node_size[] = ",comm_ptr->is_uniform);
            for (i=0; i<node_cnt; i++)
                printf(" %d ",node_sizes[i]);            
            printf("\n");
        }*/
        
    }

        /************ Mohsen (Prepare comm_rank_list Arrary) *************/  

    MPIR_CHKPMEM_MALLOC(comm_rank_list, int *, sizeof(int) * comm_ptr->remote_size, mpi_errno,
                        "comm_rank_list", MPL_MEM_COMM);

    
    // CryptMPI: comm_rank_list_back conver comm_rank_list's id to original rank. 
    // It mostly uses for cyclic_hostfil.
    MPIR_CHKPMEM_MALLOC(comm_rank_list_back, int *, sizeof(int) * comm_ptr->remote_size, mpi_errno,
                        "comm_rank_list_back", MPL_MEM_COMM);

    if (comm_ptr->cyclic_hostfil==1){
        for (i = 0; i < comm_ptr->remote_size; i++){
            comm_rank_list[i] = (i % node_cnt)*ppn + (i / node_cnt);
            comm_rank_list_back[comm_rank_list[i]] = i;
        }
    } else if (comm_ptr->is_blocked==1){
        for (i = 0; i < comm_ptr->remote_size; i++){
            comm_rank_list[i] = i;
            comm_rank_list_back[i] = i;
        }            
    } else if (comm_ptr->rank==0) {
        comm_rank_list = NULL;
        comm_rank_list_back = NULL;
        fprintf(stderr,COLOR_RED"CryptMPI warning: Can not arrange comm_rank_list[] based on Hostfile type."COLOR_RESET"\n");
    }
        
    // fprintf(stderr,"[%d  %d  %d]\n",comm_ptr->rank,comm_rank_list[comm_ptr->rank],comm_rank_list_back[comm_rank_list[comm_ptr->rank]]);


    if (comm_ptr->rank == 0 && 0){
            int remote_node_id;
            printf(COLOR_CYAN"comm_rank_list[] = \n");
            for (i=0; i<comm_ptr->remote_size; i++){
                MPID_Get_node_id(comm_ptr,comm_rank_list_back[comm_rank_list[i]], &remote_node_id);
                printf("list=%d  real=%d  node=%d\n",comm_rank_list[i],comm_rank_list_back[comm_rank_list[i]],remote_node_id);            
            }
            
            printf(COLOR_RESET);
        }

    if (node_sizes_p != NULL)
        *node_sizes_p = node_sizes;

    if (comm_rank_list_p != NULL){
        *comm_rank_list_p = comm_rank_list;
        // if (comm_ptr->rank==0) fprintf(stderr,COLOR_RED"CryptMPI warning: NULL comm_rank_list[]"COLOR_RESET"\n");

    }
        

    if (comm_rank_list_back_p != NULL)
        *comm_rank_list_back_p = comm_rank_list_back;
   

    //if (comm_ptr->rank == 0) fprintf(stderr,"End of host_check\n");

    if(mpi_errno) {
       MPIR_ERR_POP(mpi_errno);
    }    

    return (mpi_errno);
    fn_fail:
        mpi_errno = MPIR_Err_return_comm( 0, function_name, mpi_errno );

    return (mpi_errno);
}




int create_concurrent_comm (MPI_Comm comm, int size, int my_rank)
{
    init_phase=1;
    static const char function_name[] = "create_concurrent_comm";
    int mpi_errno = MPI_SUCCESS;
    MPIR_Comm* comm_ptr = NULL;

    MPIR_Comm_get_ptr( comm, comm_ptr );
    if (size <= 1) {
        return mpi_errno;
    }
    
    comm_ptr->concurrent_comm =MPI_COMM_NULL;
    
    /* get our rank and the size of this communicator */
    int local_rank = comm_ptr->node_comm->rank;
    
    mpi_errno = PMPI_Comm_split(comm, local_rank, my_rank, &(comm_ptr->concurrent_comm));

    if(mpi_errno) {
       MPIR_ERR_POP(mpi_errno);
    }    
    init_phase=0;
    return (mpi_errno);
    fn_fail:
        mpi_errno = MPIR_Err_return_comm( 0, function_name, mpi_errno );

    return (mpi_errno);
}

#endif


/* end of add */
