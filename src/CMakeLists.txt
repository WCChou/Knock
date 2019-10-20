cmake_minimum_required(VERSION 3.2)

set (local_lib /home/keiko/3rd)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${local_lib}/include/boost-1_70
    ${local_lib}/../build/yaml-cpp/include)

link_directories(
    ${local_lib}/lib
    ${local_lib}/../build/yaml-cpp-build)

set (header_files "../include/Config/Config.hpp"
                  "../include/CmdLine/Commands.hpp"
                  "../include/Util/Hash.hpp"
                  "../include/Task/Task.hpp"
                  "../include/Task/FileGenerator.hpp"
                  "../include/ProgramConfig/Config.hpp")

set (src_files CmdLine/Commands.cpp
               Util/Hash.cpp
               Task/Task.cpp
               Task/FileGenerator.cpp
               MainEntry.cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG  "-g")
add_executable(knock ${header_files} ${src_files})

find_package(Threads)
find_package(OpenSSL)

target_link_libraries(knock 
    boost_filesystem-gcc7-mt-d-x64-1_70
    boost_program_options-gcc7-mt-d-x64-1_70
    boost_date_time-gcc7-mt-d-x64-1_70
    yaml-cpp
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSSL_LIBRARIES})