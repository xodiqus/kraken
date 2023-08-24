project(kraken-core)

file(GLOB SRC "src/*.c")

add_library(${PROJECT_NAME} ${SRC})

# target_link_libraries(${PROJECT_NAME} kraken-parser)
target_include_directories(${PROJECT_NAME} PRIVATE ".")