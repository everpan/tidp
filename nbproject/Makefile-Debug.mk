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
	${OBJECTDIR}/src/TL_Cgi.o \
	${OBJECTDIR}/src/TL_Client.o \
	${OBJECTDIR}/src/TL_Command.o \
	${OBJECTDIR}/src/TL_Common.o \
	${OBJECTDIR}/src/TL_Crontab.o \
	${OBJECTDIR}/src/TL_Datetime.o \
	${OBJECTDIR}/src/TL_Elapsed.o \
	${OBJECTDIR}/src/TL_Exp.o \
	${OBJECTDIR}/src/TL_Http.o \
	${OBJECTDIR}/src/TL_HttpRequest.o \
	${OBJECTDIR}/src/TL_HttpResponse.o \
	${OBJECTDIR}/src/TL_IniFile.o \
	${OBJECTDIR}/src/TL_Logger.o \
	${OBJECTDIR}/src/TL_MD5.o \
	${OBJECTDIR}/src/TL_MemQueue.o \
	${OBJECTDIR}/src/TL_MemString.o \
	${OBJECTDIR}/src/TL_Mmap.o \
	${OBJECTDIR}/src/TL_Mysql.o \
	${OBJECTDIR}/src/TL_MysqlPool.o \
	${OBJECTDIR}/src/TL_Option.o \
	${OBJECTDIR}/src/TL_Redis.o \
	${OBJECTDIR}/src/TL_Rlog.o \
	${OBJECTDIR}/src/TL_StringBuffer.o \
	${OBJECTDIR}/src/TL_Thread.o \
	${OBJECTDIR}/src/TL_ThreadLock.o \
	${OBJECTDIR}/src/TL_TimerProvide.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtidp.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtidp.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtidp.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/src/TL_Cgi.o: src/TL_Cgi.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_Cgi.o src/TL_Cgi.cpp

${OBJECTDIR}/src/TL_Client.o: src/TL_Client.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_Client.o src/TL_Client.cpp

${OBJECTDIR}/src/TL_Command.o: src/TL_Command.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_Command.o src/TL_Command.cpp

${OBJECTDIR}/src/TL_Common.o: src/TL_Common.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_Common.o src/TL_Common.cpp

${OBJECTDIR}/src/TL_Crontab.o: src/TL_Crontab.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_Crontab.o src/TL_Crontab.cpp

${OBJECTDIR}/src/TL_Datetime.o: src/TL_Datetime.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_Datetime.o src/TL_Datetime.cpp

${OBJECTDIR}/src/TL_Elapsed.o: src/TL_Elapsed.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_Elapsed.o src/TL_Elapsed.cpp

${OBJECTDIR}/src/TL_Exp.o: src/TL_Exp.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_Exp.o src/TL_Exp.cpp

${OBJECTDIR}/src/TL_Http.o: src/TL_Http.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_Http.o src/TL_Http.cpp

${OBJECTDIR}/src/TL_HttpRequest.o: src/TL_HttpRequest.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_HttpRequest.o src/TL_HttpRequest.cpp

${OBJECTDIR}/src/TL_HttpResponse.o: src/TL_HttpResponse.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_HttpResponse.o src/TL_HttpResponse.cpp

${OBJECTDIR}/src/TL_IniFile.o: src/TL_IniFile.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_IniFile.o src/TL_IniFile.cpp

${OBJECTDIR}/src/TL_Logger.o: src/TL_Logger.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_Logger.o src/TL_Logger.cpp

${OBJECTDIR}/src/TL_MD5.o: src/TL_MD5.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_MD5.o src/TL_MD5.cpp

${OBJECTDIR}/src/TL_MemQueue.o: src/TL_MemQueue.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_MemQueue.o src/TL_MemQueue.cpp

${OBJECTDIR}/src/TL_MemString.o: src/TL_MemString.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_MemString.o src/TL_MemString.cpp

${OBJECTDIR}/src/TL_Mmap.o: src/TL_Mmap.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_Mmap.o src/TL_Mmap.cpp

${OBJECTDIR}/src/TL_Mysql.o: src/TL_Mysql.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_Mysql.o src/TL_Mysql.cpp

${OBJECTDIR}/src/TL_MysqlPool.o: src/TL_MysqlPool.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_MysqlPool.o src/TL_MysqlPool.cpp

${OBJECTDIR}/src/TL_Option.o: src/TL_Option.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_Option.o src/TL_Option.cpp

${OBJECTDIR}/src/TL_Redis.o: src/TL_Redis.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_Redis.o src/TL_Redis.cpp

${OBJECTDIR}/src/TL_Rlog.o: src/TL_Rlog.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_Rlog.o src/TL_Rlog.cpp

${OBJECTDIR}/src/TL_StringBuffer.o: src/TL_StringBuffer.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_StringBuffer.o src/TL_StringBuffer.cpp

${OBJECTDIR}/src/TL_Thread.o: src/TL_Thread.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_Thread.o src/TL_Thread.cpp

${OBJECTDIR}/src/TL_ThreadLock.o: src/TL_ThreadLock.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_ThreadLock.o src/TL_ThreadLock.cpp

${OBJECTDIR}/src/TL_TimerProvide.o: src/TL_TimerProvide.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I./include -I/usr/local/mysql/include -std=c++11 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TL_TimerProvide.o src/TL_TimerProvide.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
