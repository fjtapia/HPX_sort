include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
  file_generator
  LinuxGCC/bench/file_generator.cpp
)

#------------------------------------------------------------------------------
# includes and options for the benchmark
#------------------------------------------------------------------------------
set(BENCHMARK_LIBRARIES ${BENCHMARK_LIBRARIES} ${Boost_LIBRARIES})

if (SORT_HAS_TBB)
  include_directories(${TBB_INCLUDE_DIR})
  add_definitions(-DSORT_HAS_TBB)
  set(BENCHMARK_LIBRARIES ${BENCHMARK_LIBRARIES} ${TBB_LIBRARIES})
endif()

if (SORT_HAS_HPX)
  include_directories(${HPX_INCLUDE_DIRS})
  add_definitions(-DSORT_HAS_HPX)
  set(BENCHMARK_LIBRARIES ${BENCHMARK_LIBRARIES} ${TBB_LIBRARIES})
endif()

#------------------------------------------------------------------------------
# Benchmark
#------------------------------------------------------------------------------
add_executable(
  fast_benchmark
  LinuxGCC/bench/fast_benchmark.cpp
)
target_link_libraries(fast_benchmark ${BENCHMARK_LIBRARIES})

if (SORT_HAS_HPX)
  hpx_setup_target(fast_benchmark)
  target_link_libraries(fast_benchmark)
endif()

