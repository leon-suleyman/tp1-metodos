cmake_minimum_required(VERSION 3.0.0)
project(tp1 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(tp1 src/main.cpp)
#add_executable(testHeuristica3 test/heuristicas/testHeuristica3.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
