cmake_minimum_required(VERSION 3.16)
project(tidp)

set(CMAKE_CXX_STANDARD 14)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(.)
include_directories(include)
include_directories(include/net)
include_directories(olap)

add_library(tidp
        olap/TL_Cube.cpp
        olap/TL_Cube.h
        olap/TL_Dimension.cpp
        olap/TL_Dimension.h
        olap/TL_KeyLiner.cpp
        olap/TL_KeyLiner.h
        olap/TL_Metric.cpp
        olap/TL_Metric.h
        olap/TL_MultiDimension.cpp
        olap/TL_MultiDimension.h
        src/net/TL_Execute.cpp
        src/net/TL_ExecuteFactory.cpp
        src/net/TL_ExecuteThread.cpp
        src/net/TL_ExecuteThreadFactory.cpp
        src/net/TL_Packet.cpp
        src/net/TL_PacketManger.cpp
        src/net/TL_Server.cpp
        src/net/TL_Session.cpp
        src/net/TL_SessionManger.cpp
        src/net/TL_Socket.cpp
        src/TL_Cgi.cpp
        src/TL_Client.cpp
        src/TL_Command.cpp
        src/TL_Common.cpp
        src/TL_Crontab.cpp
        src/TL_Datetime.cpp
        src/TL_Elapsed.cpp
        src/TL_Exp.cpp
        src/TL_Http.cpp
        src/TL_HttpRequest.cpp
        src/TL_HttpResponse.cpp
        src/TL_IniFile.cpp
        src/TL_Logger.cpp
        src/TL_MD5.cpp
        src/TL_MemQueue.cpp
        src/TL_MemString.cpp
        src/TL_Mmap.cpp
        src/TL_Mysql.cpp
        src/TL_MysqlPool.cpp
        src/TL_Option.cpp
        src/TL_Redis.cpp
        src/TL_Rlog.cpp
        src/TL_StringBuffer.cpp
        src/TL_Thread.cpp
        src/TL_ThreadLock.cpp
        src/TL_TimerProvide.cpp)
#add_link_options(tidp pthread)

## install
target_include_directories(tidp PUBLIC ${CMAKE_SOURCE_DIR}/include)
#set_target_properties(tidp PROPERTIES PUBLIC_HEADER ${CMAKE_SOURCE_DIR}/include)
install(TARGETS tidp tidp)
# install header files
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include DESTINATION tidp)