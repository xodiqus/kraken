project(kraken-tests LANGUAGES CXX)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

file(GLOB PARSER_SRS "parser/*.cpp")
file(GLOB PARSER_HDR "parser/*.hpp")

add_executable(${PROJECT_NAME} main.cpp ${PARSER_SRS} ${PARSER_HDR})

target_link_libraries(${PROJECT_NAME} Boost::unit_test_framework kraken-parser)
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS} "../")
