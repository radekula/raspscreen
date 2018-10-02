cmake_minimum_required (VERSION 3.0)
project (raspscreen)

set (raspscreen_VERSION_MAJOR 0)
set (raspscreen_VERSION_MINOR 1)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 3.0)
    if (NOT DEFINED CMAKE_CXX_STANDARD)
        set(CMAKE_CXX_STANDARD 14)
    endif ()
else ()
    set (CMAKE_CXX_FLAGS "-std=c++14")
endif ()

set (BUILD_DIR "build")

find_package(PkgConfig REQUIRED)

pkg_check_modules(JSONCPP REQUIRED jsoncpp)

include_directories(${JSONCPP_INCLUDE_DIRS})
link_directories(${JSONCPP_LIBRARY_DIRS})
link_libraries(${JSONCPP_LIBRARIES})

include_directories (${CMAKE_CURRENT_LIST_DIR}/include)

file (GLOB RASPSCREEN_SRC "src/*.cxx")
add_executable (raspscreen ${RASPSCREEN_SRC})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(raspscreen Threads::Threads)

set (LIBRESTSRV_DIR ${CMAKE_CURRENT_LIST_DIR}/../librestsrv)
include_directories (${LIBRESTSRV_DIR}/include)
include_directories (${LIBRESTSRV_DIR}/include/client)
target_link_libraries(raspscreen ${LIBRESTSRV_DIR}/build/librestsrv.so)


