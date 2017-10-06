cmake_minimum_required(VERSION 3.7.2)
project(scc)

set(CMAKE_CXX_STANDARD 11)

#if(WIN32)
#    set(BOOST_INCLUDEDIR C:/mingw/include)
#    set( BOOST_LIBRARYDIR C:/mingw/lib)
#endif()

#message(${BOOST_INCLUDEDIR})

#find_package(Boost COMPONENTS system filesystem REQUIRED)

#if(Boost_FOUND)
#    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
#    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
#    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
#
#    include_directories(${Boost_INCLUDE_DIRS})
#endif()

set(SOURCE_FILES main.cpp headers/StringUtils.h headers/FileUtils.h)

add_executable(scc ${SOURCE_FILES})
target_link_libraries(scc ${Boost_LIBRARIES})