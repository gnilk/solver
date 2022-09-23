#
# CMAKE file for Solver
#
cmake_minimum_required(VERSION 3.16)

project(solve)

if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "")
    set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk)
endif()

#set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "")

set(CMAKE_SOURCE_DIR ./src)
set(CMAKE_BINARY_DIR ./bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ./lib)

#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /usr/local/lib)

set(CMAKE_BUILD_TYPE Debug)

include_directories("${PROJECT_SOURCE_DIR}")

# src
list(APPEND src src/expsolver.cpp src/tokenizer.cpp)

# tests
list(APPEND tests tests/test_expsolver.cpp)
list(APPEND tests tests/test_tokenizer.cpp)


#
# solver
#
add_executable(solve src/solver.cpp ${src})
target_include_directories(solve PRIVATE .)
set_property(TARGET solve PROPERTY CXX_STANDARD 11)

#
# Unit test runner (see: https://github.com/gnilk/testrunner )
#
if(WIN32)
    set(TRUN_CMD "c:/bin/trun.exe")
else(UNIX)
    set(TRUN_CMD "/usr/local/bin/trun")
endif()


#
# build unit test dynlib - the test runner requires tests to be in a dynamic library
#
add_library(solverlib SHARED ${src} ${tests})
set_property(TARGET solverlib PROPERTY CXX_STANDARD 11)


if (APPLE)
    target_compile_options(solve PUBLIC -Wall -Wpedantic -Wextra)

    message(STATUS ${CMAKE_OSX_ARCHITECTURES})
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(CORE_FRAMEWORK CoreFoundation)

   target_include_directories(solverlib PRIVATE /usr/local/include)
   
    list(APPEND libdep ${COCOA_FRAMEWORK} ${IOKIT_FRAMEWORK})
elseif(UNIX)
    target_compile_options(solve PUBLIC -Wall -Wpedantic -Wextra)
endif()

# link the stuff
target_link_libraries(solve ${libdep})
target_link_libraries(solverlib ${libdep})

#
# Installation handling
#
# on Linux/macOS, support "sudo make install"
if (UNIX)
    include(GNUInstallDirs)
    install(TARGETS solve RUNTIME DESTINATION bin)
endif()

#
# Custom target to execute the tests through the test runner
#
add_custom_target(
        tests ALL
        DEPENDS solverlib
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
