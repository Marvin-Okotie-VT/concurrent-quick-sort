cmake_minimum_required(VERSION 3.5)
project(CQSORT CXX)

# EDIT
# add any files you create related to the concurrent quick sort
# excluding unit tests
set(cqsort_src
  cqsort.hpp
  cqsort.tpp
  )

# EDIT
# add any files you create related to unit testing here
set(test_src
  catch.hpp
  unittests.cpp   # add your additional test cases here
  test_cqsort.cpp # do not modify this file
)

# EDIT
# add any files you create related to the benchmark program here
set(cqbench_src
  ${cqsort_src}
  cqbench.cpp
  )

# ------------------------------------------------
# You should not need to edit any files below here
# ------------------------------------------------

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
find_package( Threads )

# create the cqbench executable
add_executable(cqbench ${cqbench_src})
target_link_libraries(cqbench ${CMAKE_THREAD_LIBS_INIT})
set_property(TARGET cqbench PROPERTY CXX_STANDARD 11)

include_directories(${CMAKE_BINARY_DIR})

add_executable(unittests ${cqsort_src} ${test_src})
target_link_libraries(unittests ${CMAKE_THREAD_LIBS_INIT} )
set_property(TARGET unittests PROPERTY CXX_STANDARD 11)

enable_testing()
add_test(unittests unittests)

if(UNIX AND WERROR)
  message("-- Enabling strict compilation mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

# On Linux, using GCC, to enable coverage on tests -DCOVERAGE=TRUE
if(UNIX AND NOT APPLE AND CMAKE_COMPILER_IS_GNUCXX AND COVERAGE)
  message("Enabling Test Coverage")
  SET(GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
  set_target_properties(unittests PROPERTIES COMPILE_FLAGS ${GCC_COVERAGE_COMPILE_FLAGS} )
  target_link_libraries(unittests gcov)
  add_custom_target(coverage
    COMMAND ${CMAKE_COMMAND} -E env "ROOT=${CMAKE_CURRENT_SOURCE_DIR}"
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/coverage.sh)
endif()

# On Linux, using GCC, to enable coverage on tests -DMEMORY=TRUE
if(UNIX AND NOT APPLE AND CMAKE_COMPILER_IS_GNUCXX AND MEMORY)
  add_custom_target(memtest
    COMMAND valgrind ${CMAKE_BINARY_DIR}/unittests)
endif()

# enable clang-tidy with -DTIDY=TRUE
if(UNIX AND NOT APPLE AND CMAKE_COMPILER_IS_GNUCXX AND TIDY)
  add_custom_target(tidy
    COMMAND ${CMAKE_COMMAND} -E env "source=${CMAKE_CURRENT_SOURCE_DIR}" env "build=${CMAKE_CURRENT_BINARY_DIR}"
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/tidy.sh
    )
endif()
