include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
  file_generator
  Linux/file_generator.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/src/hpx//parallel/sort/tools)
#------------------------------------------------------------------------------
# includes and options for the benchmark
#------------------------------------------------------------------------------
set(BENCHMARK_LIBRARIES ${BENCHMARK_LIBRARIES} ${Boost_LIBRARIES})

if (SORT_HAS_TBB)
  include_directories(${TBB_INCLUDE_DIRS})
  add_definitions(-DSORT_HAS_TBB)
  set(BENCHMARK_LIBRARIES ${BENCHMARK_LIBRARIES} ${TBB_LIBRARIES})
endif()

if (SORT_HAS_HPX)
  include_directories(${HPX_INCLUDE_DIRS})
  add_definitions(-DSORT_HAS_HPX)
  set(BENCHMARK_LIBRARIES ${BENCHMARK_LIBRARIES} ${HPX_LIBRARIES})
endif()

if (SORT_HAS_GNU)
  add_definitions(-DSORT_HAS_GCC_PARALLEL)
endif()

message("Benchmark libs are ${BENCHMARK_LIBRARIES}")

#------------------------------------------------------------------------------
# Benchmark
#------------------------------------------------------------------------------
add_executable(
  benchmark
  Linux/benchmark.cpp
)
target_link_libraries(benchmark ${BENCHMARK_LIBRARIES})

if (SORT_HAS_HPX)
  hpx_setup_target(benchmark)
  target_link_libraries(benchmark)
endif()

