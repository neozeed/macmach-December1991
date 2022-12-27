#
# Mach Operating System
# Copyright (c) 1991,1990,1989 Carnegie Mellon University
# All Rights Reserved.
# 
# Permission to use, copy, modify and distribute this software and its
# documentation is hereby granted, provided that both the copyright
# notice and this permission notice appear in all copies of the
# software, derivative works or modified versions, and any portions
# thereof, and that both notices appear in supporting documentation.
# 
# CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS" 
# CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
# ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
# 
# Carnegie Mellon requests users of this software to return to
# 
#  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
#  School of Computer Science
#  Carnegie Mellon University
#  Pittsburgh PA 15213-3890
# 
# any improvements or extensions that they make and grant Carnegie Mellon
# the rights to redistribute these changes.
#
#
# HISTORY
# $Log:	Makefile,v $
# Revision 2.12  91/09/12  16:35:25  bohman
# 	mac2 support.
# 	[91/09/11  14:06:43  bohman]
# 
# Revision 2.11  91/07/09  23:14:07  danner
# 	Luna88k support.
# 	[91/06/26            danner]
# 
# Revision 2.10  91/05/14  15:20:33  mrt
# 	Correcting copyright
# 
# Revision 2.9  91/03/16  14:41:19  rpd
# 	Added REG_EXP to the Makeconf created in the build directory.
# 	[91/03/14            rpd]
# 
# Revision 2.8  91/02/05  17:00:20  mrt
# 	Changed to new copyright
# 	[91/01/28  14:43:17  mrt]
# 
# Revision 2.7  90/11/25  17:48:12  jsb
# 	Added i860 support.
# 	[90/11/25  16:16:19  jsb]
# 
# Revision 2.6  90/11/05  14:25:13  rpd
# 	SUN->SUN3.
# 	[90/10/31            rwd]
# 	Change MACHINE to TARGET_MACHINE.
# 	[90/10/29            rwd]
# 
# Revision 2.5  90/06/02  14:45:08  rpd
# 	Added rules to build mig.
# 	[90/03/26  21:27:35  rpd]
# 
#
# Condensed history:
#	Added rules for lint (dbg).
# 	Added code to save CONFIG in vers.config file (gm0w).
#	Added sun4 support (jjc).
#	Use conf/make.template and conf/mkconfig.csh (gm0w).
#	Added multimax support (dlb).
#	Added pmax and i386 support (af, rvb).
#	Added rules for pmake (rvb).
#	Much trial & error hacking (mwyoung, rpd).
#	Created (mja).
#

#
#	Targets of interest:
#		doconf		Rebuild the configuration file
#		config		Reconfigure
#		kernel		Rebuild just the kernel
#		pmake		Rebuild just the kernel using pmake
#		buildconf	Build all of the "standard" configurations
#		lint		Lint the kernel
#

VAX_cpu=vax
IBMRT_cpu=ca
SUN3_cpu=sun3
MMAX_cpu=mmax
PMAX_cpu=mips
I386_cpu=i386
SUN4_cpu=sun4
AT386_cpu=i386
I860_cpu=i860
LUNA88K_cpu=luna88k
MAC2_cpu=mac2
cpu=$(${TARGET_MACHINE}_cpu)

VAX_OUTPUT=Makefile
SUN3_OUTPUT=Makefile
IBMRT_OUTPUT=Makefile
MMAX_OUTPUT=Makefile
PMAX_OUTPUT=Makefile
I386_OUTPUT=Makefile
SUN4_OUTPUT=Makefile
AT386_OUTPUT=Makefile
I860_OUTPUT=Makefile
LUNA88K_OUTPUT=Makefile
MAC2_OUTPUT=Makefile

MASTER_DIR=conf
MASTER=${MASTER_DIR}/MASTER
MASTER_CPU=${MASTER}.${cpu}
MASTER_LOCAL=${MASTER_DIR}/MASTER.local
MASTER_CPU_LOCAL=${MASTER}.${cpu}.local
CONFIG_INPUT=${CONFIG}/${CONFIG}
CONFIG_OUTPUT=${CONFIG}/$(${TARGET_MACHINE}_OUTPUT)

MAKE_ARGS="REG_EXP=${REG_EXP}"
PMAKE_ARGS= -k -P6 "P=&" all

SOURCE_DIR=..

${CONFIG}.all:	${CONFIG}/make \
		${CONFIG}/Makeconf \
		${CONFIG}.doconf \
		${CONFIG}.config \
		${CONFIG}.mig \
		${CONFIG}.make

${CONFIG}.make kernel:	${CONFIG}/make ${CONFIG}/Makeconf
	@echo "[ making ${CONFIG} ]"
	@cd ${CONFIG} && USE_SOURCE_DIR=1 ./make ${MAKE_ARGS}

pmake:	${CONFIG}/make ${CONFIG}/Makeconf
	@echo "[ pmaking ${CONFIG} ]"
	@cd ${CONFIG} && USE_SOURCE_DIR=1 ./pmake ${PMAKE_ARGS}

${CONFIG}.lint lint:	${CONFIG}/make ${CONFIG}/Makeconf
	@echo "[ linting ${CONFIG} ]"
	@cd ${CONFIG} && USE_SOURCE_DIR=1 ./make lint ${MAKE_ARGS}

${CONFIG}.doconf doconf: ${CONFIG_INPUT}

${CONFIG}.config config::	${CONFIG}/make \
				${CONFIG}/Makeconf \
				${CONFIG}/Makefile.top-level \
				./bin/config \
				${CONFIG_INPUT} \
				conf/files \
				conf/files.${cpu} \
				conf/Makefile.template \
				conf/Makefile.${cpu}
	@cd ${CONFIG} && \
         ./make CONFIG=${CONFIG} \
		CONFIG_SUFFIX=${CONFIG_SUFFIX} \
		CONFIG_FLAGS=${CONFIG_FLAGS} \
		-f Makefile.top-level \
		make.config

${CONFIG}.mig mig: ./bin/mig ./lib/migcom

./bin ./lib ../bin ./src ./src/config ./src/mig: 
	mkdir $@

./bin/config: ./bin ./src ./src/config ALWAYS
	@echo "[ generating $@ ]"
	cd src/config && ${MAKE} ../../bin/config 

./bin/mig: ./bin ./src ./src/mig ALWAYS
	@echo "[ generating $@ ]"
	cd src/mig && ${MAKE} DDIR=../../bin mig.install

./lib/migcom: ./lib ./src ./src/mig ALWAYS
	@echo "[ generating $@ ]"
	cd src/mig && ${MAKE} DDIR=../../lib migcom.install

./bin/buildconf: ./bin ./src
	@echo "[ generating $@ ]"
	cd src && ${MAKE} ../bin/buildconf

${CONFIG}/Makeconf:
	@echo "[ generating $@ ]"
	@echo "REG_EXP=.*" > $@

${CONFIG}/Makefile.top-level:	Makefile
	@echo "[ generating $@ ]"
	@rm -f $@; cp -p Makefile $@

${CONFIG}/make:	Makefile conf/make.template
	@echo "[ generating $@ ]"
	@-if [ -d ${CONFIG} ]; then true; else mkdir ${CONFIG}; fi
	@sed -e 's;@OBJECTDIR@;${OBJECTDIR};g' -e 's;@VPATH@;${VPATH};g' \
	 < conf/make.template > $@.tmp; \
	 chmod +x $@.tmp; \
         mv $@.tmp $@; \
	 rm -f ${CONFIG}/pmake; \
	 ln $@ ${CONFIG}/pmake

${CONFIG_INPUT}:	${MASTER} ${MASTER_CPU} \
			${MASTER_LOCAL} ${MASTER_CPU_LOCAL} \
			Makefile conf/mkconfig.csh
	@echo "[ generating $@ from ${MASTER_DIR}/MASTER{,.${cpu}}{,.local} ]"
	@-if [ -d ${CONFIG} ]; then true; else mkdir ${CONFIG}; fi
	@csh -f conf/mkconfig.csh ${CONFIG} ${MASTER} ${MASTER_CPU} ${MASTER_LOCAL} ${MASTER_CPU_LOCAL} >${CONFIG_INPUT}.tmp
	@-if [ -s ${CONFIG_INPUT}.tmp ]; then \
	    if [ -f ${CONFIG_INPUT} ]; then \
		diff ${CONFIG_INPUT} ${CONFIG_INPUT}.tmp; \
		mv ${CONFIG_INPUT} ${CONFIG_INPUT}.old; \
	    fi; \
	    mv ${CONFIG_INPUT}.tmp ${CONFIG_INPUT}; \
	else \
	    rm -f ${CONFIG_INPUT}.tmp; \
	fi

make.config:	../${CONFIG_OUTPUT}${CONFIG_SUFFIX}

../${CONFIG_OUTPUT}${CONFIG_SUFFIX}:	\
		../${CONFIG_INPUT} \
		$(SOURCE_DIR)/conf/files \
		$(SOURCE_DIR)/conf/files.${cpu} \
		$(SOURCE_DIR)/conf/Makefile.template \
		$(SOURCE_DIR)/conf/Makefile.${cpu} \
		./Makefile.top-level
	@echo "[ configuring in ${CONFIG} ]"
	../bin/config -c $(SOURCE_DIR)/conf ${CONFIG_FLAGS} ${CONFIG}
	@echo ${CONFIG} >vers.config
	@-if [ -d sys ] ; then true; else mkdir sys; fi

buildconf: ./bin/buildconf conf/buildconf.${cpu}
	@-if [ "_${BUILDCONF}_" = "__" ]; then \
	      argv=`sed <conf/buildconf.${cpu} -e "/^#/d"`; \
	  else \
	      argv=${BUILDCONF}; \
	  fi; \
	exec ./bin/buildconf ${OPTIONS} $$argv

ALWAYS:

-include ${cpu}/Makefile.top-level
