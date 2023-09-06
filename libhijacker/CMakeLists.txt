###################################################################################################
# PS5SDK - Example: pipe pirate
# Uses the read/write primitive to read and write some kernel data.
# @authors ChendoChap, Specter, Znullptr
###################################################################################################

cmake_minimum_required (VERSION 3.20)

set(basename "hijacker")
project(${basename} C CXX ASM)

# Language Standard Defaults
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_CXX_STANDARD}" STREQUAL "")
	set(CMAKE_CXX_STANDARD 20)
endif()

# Check for sub-project as part of main build or external build
if (NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	set(IS_SUBPROJECT TRUE)
else()
	set(IS_SUBPROJECT FALSE)
endif()

message("IS_SUBPROJECT: ${IS_SUBPROJECT}")

set(D_CWD "${PROJECT_SOURCE_DIR}")


# Headers
include_directories	(SYSTEM "${D_PS5SDK}")
include_directories	(SYSTEM "${D_PS5SDK}/include")

set(D_SRC ${D_CWD}/source)

file(GLOB SrcFiles ${D_SRC}/*.c ${D_SRC}/*.cpp ${D_SRC}/**/*.cpp ${D_SRC}/*.h ${D_SRC}/*.hpp ${D_SRC}/**/*.hpp ${D_SRC}/*.s)

add_library(${PROJECT_NAME} STATIC ${SrcFiles})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")

# Must build with clang
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "[Cc]lang")
	set(IS_CLANG 1)
else()
	message(FATAL_ERROR "${PROJECT_NAME} is meant to be built with clang! CompilerID: ${CMAKE_CXX_COMPILER_ID}")
endif()

# Finalize main target sources
target_compile_options(${PROJECT_NAME} PUBLIC
	$<$<COMPILE_LANGUAGE:C>:${C_DEFS} ${C_FLAGS}>
	$<$<COMPILE_LANGUAGE:CXX>:${CXX_DEFS} ${CXX_FLAGS}>
	$<$<COMPILE_LANGUAGE:ASM>:${ASM_FLAGS}>
)

message("========== build: ${PROJECT_NAME} ==========")

set(CMAKE_C_FLAGS "--target=x86_64-freebsd-pc-elf -march=znver2 -mavx2 -DPPR -DPS5 -DPS5_FW_VERSION=${V_FW} ")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_POSIX_SOURCE -D_POSIX_C_SOURCE=200112 -D__BSD_VISIBLE=1 -D__XSI_VISIBLE=500")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-builtin -nostdlib -Wall") #  -nostartfiles
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}  -O0 -march=znver2 -mavx2 -Wall -Wextra -Wmove -Wmost -Werror -pedantic -pedantic-errors -fno-exceptions")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g0 -O0")
target_include_directories(${PROJECT_NAME} PRIVATE "${D_CWD}/../include")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
