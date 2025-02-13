#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/FrameAllocator.o \
	${OBJECTDIR}/PageTableManager.o \
	${OBJECTDIR}/Process.o \
	${OBJECTDIR}/Scheduler.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../memorysubsystemw2019/dist/Release/GNU-Linux/libmemorysubsystemw2019.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/program3

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/program3: ../memorysubsystemw2019/dist/Release/GNU-Linux/libmemorysubsystemw2019.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/program3: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/program3 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/FrameAllocator.o: FrameAllocator.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../memorysubsystemw2019 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FrameAllocator.o FrameAllocator.cpp

${OBJECTDIR}/PageTableManager.o: PageTableManager.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../memorysubsystemw2019 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/PageTableManager.o PageTableManager.cpp

${OBJECTDIR}/Process.o: Process.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../memorysubsystemw2019 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Process.o Process.cpp

${OBJECTDIR}/Scheduler.o: Scheduler.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../memorysubsystemw2019 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Scheduler.o Scheduler.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../memorysubsystemw2019 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../memorysubsystemw2019 && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:
	cd ../memorysubsystemw2019 && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
