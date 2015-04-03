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
CND_PLATFORM=GNU-Linux-x86
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
	${OBJECTDIR}/Connector.o \
	${OBJECTDIR}/Database.o \
	${OBJECTDIR}/HTTPRequestBuilder.o \
	${OBJECTDIR}/Logger.o \
	${OBJECTDIR}/Main.o \
	${OBJECTDIR}/Scheduler.o \
	${OBJECTDIR}/Utility.o


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
LDLIBSOPTIONS=-L/usr/local/lib /usr/local/lib/libjsoncpp.a /usr/local/lib/libmongoc-static-1.0.a /usr/local/lib/libbson-static-1.0.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/metroapiaggregator

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/metroapiaggregator: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/metroapiaggregator ${OBJECTFILES} ${LDLIBSOPTIONS} -lpthread -lrt -lssl -lcrypto

${OBJECTDIR}/Connector.o: Connector.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/libmongoc-1.0/ -I/usr/local/include/libbson-1.0/ -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Connector.o Connector.cpp

${OBJECTDIR}/Database.o: Database.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/libmongoc-1.0/ -I/usr/local/include/libbson-1.0/ -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Database.o Database.cpp

${OBJECTDIR}/HTTPRequestBuilder.o: HTTPRequestBuilder.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/libmongoc-1.0/ -I/usr/local/include/libbson-1.0/ -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/HTTPRequestBuilder.o HTTPRequestBuilder.cpp

${OBJECTDIR}/Logger.o: Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/libmongoc-1.0/ -I/usr/local/include/libbson-1.0/ -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Logger.o Logger.cpp

${OBJECTDIR}/Main.o: Main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/libmongoc-1.0/ -I/usr/local/include/libbson-1.0/ -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Main.o Main.cpp

${OBJECTDIR}/Scheduler.o: Scheduler.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/libmongoc-1.0/ -I/usr/local/include/libbson-1.0/ -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Scheduler.o Scheduler.cpp

${OBJECTDIR}/Utility.o: Utility.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/local/include/libmongoc-1.0/ -I/usr/local/include/libbson-1.0/ -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Utility.o Utility.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/metroapiaggregator

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
