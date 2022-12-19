project(kraken-parser LANGUAGES C)

file(GLOB SRS "*.c")
file(GLOB HDS "*.h")

add_library(${PROJECT_NAME} ${SRS} ${HDS})
