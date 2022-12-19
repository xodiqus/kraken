project(kraken-compiler)

file(GLOB SRS "./*.c")

add_executable(${PROJECT_NAME} ${SRS})
target_link_libraries(${PROJECT_NAME} kraken-parser)