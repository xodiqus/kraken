cmake_minimum_required(VERSION 3.22)
project(kraken)

add_definitions(-DKRAKEN_DEBUG)
include_directories(".")

add_subdirectory(core)
add_subdirectory(parser)
add_subdirectory(compiler)
add_subdirectory(test)