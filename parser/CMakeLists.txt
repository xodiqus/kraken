project(kraken-parser LANGUAGES C)

file(GLOB SRS "src/*.c")
file(GLOB HDS "*.h")

add_library(${PROJECT_NAME} ${SRS} ${HDS})

target_include_directories(${PROJECT_NAME} PRIVATE ".")