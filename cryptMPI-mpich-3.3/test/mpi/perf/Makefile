# Makefile.in generated by automake 1.15.1 from Makefile.am.
# perf/Makefile.  Generated from Makefile.in by configure.

# Copyright (C) 1994-2017 Free Software Foundation, Inc.

# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

MAKE=make

am__is_gnu_make = { \
  if test -z '$(MAKELEVEL)'; then \
    false; \
  elif test -n '$(MAKE_HOST)'; then \
    true; \
  elif test -n '$(MAKE_VERSION)' && test -n '$(CURDIR)'; then \
    true; \
  else \
    false; \
  fi; \
}
am__make_running_with_option = \
  case $${target_option-} in \
      ?) ;; \
      *) echo "am__make_running_with_option: internal error: invalid" \
              "target option '$${target_option-}' specified" >&2; \
         exit 1;; \
  esac; \
  has_opt=no; \
  sane_makeflags=$$MAKEFLAGS; \
  if $(am__is_gnu_make); then \
    sane_makeflags=$$MFLAGS; \
  else \
    case $$MAKEFLAGS in \
      *\\[\ \	]*) \
        bs=\\; \
        sane_makeflags=`printf '%s\n' "$$MAKEFLAGS" \
          | sed "s/$$bs$$bs[$$bs $$bs	]*//g"`;; \
    esac; \
  fi; \
  skip_next=no; \
  strip_trailopt () \
  { \
    flg=`printf '%s\n' "$$flg" | sed "s/$$1.*$$//"`; \
  }; \
  for flg in $$sane_makeflags; do \
    test $$skip_next = yes && { skip_next=no; continue; }; \
    case $$flg in \
      *=*|--*) continue;; \
        -*I) strip_trailopt 'I'; skip_next=yes;; \
      -*I?*) strip_trailopt 'I';; \
        -*O) strip_trailopt 'O'; skip_next=yes;; \
      -*O?*) strip_trailopt 'O';; \
        -*l) strip_trailopt 'l'; skip_next=yes;; \
      -*l?*) strip_trailopt 'l';; \
      -[dEDm]) skip_next=yes;; \
      -[JT]) skip_next=yes;; \
    esac; \
    case $$flg in \
      *$$target_option*) has_opt=yes; break;; \
    esac; \
  done; \
  test $$has_opt = yes
am__make_dryrun = (target_option=n; $(am__make_running_with_option))
am__make_keepgoing = (target_option=k; $(am__make_running_with_option))
pkgdatadir = $(datadir)/mpich-testsuite
pkgincludedir = $(includedir)/mpich-testsuite
pkglibdir = $(libdir)/mpich-testsuite
pkglibexecdir = $(libexecdir)/mpich-testsuite
am__cd = CDPATH="$${ZSH_VERSION+.}$(PATH_SEPARATOR)" && cd
install_sh_DATA = $(install_sh) -c -m 644
install_sh_PROGRAM = $(install_sh) -c
install_sh_SCRIPT = $(install_sh) -c
INSTALL_HEADER = $(INSTALL_DATA)
transform = $(program_transform_name)
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_triplet = x86_64-pc-linux-gnu
host_triplet = x86_64-pc-linux-gnu
noinst_PROGRAMS = transp_datatype$(EXEEXT) non_zero_root$(EXEEXT) \
	sendrecvl$(EXEEXT) twovec$(EXEEXT) dtpack$(EXEEXT) \
	allredtrace$(EXEEXT) commcreatep$(EXEEXT) allredtrace$(EXEEXT) \
	commcreatep$(EXEEXT) timer$(EXEEXT) manyrma$(EXEEXT) \
	nestvec$(EXEEXT) nestvec2$(EXEEXT) indexperf$(EXEEXT)
subdir = perf
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
am__aclocal_m4_deps = $(top_srcdir)/confdb/aclocal_cache.m4 \
	$(top_srcdir)/confdb/aclocal_cc.m4 \
	$(top_srcdir)/confdb/aclocal_cxx.m4 \
	$(top_srcdir)/confdb/aclocal_f77.m4 \
	$(top_srcdir)/confdb/aclocal_fc.m4 \
	$(top_srcdir)/confdb/aclocal_make.m4 \
	$(top_srcdir)/confdb/aclocal_runlog.m4 \
	$(top_srcdir)/confdb/aclocal_util.m4 $(top_srcdir)/version.m4 \
	$(top_srcdir)/configure.ac
am__configure_deps = $(am__aclocal_m4_deps) $(CONFIGURE_DEPENDENCIES) \
	$(ACLOCAL_M4)
DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)
mkinstalldirs = $(install_sh) -d
CONFIG_HEADER = $(top_builddir)/include/mpitestconf.h
CONFIG_CLEAN_FILES =
CONFIG_CLEAN_VPATH_FILES =
PROGRAMS = $(noinst_PROGRAMS)
allredtrace_SOURCES = allredtrace.c
allredtrace_OBJECTS = allredtrace.$(OBJEXT)
allredtrace_LDADD = $(LDADD)
allredtrace_DEPENDENCIES = $(top_builddir)/util/mtest.o \
	$(top_builddir)/dtpools/src/.libs/libdtpools.a
commcreatep_SOURCES = commcreatep.c
commcreatep_OBJECTS = commcreatep.$(OBJEXT)
commcreatep_LDADD = $(LDADD)
commcreatep_DEPENDENCIES = $(top_builddir)/util/mtest.o \
	$(top_builddir)/dtpools/src/.libs/libdtpools.a
dtpack_SOURCES = dtpack.c
dtpack_OBJECTS = dtpack-dtpack.$(OBJEXT)
dtpack_LDADD = $(LDADD)
dtpack_DEPENDENCIES = $(top_builddir)/util/mtest.o \
	$(top_builddir)/dtpools/src/.libs/libdtpools.a
dtpack_LINK = $(CCLD) $(dtpack_CFLAGS) $(CFLAGS) $(AM_LDFLAGS) \
	$(LDFLAGS) -o $@
indexperf_SOURCES = indexperf.c
indexperf_OBJECTS = indexperf-indexperf.$(OBJEXT)
indexperf_LDADD = $(LDADD)
indexperf_DEPENDENCIES = $(top_builddir)/util/mtest.o \
	$(top_builddir)/dtpools/src/.libs/libdtpools.a
indexperf_LINK = $(CCLD) $(indexperf_CFLAGS) $(CFLAGS) $(AM_LDFLAGS) \
	$(LDFLAGS) -o $@
manyrma_SOURCES = manyrma.c
manyrma_OBJECTS = manyrma.$(OBJEXT)
manyrma_LDADD = $(LDADD)
manyrma_DEPENDENCIES = $(top_builddir)/util/mtest.o \
	$(top_builddir)/dtpools/src/.libs/libdtpools.a
nestvec_SOURCES = nestvec.c
nestvec_OBJECTS = nestvec-nestvec.$(OBJEXT)
nestvec_LDADD = $(LDADD)
nestvec_DEPENDENCIES = $(top_builddir)/util/mtest.o \
	$(top_builddir)/dtpools/src/.libs/libdtpools.a
nestvec_LINK = $(CCLD) $(nestvec_CFLAGS) $(CFLAGS) $(AM_LDFLAGS) \
	$(LDFLAGS) -o $@
nestvec2_SOURCES = nestvec2.c
nestvec2_OBJECTS = nestvec2-nestvec2.$(OBJEXT)
nestvec2_LDADD = $(LDADD)
nestvec2_DEPENDENCIES = $(top_builddir)/util/mtest.o \
	$(top_builddir)/dtpools/src/.libs/libdtpools.a
nestvec2_LINK = $(CCLD) $(nestvec2_CFLAGS) $(CFLAGS) $(AM_LDFLAGS) \
	$(LDFLAGS) -o $@
non_zero_root_SOURCES = non_zero_root.c
non_zero_root_OBJECTS = non_zero_root.$(OBJEXT)
non_zero_root_LDADD = $(LDADD)
non_zero_root_DEPENDENCIES = $(top_builddir)/util/mtest.o \
	$(top_builddir)/dtpools/src/.libs/libdtpools.a
sendrecvl_SOURCES = sendrecvl.c
sendrecvl_OBJECTS = sendrecvl.$(OBJEXT)
sendrecvl_LDADD = $(LDADD)
sendrecvl_DEPENDENCIES = $(top_builddir)/util/mtest.o \
	$(top_builddir)/dtpools/src/.libs/libdtpools.a
timer_SOURCES = timer.c
timer_OBJECTS = timer.$(OBJEXT)
timer_LDADD = $(LDADD)
timer_DEPENDENCIES = $(top_builddir)/util/mtest.o \
	$(top_builddir)/dtpools/src/.libs/libdtpools.a
transp_datatype_SOURCES = transp_datatype.c
transp_datatype_OBJECTS = transp_datatype.$(OBJEXT)
transp_datatype_LDADD = $(LDADD)
transp_datatype_DEPENDENCIES = $(top_builddir)/util/mtest.o \
	$(top_builddir)/dtpools/src/.libs/libdtpools.a
twovec_SOURCES = twovec.c
twovec_OBJECTS = twovec.$(OBJEXT)
twovec_DEPENDENCIES = $(top_builddir)/util/mtest.o
AM_V_P = $(am__v_P_$(V))
am__v_P_ = $(am__v_P_$(AM_DEFAULT_VERBOSITY))
am__v_P_0 = false
am__v_P_1 = :
AM_V_GEN = $(am__v_GEN_$(V))
am__v_GEN_ = $(am__v_GEN_$(AM_DEFAULT_VERBOSITY))
am__v_GEN_0 = @echo "  GEN     " $@;
am__v_GEN_1 = 
AM_V_at = $(am__v_at_$(V))
am__v_at_ = $(am__v_at_$(AM_DEFAULT_VERBOSITY))
am__v_at_0 = @
am__v_at_1 = 
DEFAULT_INCLUDES = -I. -I$(top_builddir)/include
depcomp = $(SHELL) $(top_srcdir)/confdb/depcomp
am__depfiles_maybe = depfiles
am__mv = mv -f
AM_V_lt = $(am__v_lt_$(V))
am__v_lt_ = $(am__v_lt_$(AM_DEFAULT_VERBOSITY))
am__v_lt_0 = --silent
am__v_lt_1 = 
COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
AM_V_CC = $(am__v_CC_$(V))
am__v_CC_ = $(am__v_CC_$(AM_DEFAULT_VERBOSITY))
am__v_CC_0 = @echo "  CC      " $@;
am__v_CC_1 = 
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -o $@
AM_V_CCLD = $(am__v_CCLD_$(V))
am__v_CCLD_ = $(am__v_CCLD_$(AM_DEFAULT_VERBOSITY))
am__v_CCLD_0 = @echo "  CCLD    " $@;
am__v_CCLD_1 = 
SOURCES = allredtrace.c commcreatep.c dtpack.c indexperf.c manyrma.c \
	nestvec.c nestvec2.c non_zero_root.c sendrecvl.c timer.c \
	transp_datatype.c twovec.c
DIST_SOURCES = allredtrace.c commcreatep.c dtpack.c indexperf.c \
	manyrma.c nestvec.c nestvec2.c non_zero_root.c sendrecvl.c \
	timer.c transp_datatype.c twovec.c
am__can_run_installinfo = \
  case $$AM_UPDATE_INFO_DIR in \
    n|no|NO) false;; \
    *) (install-info --version) >/dev/null 2>&1;; \
  esac
am__tagged_files = $(HEADERS) $(SOURCES) $(TAGS_FILES) $(LISP)
# Read a list of newline-separated strings from the standard input,
# and print each of them once, without duplicates.  Input order is
# *not* preserved.
am__uniquify_input = $(AWK) '\
  BEGIN { nonempty = 0; } \
  { items[$$0] = 1; nonempty = 1; } \
  END { if (nonempty) { for (i in items) print i; }; } \
'
# Make sure the list of sources is unique.  This is necessary because,
# e.g., the same source file might be shared among _SOURCES variables
# for different programs/libraries.
am__define_uniq_tagged_files = \
  list='$(am__tagged_files)'; \
  unique=`for i in $$list; do \
    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
  done | $(am__uniquify_input)`
ETAGS = etags
CTAGS = ctags
am__DIST_COMMON = $(srcdir)/Makefile.in $(top_srcdir)/Makefile.mtest \
	$(top_srcdir)/confdb/depcomp README
DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
VPATH = VPATH=.:${srcdir}
ACLOCAL = ${SHELL} /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3/test/mpi/confdb/missing aclocal-1.15
ALLOCMEMF = allocmemf 1
ALLOCMEMFC = allocmemf90 1
AMTAR = $${TAR-tar}
AM_DEFAULT_VERBOSITY = 0
AR = ar
AUTOCONF = ${SHELL} /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3/test/mpi/confdb/missing autoconf
AUTOHEADER = ${SHELL} /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3/test/mpi/confdb/missing autoheader
AUTOMAKE = ${SHELL} /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3/test/mpi/confdb/missing automake-1.15
AWK = gawk
CC = /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3/Install_mpich/bin/mpicc
CCDEPMODE = depmode=gcc3
CFLAGS = 
CPP = gcc -std=gnu99 -E
CPPFLAGS = 
CXX = /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3/Install_mpich/bin/mpicxx
CXXCPP = 
CXXDEPMODE = depmode=gcc3
CXXFLAGS = 
CYGPATH_W = echo
DEFS = -DHAVE_CONFIG_H
DEPDIR = .deps
DTP_SWITCH = ON
ECHO_C = 
ECHO_N = -n
ECHO_T = 
EGREP = /usr/bin/grep -E
EXEEXT = 
F03SPAWNARGTEST = 
F77 = /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3/Install_mpich/bin/mpif77
F77SPAWNARGTEST = 
F77_GETARG_LIBS = 
F77_MPI_ADDRESS = integer*8
F77_MPI_OFFSET = integer*8
F77_NAME_MANGLE = F77_NAME_LOWER_USCORE
FC = /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3/Install_mpich/bin/mpifort
FCFLAGS = -fcray-pointer
FCFLAGS_f90 = 
FCMODEXT = mod
FCMODINCFLAG = -I
FCMODOUTFLAG = -J
FC_GETARG_LIBS = 
FC_WORK_FILES_ARG = 
FFLAGS = -fcray-pointer
FGREP = /usr/bin/grep -F
FLIBS =  -L/usr/lib/gcc/x86_64-redhat-linux/4.8.5 -L/usr/lib/gcc/x86_64-redhat-linux/4.8.5/../../../../lib64 -L/lib/../lib64 -L/usr/lib/../lib64 -L/usr/lib/gcc/x86_64-redhat-linux/4.8.5/../../.. -lpthread -lgfortran -lm -lquadmath
FROM_MPICH = yes
GREP = /usr/bin/grep
INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
INSTALL_SCRIPT = ${INSTALL}
INSTALL_STRIP_PROGRAM = $(install_sh) -c -s
LDFLAGS = 
LIBOBJS = 
LIBS = 
LTLIBOBJS = 
MAINT = 
MAKE = make
MAKEINFO = ${SHELL} /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3/test/mpi/confdb/missing makeinfo
MKDIR_P = mkdir -p
MPICC = 
MPICH_ENABLE_CXX = yes
MPICH_ENABLE_F77 = yes
MPICH_ENABLE_FC = yes
MPICH_THREAD_LEVEL = MPI_THREAD_MULTIPLE
MPICXX = 
MPIEXEC = /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3/Install_mpich/bin/mpiexec
MPIF77 = 
MPIFC = 
MPILIBLOC = 
MPILIBNAME = mpi
MPI_HAS_MPIX = yes
MPI_IS_STRICT = false
MPI_NO_RMA = 
MPI_NO_SPAWN = 
MPI_SIZEOF_AINT = 8
MPI_SIZEOF_OFFSET = 8
MPI_SOURCE = 
MPI_SUBVERSION = 1
MPI_VERSION = 3
OBJEXT = o
PACKAGE = mpich-testsuite
PACKAGE_BUGREPORT = discuss@mpich.org
PACKAGE_NAME = mpich-testsuite
PACKAGE_STRING = mpich-testsuite 3.3rc1
PACKAGE_TARNAME = mpich-testsuite
PACKAGE_URL = http://www.mpich.org/
PACKAGE_VERSION = 3.3rc1
PATH_SEPARATOR = :
PERL = /usr/bin/perl
RANLIB = ranlib
RUN_XFAIL = false
SET_CFLAGS = CFLAGS=
SET_MAKE = MAKE=make
SHELL = /bin/sh
STRIP = 
VERSION = 3.3rc1
WRAPPER_CFLAGS = 
WRAPPER_CPPFLAGS = 
WRAPPER_CXXFLAGS = 
WRAPPER_FCFLAGS = 
WRAPPER_FFLAGS = 
WRAPPER_LDFLAGS = 
WRAPPER_LIBS = -lm   -lpthread  -lrt 
abs_builddir = /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3/test/mpi/perf
abs_srcdir = /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3/test/mpi/perf
abs_top_builddir = /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3/test/mpi
abs_top_srcdir = /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3/test/mpi
ac_ct_AR = 
ac_ct_CC = 
ac_ct_CXX = 
ac_ct_F77 = 
ac_ct_FC = 
am__include = include
am__leading_dot = .
am__quote = 
am__tar = $${TAR-tar} chof - "$$tardir"
am__untar = $${TAR-tar} xf -
bindir = ${exec_prefix}/bin
build = x86_64-pc-linux-gnu
build_alias = 
build_cpu = x86_64
build_os = linux-gnu
build_vendor = pc
builddir = .
ckpointdir = #ckpoint
comm_overlap = 
cxxdir = cxx
datadir = ${datarootdir}
datarootdir = ${prefix}/share
docdir = ${datarootdir}/doc/${PACKAGE_TARNAME}
dvidir = ${docdir}
errordir = errors
exec_prefix = ${prefix}
f08dir = #
f77dir = f77
f90dir = f90
faultsdir = #
ftdir = ft
host = x86_64-pc-linux-gnu
host_alias = 
host_cpu = x86_64
host_os = linux-gnu
host_vendor = pc
htmldir = ${docdir}
impldir = mpich
includedir = ${prefix}/include
infodir = ${datarootdir}/info
install_sh = ${SHELL} /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3/test/mpi/confdb/install-sh
iodir = io
largetest = #
libdir = ${exec_prefix}/lib
libexecdir = ${exec_prefix}/libexec
localedir = ${datarootdir}/locale
localstatedir = ${prefix}/var
mandir = ${datarootdir}/man
master_top_builddir = /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3
master_top_srcdir = /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3
mkdir_p = $(MKDIR_P)
mpich_top_srcdir = /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3
mpix = 
namepub_tests = 
nocxxdistgraph = #
nslib = 
oldincludedir = /usr/include
otherlangs =  f77 f90 cxx
pdfdir = ${docdir}
perfdir = perf
prefix = /home/gavahi/ocb_CryptMPI_2022/MPICH/cryptMPI-mpich-3.3/Install_mpich
program_transform_name = s,x,x,
psdir = ${docdir}
rmadir = rma
sbindir = ${exec_prefix}/sbin
sharedstatedir = ${prefix}/com
socklib = 
spawndir = spawn
srcdir = .
subdirs =  dtpools
sysconfdir = ${prefix}/etc
target_alias = 
threadlib = -lpthread
threadsdir = threads
top_build_prefix = ../
top_builddir = ..
top_srcdir = ..

# AM_CPPFLAGS are used for C++ code as well

# Add libdtpools support
AM_CPPFLAGS = -I$(top_builddir)/include -I$(top_srcdir)/include \
	-I$(top_builddir)/dtpoos/include \
	-I$(top_srcdir)/dtpools/include
LDADD = $(top_builddir)/util/mtest.o \
	$(top_builddir)/dtpools/src/.libs/libdtpools.a
CLEANFILES = $(SUMMARY_BASENAME).xml $(SUMMARY_BASENAME).tap $(SUMMARY_BASENAME).junit.xml
EXTRA_DIST = testlist

# Force all tests to be compiled with optimization 
AM_CFLAGS = -O
# While the AM_CFLAGS should be sufficient, to make this Makefile more
# robust, indicate that these specific programs *must* be compiled with
# optimization
dtpack_CFLAGS = -O
nestvec_CFLAGS = -O
nestvec2_CFLAGS = -O
indexperf_CFLAGS = -O
twovec_LDADD = -lm $(top_builddir)/util/mtest.o
all: all-am

.SUFFIXES:
.SUFFIXES: .c .o .obj
$(srcdir)/Makefile.in:  $(srcdir)/Makefile.am $(top_srcdir)/Makefile.mtest $(am__configure_deps)
	@for dep in $?; do \
	  case '$(am__configure_deps)' in \
	    *$$dep*) \
	      ( cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh ) \
	        && { if test -f $@; then exit 0; else break; fi; }; \
	      exit 1;; \
	  esac; \
	done; \
	echo ' cd $(top_srcdir) && $(AUTOMAKE) --foreign perf/Makefile'; \
	$(am__cd) $(top_srcdir) && \
	  $(AUTOMAKE) --foreign perf/Makefile
Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status
	@case '$?' in \
	  *config.status*) \
	    cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh;; \
	  *) \
	    echo ' cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe)'; \
	    cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe);; \
	esac;
$(top_srcdir)/Makefile.mtest $(am__empty):

$(top_builddir)/config.status: $(top_srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh

$(top_srcdir)/configure:  $(am__configure_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(ACLOCAL_M4):  $(am__aclocal_m4_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(am__aclocal_m4_deps):

clean-noinstPROGRAMS:
	-test -z "$(noinst_PROGRAMS)" || rm -f $(noinst_PROGRAMS)

allredtrace$(EXEEXT): $(allredtrace_OBJECTS) $(allredtrace_DEPENDENCIES) $(EXTRA_allredtrace_DEPENDENCIES) 
	@rm -f allredtrace$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(allredtrace_OBJECTS) $(allredtrace_LDADD) $(LIBS)

commcreatep$(EXEEXT): $(commcreatep_OBJECTS) $(commcreatep_DEPENDENCIES) $(EXTRA_commcreatep_DEPENDENCIES) 
	@rm -f commcreatep$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(commcreatep_OBJECTS) $(commcreatep_LDADD) $(LIBS)

dtpack$(EXEEXT): $(dtpack_OBJECTS) $(dtpack_DEPENDENCIES) $(EXTRA_dtpack_DEPENDENCIES) 
	@rm -f dtpack$(EXEEXT)
	$(AM_V_CCLD)$(dtpack_LINK) $(dtpack_OBJECTS) $(dtpack_LDADD) $(LIBS)

indexperf$(EXEEXT): $(indexperf_OBJECTS) $(indexperf_DEPENDENCIES) $(EXTRA_indexperf_DEPENDENCIES) 
	@rm -f indexperf$(EXEEXT)
	$(AM_V_CCLD)$(indexperf_LINK) $(indexperf_OBJECTS) $(indexperf_LDADD) $(LIBS)

manyrma$(EXEEXT): $(manyrma_OBJECTS) $(manyrma_DEPENDENCIES) $(EXTRA_manyrma_DEPENDENCIES) 
	@rm -f manyrma$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(manyrma_OBJECTS) $(manyrma_LDADD) $(LIBS)

nestvec$(EXEEXT): $(nestvec_OBJECTS) $(nestvec_DEPENDENCIES) $(EXTRA_nestvec_DEPENDENCIES) 
	@rm -f nestvec$(EXEEXT)
	$(AM_V_CCLD)$(nestvec_LINK) $(nestvec_OBJECTS) $(nestvec_LDADD) $(LIBS)

nestvec2$(EXEEXT): $(nestvec2_OBJECTS) $(nestvec2_DEPENDENCIES) $(EXTRA_nestvec2_DEPENDENCIES) 
	@rm -f nestvec2$(EXEEXT)
	$(AM_V_CCLD)$(nestvec2_LINK) $(nestvec2_OBJECTS) $(nestvec2_LDADD) $(LIBS)

non_zero_root$(EXEEXT): $(non_zero_root_OBJECTS) $(non_zero_root_DEPENDENCIES) $(EXTRA_non_zero_root_DEPENDENCIES) 
	@rm -f non_zero_root$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(non_zero_root_OBJECTS) $(non_zero_root_LDADD) $(LIBS)

sendrecvl$(EXEEXT): $(sendrecvl_OBJECTS) $(sendrecvl_DEPENDENCIES) $(EXTRA_sendrecvl_DEPENDENCIES) 
	@rm -f sendrecvl$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(sendrecvl_OBJECTS) $(sendrecvl_LDADD) $(LIBS)

timer$(EXEEXT): $(timer_OBJECTS) $(timer_DEPENDENCIES) $(EXTRA_timer_DEPENDENCIES) 
	@rm -f timer$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(timer_OBJECTS) $(timer_LDADD) $(LIBS)

transp_datatype$(EXEEXT): $(transp_datatype_OBJECTS) $(transp_datatype_DEPENDENCIES) $(EXTRA_transp_datatype_DEPENDENCIES) 
	@rm -f transp_datatype$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(transp_datatype_OBJECTS) $(transp_datatype_LDADD) $(LIBS)

twovec$(EXEEXT): $(twovec_OBJECTS) $(twovec_DEPENDENCIES) $(EXTRA_twovec_DEPENDENCIES) 
	@rm -f twovec$(EXEEXT)
	$(AM_V_CCLD)$(LINK) $(twovec_OBJECTS) $(twovec_LDADD) $(LIBS)

mostlyclean-compile:
	-rm -f *.$(OBJEXT)

distclean-compile:
	-rm -f *.tab.c

include ./$(DEPDIR)/allredtrace.Po
include ./$(DEPDIR)/commcreatep.Po
include ./$(DEPDIR)/dtpack-dtpack.Po
include ./$(DEPDIR)/indexperf-indexperf.Po
include ./$(DEPDIR)/manyrma.Po
include ./$(DEPDIR)/nestvec-nestvec.Po
include ./$(DEPDIR)/nestvec2-nestvec2.Po
include ./$(DEPDIR)/non_zero_root.Po
include ./$(DEPDIR)/sendrecvl.Po
include ./$(DEPDIR)/timer.Po
include ./$(DEPDIR)/transp_datatype.Po
include ./$(DEPDIR)/twovec.Po

.c.o:
	$(AM_V_CC)depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.o$$||'`;\
	$(COMPILE) -MT $@ -MD -MP -MF $$depbase.Tpo -c -o $@ $< &&\
	$(am__mv) $$depbase.Tpo $$depbase.Po
#	$(AM_V_CC)source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(COMPILE) -c -o $@ $<

.c.obj:
	$(AM_V_CC)depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.obj$$||'`;\
	$(COMPILE) -MT $@ -MD -MP -MF $$depbase.Tpo -c -o $@ `$(CYGPATH_W) '$<'` &&\
	$(am__mv) $$depbase.Tpo $$depbase.Po
#	$(AM_V_CC)source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(COMPILE) -c -o $@ `$(CYGPATH_W) '$<'`

dtpack-dtpack.o: dtpack.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(dtpack_CFLAGS) $(CFLAGS) -MT dtpack-dtpack.o -MD -MP -MF $(DEPDIR)/dtpack-dtpack.Tpo -c -o dtpack-dtpack.o `test -f 'dtpack.c' || echo '$(srcdir)/'`dtpack.c
	$(AM_V_at)$(am__mv) $(DEPDIR)/dtpack-dtpack.Tpo $(DEPDIR)/dtpack-dtpack.Po
#	$(AM_V_CC)source='dtpack.c' object='dtpack-dtpack.o' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(dtpack_CFLAGS) $(CFLAGS) -c -o dtpack-dtpack.o `test -f 'dtpack.c' || echo '$(srcdir)/'`dtpack.c

dtpack-dtpack.obj: dtpack.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(dtpack_CFLAGS) $(CFLAGS) -MT dtpack-dtpack.obj -MD -MP -MF $(DEPDIR)/dtpack-dtpack.Tpo -c -o dtpack-dtpack.obj `if test -f 'dtpack.c'; then $(CYGPATH_W) 'dtpack.c'; else $(CYGPATH_W) '$(srcdir)/dtpack.c'; fi`
	$(AM_V_at)$(am__mv) $(DEPDIR)/dtpack-dtpack.Tpo $(DEPDIR)/dtpack-dtpack.Po
#	$(AM_V_CC)source='dtpack.c' object='dtpack-dtpack.obj' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(dtpack_CFLAGS) $(CFLAGS) -c -o dtpack-dtpack.obj `if test -f 'dtpack.c'; then $(CYGPATH_W) 'dtpack.c'; else $(CYGPATH_W) '$(srcdir)/dtpack.c'; fi`

indexperf-indexperf.o: indexperf.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(indexperf_CFLAGS) $(CFLAGS) -MT indexperf-indexperf.o -MD -MP -MF $(DEPDIR)/indexperf-indexperf.Tpo -c -o indexperf-indexperf.o `test -f 'indexperf.c' || echo '$(srcdir)/'`indexperf.c
	$(AM_V_at)$(am__mv) $(DEPDIR)/indexperf-indexperf.Tpo $(DEPDIR)/indexperf-indexperf.Po
#	$(AM_V_CC)source='indexperf.c' object='indexperf-indexperf.o' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(indexperf_CFLAGS) $(CFLAGS) -c -o indexperf-indexperf.o `test -f 'indexperf.c' || echo '$(srcdir)/'`indexperf.c

indexperf-indexperf.obj: indexperf.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(indexperf_CFLAGS) $(CFLAGS) -MT indexperf-indexperf.obj -MD -MP -MF $(DEPDIR)/indexperf-indexperf.Tpo -c -o indexperf-indexperf.obj `if test -f 'indexperf.c'; then $(CYGPATH_W) 'indexperf.c'; else $(CYGPATH_W) '$(srcdir)/indexperf.c'; fi`
	$(AM_V_at)$(am__mv) $(DEPDIR)/indexperf-indexperf.Tpo $(DEPDIR)/indexperf-indexperf.Po
#	$(AM_V_CC)source='indexperf.c' object='indexperf-indexperf.obj' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(indexperf_CFLAGS) $(CFLAGS) -c -o indexperf-indexperf.obj `if test -f 'indexperf.c'; then $(CYGPATH_W) 'indexperf.c'; else $(CYGPATH_W) '$(srcdir)/indexperf.c'; fi`

nestvec-nestvec.o: nestvec.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(nestvec_CFLAGS) $(CFLAGS) -MT nestvec-nestvec.o -MD -MP -MF $(DEPDIR)/nestvec-nestvec.Tpo -c -o nestvec-nestvec.o `test -f 'nestvec.c' || echo '$(srcdir)/'`nestvec.c
	$(AM_V_at)$(am__mv) $(DEPDIR)/nestvec-nestvec.Tpo $(DEPDIR)/nestvec-nestvec.Po
#	$(AM_V_CC)source='nestvec.c' object='nestvec-nestvec.o' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(nestvec_CFLAGS) $(CFLAGS) -c -o nestvec-nestvec.o `test -f 'nestvec.c' || echo '$(srcdir)/'`nestvec.c

nestvec-nestvec.obj: nestvec.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(nestvec_CFLAGS) $(CFLAGS) -MT nestvec-nestvec.obj -MD -MP -MF $(DEPDIR)/nestvec-nestvec.Tpo -c -o nestvec-nestvec.obj `if test -f 'nestvec.c'; then $(CYGPATH_W) 'nestvec.c'; else $(CYGPATH_W) '$(srcdir)/nestvec.c'; fi`
	$(AM_V_at)$(am__mv) $(DEPDIR)/nestvec-nestvec.Tpo $(DEPDIR)/nestvec-nestvec.Po
#	$(AM_V_CC)source='nestvec.c' object='nestvec-nestvec.obj' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(nestvec_CFLAGS) $(CFLAGS) -c -o nestvec-nestvec.obj `if test -f 'nestvec.c'; then $(CYGPATH_W) 'nestvec.c'; else $(CYGPATH_W) '$(srcdir)/nestvec.c'; fi`

nestvec2-nestvec2.o: nestvec2.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(nestvec2_CFLAGS) $(CFLAGS) -MT nestvec2-nestvec2.o -MD -MP -MF $(DEPDIR)/nestvec2-nestvec2.Tpo -c -o nestvec2-nestvec2.o `test -f 'nestvec2.c' || echo '$(srcdir)/'`nestvec2.c
	$(AM_V_at)$(am__mv) $(DEPDIR)/nestvec2-nestvec2.Tpo $(DEPDIR)/nestvec2-nestvec2.Po
#	$(AM_V_CC)source='nestvec2.c' object='nestvec2-nestvec2.o' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(nestvec2_CFLAGS) $(CFLAGS) -c -o nestvec2-nestvec2.o `test -f 'nestvec2.c' || echo '$(srcdir)/'`nestvec2.c

nestvec2-nestvec2.obj: nestvec2.c
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(nestvec2_CFLAGS) $(CFLAGS) -MT nestvec2-nestvec2.obj -MD -MP -MF $(DEPDIR)/nestvec2-nestvec2.Tpo -c -o nestvec2-nestvec2.obj `if test -f 'nestvec2.c'; then $(CYGPATH_W) 'nestvec2.c'; else $(CYGPATH_W) '$(srcdir)/nestvec2.c'; fi`
	$(AM_V_at)$(am__mv) $(DEPDIR)/nestvec2-nestvec2.Tpo $(DEPDIR)/nestvec2-nestvec2.Po
#	$(AM_V_CC)source='nestvec2.c' object='nestvec2-nestvec2.obj' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(nestvec2_CFLAGS) $(CFLAGS) -c -o nestvec2-nestvec2.obj `if test -f 'nestvec2.c'; then $(CYGPATH_W) 'nestvec2.c'; else $(CYGPATH_W) '$(srcdir)/nestvec2.c'; fi`

ID: $(am__tagged_files)
	$(am__define_uniq_tagged_files); mkid -fID $$unique
tags: tags-am
TAGS: tags

tags-am: $(TAGS_DEPENDENCIES) $(am__tagged_files)
	set x; \
	here=`pwd`; \
	$(am__define_uniq_tagged_files); \
	shift; \
	if test -z "$(ETAGS_ARGS)$$*$$unique"; then :; else \
	  test -n "$$unique" || unique=$$empty_fix; \
	  if test $$# -gt 0; then \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      "$$@" $$unique; \
	  else \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      $$unique; \
	  fi; \
	fi
ctags: ctags-am

CTAGS: ctags
ctags-am: $(TAGS_DEPENDENCIES) $(am__tagged_files)
	$(am__define_uniq_tagged_files); \
	test -z "$(CTAGS_ARGS)$$unique" \
	  || $(CTAGS) $(CTAGSFLAGS) $(AM_CTAGSFLAGS) $(CTAGS_ARGS) \
	     $$unique

GTAGS:
	here=`$(am__cd) $(top_builddir) && pwd` \
	  && $(am__cd) $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) "$$here"
cscopelist: cscopelist-am

cscopelist-am: $(am__tagged_files)
	list='$(am__tagged_files)'; \
	case "$(srcdir)" in \
	  [\\/]* | ?:[\\/]*) sdir="$(srcdir)" ;; \
	  *) sdir=$(subdir)/$(srcdir) ;; \
	esac; \
	for i in $$list; do \
	  if test -f "$$i"; then \
	    echo "$(subdir)/$$i"; \
	  else \
	    echo "$$sdir/$$i"; \
	  fi; \
	done >> $(top_builddir)/cscope.files

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH tags

distdir: $(DISTFILES)
	@srcdirstrip=`echo "$(srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	topsrcdirstrip=`echo "$(top_srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	list='$(DISTFILES)'; \
	  dist_files=`for file in $$list; do echo $$file; done | \
	  sed -e "s|^$$srcdirstrip/||;t" \
	      -e "s|^$$topsrcdirstrip/|$(top_builddir)/|;t"`; \
	case $$dist_files in \
	  */*) $(MKDIR_P) `echo "$$dist_files" | \
			   sed '/\//!d;s|^|$(distdir)/|;s,/[^/]*$$,,' | \
			   sort -u` ;; \
	esac; \
	for file in $$dist_files; do \
	  if test -f $$file || test -d $$file; then d=.; else d=$(srcdir); fi; \
	  if test -d $$d/$$file; then \
	    dir=`echo "/$$file" | sed -e 's,/[^/]*$$,,'`; \
	    if test -d "$(distdir)/$$file"; then \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    if test -d $(srcdir)/$$file && test $$d != $(srcdir); then \
	      cp -fpR $(srcdir)/$$file "$(distdir)$$dir" || exit 1; \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    cp -fpR $$d/$$file "$(distdir)$$dir" || exit 1; \
	  else \
	    test -f "$(distdir)/$$file" \
	    || cp -p $$d/$$file "$(distdir)/$$file" \
	    || exit 1; \
	  fi; \
	done
check-am: all-am
check: check-am
all-am: Makefile $(PROGRAMS)
installdirs:
install: install-am
install-exec: install-exec-am
install-data: install-data-am
uninstall: uninstall-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-am
install-strip:
	if test -z '$(STRIP)'; then \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	      install; \
	else \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	    "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'" install; \
	fi
mostlyclean-generic:

clean-generic:
	-test -z "$(CLEANFILES)" || rm -f $(CLEANFILES)

distclean-generic:
	-test -z "$(CONFIG_CLEAN_FILES)" || rm -f $(CONFIG_CLEAN_FILES)
	-test . = "$(srcdir)" || test -z "$(CONFIG_CLEAN_VPATH_FILES)" || rm -f $(CONFIG_CLEAN_VPATH_FILES)

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-am

clean-am: clean-generic clean-noinstPROGRAMS mostlyclean-am

distclean: distclean-am
	-rm -rf ./$(DEPDIR)
	-rm -f Makefile
distclean-am: clean-am distclean-compile distclean-generic \
	distclean-tags

dvi: dvi-am

dvi-am:

html: html-am

html-am:

info: info-am

info-am:

install-data-am:

install-dvi: install-dvi-am

install-dvi-am:

install-exec-am:

install-html: install-html-am

install-html-am:

install-info: install-info-am

install-info-am:

install-man:

install-pdf: install-pdf-am

install-pdf-am:

install-ps: install-ps-am

install-ps-am:

installcheck-am:

maintainer-clean: maintainer-clean-am
	-rm -rf ./$(DEPDIR)
	-rm -f Makefile
maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-am

mostlyclean-am: mostlyclean-compile mostlyclean-generic

pdf: pdf-am

pdf-am:

ps: ps-am

ps-am:

uninstall-am:

.MAKE: install-am install-strip

.PHONY: CTAGS GTAGS TAGS all all-am check check-am clean clean-generic \
	clean-noinstPROGRAMS cscopelist-am ctags ctags-am distclean \
	distclean-compile distclean-generic distclean-tags distdir dvi \
	dvi-am html html-am info info-am install install-am \
	install-data install-data-am install-dvi install-dvi-am \
	install-exec install-exec-am install-html install-html-am \
	install-info install-info-am install-man install-pdf \
	install-pdf-am install-ps install-ps-am install-strip \
	installcheck installcheck-am installdirs maintainer-clean \
	maintainer-clean-generic mostlyclean mostlyclean-compile \
	mostlyclean-generic pdf pdf-am ps ps-am tags tags-am uninstall \
	uninstall-am

.PRECIOUS: Makefile


$(top_builddir)/util/mtest.$(OBJEXT): $(top_srcdir)/util/mtest.c
	(cd $(top_builddir)/util && $(MAKE) mtest.$(OBJEXT))

$(top_builddir)/dtpools/src/.libs/libdtpools.a: $(top_srcdir)/dtpools/src/*.c
	(cd $(top_builddir)/dtpools && $(MAKE))

SUMMARY_BASENAME ?= summary

testing:
	$(top_builddir)/runtests -srcdir=$(srcdir) -tests=testlist \
		-mpiexec="${MPIEXEC}" -xmlfile=$(SUMMARY_BASENAME).xml \
		-tapfile=$(SUMMARY_BASENAME).tap -junitfile=$(SUMMARY_BASENAME).junit.xml

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
