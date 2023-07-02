###################################################################################################
# PS5SDK - CRT build script
# @author Znullptr
###################################################################################################

cmake_minimum_required (VERSION 3.20)

set(basename "ps5sdk_crt")
project(${basename} C)

# Language Standard Defaults
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if ("${CMAKE_C_COMPILER_ID}" MATCHES "[Cc]lang")
	set(IS_CLANG 1)
else()
	message(FATAL_ERROR "${PROJECT_NAME} is meant to be built with clang! CompilerID: ${CMAKE_C_COMPILER_ID}")
endif()

if (NOT DEFINED D_PS5SDK)
	set(D_PS5SDK ".")
endif()

include_directories	(SYSTEM "${D_PS5SDK}")
include_directories	(SYSTEM "${D_PS5SDK}/include")

########## finalize main target sources ##########

file(GLOB SrcFiles *.c)

add_library(${PROJECT_NAME} STATIC ${SrcFiles})
