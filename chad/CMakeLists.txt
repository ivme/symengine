cmake_minimum_required(VERSION 3.4)
project(chad)

#GMP
include(../cmake/LibFindMacros.cmake)
libfind_include(gmpxx.h gmp)
libfind_library(gmpxx gmp)
libfind_library(gmp gmp)
set(GMP_LIBRARIES ${GMPXX_LIBRARY} ${GMP_LIBRARY})
set(GMP_INCLUDE_DIRS ${GMP_INCLUDE_DIR})
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GMP DEFAULT_MSG GMP_LIBRARIES GMP_INCLUDE_DIRS)
mark_as_advanced(GMP_INCLUDE_DIR GMPXX_LIBRARY GMP_LIBRARY)

include_directories(${GMP_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GMP_LIBRARIES})
# LIBS is later used in target_link_libraries()

#symengine
include_directories(BEFORE ..)
include_directories(BEFORE ../bin)
include_directories(BEFORE ../symengine/utilities/teuchos)

find_library(SYMEGNINE_LIBRARY NAMES symengine PATHS ${PROJECT_SOURCE_DIR}/../symengine)
add_library(symengine STATIC IMPORTED)
set_property(TARGET symengine PROPERTY IMPORTED_LOCATION ${SYMENGINE_LIBRARY})

#add executables and link to libraries
add_executable(expand1 expand1.cpp)
target_link_libraries(expand1 symengine gmp)