#!/bin/sh

HYDRA_TOPO_DEBUG=1
export HYDRA_TOPO_DEBUG

if test -f dummy; then
    errors=0

    for topo in topo1 topo2 topo3 topo4 ; do
	export HWLOC_XMLFILE=../../impls/hydra/binding_reference/$topo.xml
	for bind_ in hwthread hwthread:2 hwthread:4 hwthread:8 core core:2 core:4 socket numa board ; do
	    for map_ in hwthread hwthread:2 hwthread:4 hwthread:8 core core:2 core:4 socket numa board ; do
		bind=`echo $bind_ | sed -e 's/:/-/g'`
		map=`echo $map_ | sed -e 's/:/-/g'`
		/home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3/Install_mpich/bin/mpiexec -bind-to $bind_ -map-by $map_ -n 16 ./dummy | sort -k2n > actual.${topo}.${bind}.${map}.out
		diff ../../impls/hydra/binding_reference/expected.${topo}.${bind}.${map}.out \
		    actual.${topo}.${bind}.${map}.out
		if test "$?" != "0" ; then
		    echo "ERROR $topo $bind $map"
		    errors=1
		else
		    rm actual.${topo}.${bind}.${map}.out
		fi
	    done
	done
    done

    if test "$errors" = "0"; then
	echo " No Errors"
	exit 0
    fi
else
    echo "run make to build required dummy executable"
    exit 1
fi
