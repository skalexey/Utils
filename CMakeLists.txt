# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project (utils C CXX)

if(MSVC)
	# fix __cplusplus macro for MSVC
	add_compile_options(
		"/Zc:__cplusplus"
		"/std:c++latest"
	)
else()
	set(CMAKE_CXX_STANDARD 20)
endif()

option(INHERIT_PARENT_SRC "Adds all parent's source files" OFF)

## Set and add includes
# private includes
include_directories("include/utils")

# external includes
if (NOT "${INCLUDES}" STREQUAL "")
	message("Add extra includes: '${INCLUDES}'")
endif()

# global includes
set(INCLUDES ${INCLUDES} "${PROJECT_SOURCE_DIR}/include")

set(utils_INCLUDES ${utils_INCLUDES} ${CUSTOM_INCLUDES} ${INCLUDES})
set(utils_INCLUDES ${utils_INCLUDES} PARENT_SCOPE)

include_directories(${utils_INCLUDES})

message("utils_INCLUDES: ${utils_INCLUDES}")

# Set sources
file(GLOB SRC CONFIGURE_DEPENDS "src/*")
file(GLOB_RECURSE HEADERS "include/*.h")

# Inherit parent sources logic
set(PARENT_SRC ${PARENT_SRC} ${SRC})
set(PARENT_SRC ${PARENT_SRC} PARENT_SCOPE)

if (INHERIT_PARENT_SRC)
	set(SRC ${PARENT_SRC})
	message("'Project' project PARENT_SRC: ${PARENT_SRC}")
endif()
# End of Inherit parent sources logic

message("utils SRC: ${SRC}, HEADERS: ${HEADERS}")

add_library(utils ${SRC} ${HEADERS})
set_target_properties(utils PROPERTIES LINKER_LANGUAGE CXX)