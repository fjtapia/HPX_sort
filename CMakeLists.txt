cmake_minimum_required(VERSION 2.8)
project(sort_parallel)

# Setup cmake policies.
foreach(p
    CMP0012 CMP0013 CMP0014
    CMP0022 CMP0025 CMP0043
    CMP0053
  )
  if(POLICY ${p})
    cmake_policy(SET ${p} NEW)
  endif()
endforeach()

#--------------------------------------------------
# Setup Output directories
#--------------------------------------------------
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Executables."
)
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Libraries"
)
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all static libraries."
)

#------------------------------------------------------------------------------
# Boost
#------------------------------------------------------------------------------
set(Boost_ADDITIONAL_VERSIONS
    ${Boost_ADDITIONAL_VERSIONS}
    "1.59.0" "1.59"
    "1.58.0" "1.58"
    "1.57.0" "1.57")

find_package(Boost
  1.55
  REQUIRED
  COMPONENTS
    regex date_time serialization chrono atomic
    program_options filesystem log log_setup system thread
)

#------------------------------------------------------------------------------
# CMake modules 
#------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#--------------------------------------------------
# Find HPX
#--------------------------------------------------
find_package(HPX)
if(HPX_FOUND)
  set(SORT_HAS_HPX 1)
endif()

#------------------------------------------------------------------------------
# TBB
#------------------------------------------------------------------------------
option(SORT_WITH_TBB "Enable TBB sort" 1)
if (SORT_WITH_TBB)
    find_package(TBB)
    if (TBB_FOUND)
      message("TBB supported")
      set(SORT_HAS_TBB 1)
    else()
    endif()
endif()

#--------------------------------------------------
# GCC parallel or Windows PPL
#--------------------------------------------------
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(SORT_HAS_CLANG)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  option(SORT_WITH_GNU_PARALLEL "Enable GNU Parallel sort" 1)
  if (SORT_WITH_GNU_PARALLEL)
      find_package(OpenMP)
      set(SORT_HAS_GNU 1)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    #  target_compile_definitions(${PROJECT_NAME} PRIVATE "_GLIBCXX_PARALLEL")
    #  target_compile_options(${PROJECT_NAME} PRIVATE "-floop-parallelize-all" "-ftree-parallelize-loops=4")
   endif() 
endif()

#------------------------------------------------------------------------------
# include dirs
#------------------------------------------------------------------------------
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS}
)

if (WIN32)
  add_definitions(/bigobj)
endif()
#------------------------------------------------------------------------------
# tests
#------------------------------------------------------------------------------
#add_subdirectory(test)
#add_subdirectory(example)
add_subdirectory(benchmark)

