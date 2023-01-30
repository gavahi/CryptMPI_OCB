
# CryptMPI: A Fast Encrypted MPI Library using GCM and OCB
CryptMPI provides secure inter-node communication in the HPC cluster and cloud environment.
I implemented a prototypes in MPICH-3.3 (for Ethernet) using AES-GCM from the [BoringSSL library](https://boringssl.googlesource.com/boringssl/) and also 
[OCB](https://web.cs.ucdavis.edu/~rogaway/ocb/ocb-faq.htm).

OCB could be 2-6 times faster than GCM, but a precise answer depends on a many factors. Please refer to the [performance page](https://web.cs.ucdavis.edu/~rogaway/ocb/performance/) for moew details across various platforms.

For next step, I will add MVAPICH2-2.3.3(for Infiniband) as well.

Up to now, I implemented secure approach for following routines: 

| Pont-to-point routines|  Collective routines   |
|:---------------------:|:----------------------:|
| [MPI_Send](#point-to-point)         	| [MPI_Gather](#gather)          	 |
| MPI_Recv          	| [MPI_Scatter](#scatter)        |
| MPI_Wait       	| [MPI_Alltoall](#alltoall)          |
| MPI_Waitall          	| [MPI_Allgather](#allgather)    |
| MPI_Isend    	 		| [MPI_Allreduce](#allreduce)  	 |
| MPI_Irecv     	    | [MPI_Bcast](#bcast)		   	 |


## Installation
To install cryptMPI for the Infiniband and Ethernet network please follow following steps:
#### Package requirement
 autoconf version... >= 2.67
 automake version... >= 1.15
 libtool version... >= 2.4.4

To install the above package you could use get-lib.sh

After installing, set the path for the above packages.

```bash
export PATH=/HOME_DIR/automake/bin:$PATH
export LD_LIBRARY_PATH=/HOME_DIR/automake/lib:$LD_LIBRARY_PATH
```


#### Installation for BoringSSL

Download and unzip it:
```bash
wget https://github.com/google/boringssl/archive/master.zip
unzip
```

BoringSSL needs GO package. So, install GO in this way:

```bash
cd BORINGSSL-DIR/
wget https://golang.org/dl/go1.16.2.linux-amd64.tar.gz 
tar xzf go1.16.2.linux-amd64.tar.gz
export GOROOT=/BORINGSSL-DIR/go
export GOPATH=/BORINGSSL-DIR/go
export PATH=$GOPATH/bin:$GOROOT/bin:$PATH
```

After installing GO, countine BoringSSL installagtion:

```bash
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=1 ..
make
```

#### Installation for OCB

Download OCB_LIBRARY tar file from current repo and unzip it.
After that, countine installagtion:

```bash
cd OCB_LIBRARY
gcc  -march=native -O3 -c -fPIC ocb.c -o ocb.o
gcc -shared -o libocb.so -fPIC ocb.o
```
It must generate libocb.so file which we need it for next steo.


#### Installation for CryptMPI-MPICH (Ethernet) 
Steps:
```bash
./autogen.sh
./configure --prefix=/INSTALLITION_DIR/install
```

In the *Makefile*: 

1- add -L/YOUR_PATH_TO_BORINGSSL/build/crypto -lcrypto in *LIBS*

(e.g. LIBS =-L/YOUR_PATH_TO_BORINGSSL/build/crypto -lcrypto -lpthread)

2- add -I/YOUR_PATH_TO_BORINGSSL/include -fopenmp in *CFLAGS*

3- add -fopenmp in *LDFLAGS*


```bash
export LD_LIBRARY_PATH=/YOUR_PATH_TO_BORINGSSL/build/crypto
make clean
make -j
make install
```

## Usage
To run MPI applications using CryptMPI please follow following steps:

#### CryptMPI-MPICH (Ethernet)
```bash
export LD_LIBRARY_PATH=/MPICH_INSTALL_DIR/install/lib:/BORINGSSL_INSTALL_DIR/build/crypto:/BORINGSSL_INSTALL_DIR/
```


## Performance measurement
The performance was measured on 100Gb/s Infiniband and 10Gb/s Ethernet network. 
I tested it on bare metal (Physical server without virtualization) and also on Container platform.
I developed a container-based virtual cluster using Docker or Singularity as container engines. 
Additionally, I used container orchestrations like Docker swarm and Kubernetes to provide mechanisms to enhance inter-node data communication security.
I used several Container Network Interfaces (CNI) for Kubernetes to handle network security with different encryption methods: Calico, Antrea, Cillium, etc.
Finally, I measured the encryption performance provided by container orchestrations then the CryptMPI approach.
The empirical evaluation on multiple production systems shows that my proposed encrypting approach in CryptMPI achieves 2 to 10 times improvement (depending on message size, number of nodes and number of processes per node) over Docker swarm and Kubernetes built-in security mechanisms.

In my evaluation process, I used many MPI application and benchmarks such as:
- OSU micro-benchmark 5.8
- NAS parallel benchmarks 3.3.1 
- N-Body

## Flags List

The flags are discussed in this section, will work for MVAPICH also when it is avaiable.


#### Point-to-Point
It includes send.c/recv.c/irecv.c/isend.c/wait/waitall communcations:

In current version, we only provided the naive modes of GCM and OCB using following flags:
```bash
export MV2_SECURITY_APPROACH=401
echo "Naive GCM"


export MV2_SECURITY_APPROACH=402
echo "Naive OCB"
```

#### Gather

- MPICH

```bash
export MV2_SECURITY_APPROACH=301
echo "Naive GCM"


export MV2_SECURITY_APPROACH=311
echo "Naive OCB"


export MV2_SECURITY_APPROACH=312
echo "Naive OCB 2 Unrolling"


export MV2_SECURITY_APPROACH=313
echo "Naive OCB 4 Unrolling"


export MV2_SECURITY_APPROACH=302
export MV2_INTER_GATHER_TUNING=3 
echo "Opportunistic Binomial Gather (Direct - No Shared-Mem) [Gather_intra]" 


export MV2_SECURITY_APPROACH=302
export MV2_INTER_GATHER_TUNING=4
export MV2_CONCURRENT_COMM=1
echo "CHS [Gather_MV2_Direct_CHS]" 
```

#### Scatter


```bash
unset MV2_INTER_SCATTER_TUNING
export MV2_SECURITY_APPROACH=221
echo "Naive GCM"


export MV2_SECURITY_APPROACH=222
echo "Naive OCB"


export MV2_SECURITY_APPROACH=223
echo "Naive OCB 2 Unrolling"


export MV2_SECURITY_APPROACH=224
echo "Naive OCB 4 Unrolling"


export MV2_SECURITY_APPROACH=200
export MV2_INTER_SCATTER_TUNING=6
export MV2_CONCURRENT_COMM=1
echo "b-s-c: concurrent with shared memory [Scatter_MV2_Direct_CHS]"


export MV2_SECURITY_APPROACH=200
export MV2_INTER_SCATTER_TUNING=7
echo "rr: round robin [Scatter_MV2_Direct_no_shmem_intra_RR]"


export MV2_SECURITY_APPROACH=200
export MV2_INTER_SCATTER_TUNING=9
echo "n-bcast: hierarchical broadcast [Scatter_MV2_Direct_HBcast]"


export MV2_SECURITY_APPROACH=200
export MV2_INTER_SCATTER_TUNING=10
echo "MPIR_Scatter_MV2_Direct_no_shmem [Scatter_MV2_Direct_no_shmem]"


export MV2_SECURITY_APPROACH=200
export MV2_INTER_SCATTER_TUNING=11
echo "MPIR_Scatter_MV2_two_level_Direct [Scatter_MV2_two_level_Direct]"


export MV2_SECURITY_APPROACH=200
export MV2_INTER_SCATTER_TUNING=12
echo "MPIR_Scatter_MV2_Direct [Scatter_MV2_Direct]"
```

#### AlltoAll


- MPICH

```bash

unset MV2_ALLTOALL_TUNING
unset MV2_SECURITY_APPROACH
unset MV2_INTER_ALLGATHER_TUNING
unset MV2_CONCURRENT_COMM
echo "Default"


export MV2_SECURITY_APPROACH=1002
echo "Naive"


export MV2_SECURITY_APPROACH=1003
echo "Naive OCB"


export MV2_SECURITY_APPROACH=1004
echo "Naive OCB 2 Unrolling"


export MV2_SECURITY_APPROACH=1005
echo "Naive OCB 4 Unrolling"


export MV2_ALLTOALL_TUNING=0
export MV2_SECURITY_APPROACH=2001
echo "OBruck"


export MV2_ALLTOALL_TUNING=2
export MV2_SECURITY_APPROACH=2001
echo "OSD"


export MV2_ALLTOALL_TUNING=5
unset MV2_SECURITY_APPROACH
echo "CHS"


export MV2_ALLTOALL_TUNING=5
export MV2_SECURITY_APPROACH=2002
echo "Naive-CHS"


export MV2_ALLTOALL_TUNING=5
export MV2_SECURITY_APPROACH=2001
echo "O-CHS"
```


#### Allgather


- MPICH

```bash
export MV2_INTER_ALLGATHER_TUNING=1
export MV2_SECURITY_APPROACH=1001
echo "MPIR_Naive_Sec_Allgather"


export MV2_INTER_ALLGATHER_TUNING=2
export MV2_SECURITY_APPROACH=1001
echo "Naive OCB"


export MV2_INTER_ALLGATHER_TUNING=3
export MV2_SECURITY_APPROACH=1001
echo "Naive OCB 2 Unrolling"


export MV2_INTER_ALLGATHER_TUNING=4
export MV2_SECURITY_APPROACH=1001
echo "Naive OCB 4 Unrolling"


export MV2_INTER_ALLGATHER_TUNING=8 
export MV2_SECURITY_APPROACH=2005
echo "MPIR_Allgather_Bruck_SEC"


export MV2_INTER_ALLGATHER_TUNING=9 
export MV2_SECURITY_APPROACH=2005
echo "MPIR_Allgather_Ring_SEC"


export MV2_INTER_ALLGATHER_TUNING=10 
export MV2_SECURITY_APPROACH=2005
echo "MPIR_Allgather_RD_MV2"


export MV2_INTER_ALLGATHER_TUNING=14
export MV2_SECURITY_APPROACH=2006
export MV2_SHMEM_LEADERS=1
echo "ALLGATHER_2LVL_SHMEM"


export MV2_INTER_ALLGATHER_TUNING=16 
export MV2_SECURITY_APPROACH=2005
echo "MPIR_Allgather_NaivePlus_RDB_MV2"


export MV2_INTER_ALLGATHER_TUNING=18
export MV2_SECURITY_APPROACH=2006
echo "MPIR_2lvl_SharedMem_Concurrent_Encryption_Allgather(Single-leader)"


export MV2_INTER_ALLGATHER_TUNING=20
export MV2_SHMEM_LEADERS=1
export MV2_CONCURRENT_COMM=1
export MV2_SECURITY_APPROACH=2006
echo "MPIR_Allgather_2lvl_Concurrent_Multileader_SharedMem(Multi-leaders)"


export MV2_INTER_ALLGATHER_TUNING=21
export MV2_SECURITY_APPROACH=2007
echo "MPIR_2lvl_Allgather_MV2(SH1 Not-uniform)"


export MV2_INTER_ALLGATHER_TUNING=22
export MV2_SECURITY_APPROACH=2007
echo "MPIR_2lvl_Allgather_nonblocked_MV2"
```

 
#### Allreduce


- MPICH

```bash
export MV2_SECURITY_APPROACH=2005 
export MV2_INTER_ALLREDUCE_TUNING=1
echo "Recursive Doubling (RD) Secure"


export MV2_SECURITY_APPROACH=2005 
export MV2_INTER_ALLREDUCE_TUNING=2
echo "Reduce-scatter-Allgather (RS) Secure"


export MV2_SECURITY_APPROACH=2005 
export MV2_INTER_ALLREDUCE_TUNING=3
echo "SMP (Single-leader + Shared Memory) Secure"


export MV2_SECURITY_APPROACH=2005 
export MV2_INTER_ALLREDUCE_TUNING=6
echo "Concurrent (Multileader + Shared Memory) via Recursive Doubling (RD)"


export MV2_SECURITY_APPROACH=2005 
export MV2_INTER_ALLREDUCE_TUNING=7
echo "Concurrent (Multileader + Shared Memory) via Reduce-scatter-Allgather (RS)"


export MV2_SECURITY_APPROACH=2005 
export MV2_INTER_ALLREDUCE_TUNING=8
echo "Concurrent (Multileader + Shared Memory) via Ring"
```

#### Bcast

Set these parameters to enable MPIR_Bcast_ML_Shmem_MV2() which is responsible for encrypted multi-leader Bcast:


- MPICH

```bash
export MV2_SECURITY_APPROACH=1
echo "Naive"


export MV2_SECURITY_APPROACH=2
echo "Naive OCB"


export MV2_CONCURRENT_BCAST=1
export MV2_SECURITY_APPROACH=0
echo "Unencrypted CHS (Multileader + Shared Memory)"


export MV2_CONCURRENT_BCAST=1
export MV2_SECURITY_APPROACH=333
echo "Encrypted CHS (Multileader + Shared Memory)"
```


#### Hints

List all exported environment variables command:

```bash
printenv | grep MV2 | perl -ne 'print "export $_"'
```


Display Functions name and debuging points:

```bash
export MV2_PRINT_FUN_NAME=1
export MV2_DEBUG_INIT_FILE=1
```
