cmake_minimum_required(VERSION 3.16)
# --------------------- Google Test を構成 --------------------- #
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()
# --------------------- Google Test を構成 --------------------- #


# 環境変数の設定 set(変数名 値)

# CMAKE_CXX_COMPILERは任意で宣言する定数と違いc++のコンパイラを指定することが可能
#SET( CMAKE_CXX_COMPILER /usr/bin/clang++ )

# C++ バージョン指定
#add_definitions(-std=c++11 -Wall)
set(CMAKE_CXX_STANDARD 14)

set(EXE "start")
set(TEST_EXE "test")
set(SRC_ROOT "${CMAKE_SOURCE_DIR}/examples")

file(GLOB MAIN_CPP ${SRC_ROOT}/main.cpp)
file(GLOB SOURCE_CPPS ${SRC_ROOT}/*/*.cpp)
file(GLOB TEST_CPPS ./test/*.cpp)


# Build Sample code
add_executable(${EXE}
  ${MAIN_CPP}
)

#add_library(${EXE}
#  ${SOURCE_CPPS}  
#)

# Build Test code
add_executable(${TEST_EXE} ${TEST_CPPS}
  ${SOURCE_CPPS}  
) ### 注意: 通常のmain.cppを含めてはいけない

include_directories(
	${CMAKE_SOURCE_DIR}/include
)

link_directories(
    
)

# Libraries for sample code.
target_link_libraries(
	${EXE}
)
# Libraries for test code.
target_link_libraries(
	${TEST_EXE} gtest_main
)
