cmake_minimum_required(VERSION 3.10.0)
project(BibleTUI CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math")


include(FetchContent)
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(json)

# cURLpp + libcurl
find_package(CURL REQUIRED)

# Se cURLpp estiver instalado no sistema
find_path(CURLPP_INCLUDE_DIR curlpp/cURLpp.hpp)
find_library(CURLPP_LIBRARY NAMES curlpp)

if (NOT CURLPP_INCLUDE_DIR OR NOT CURLPP_LIBRARY)
    message(FATAL_ERROR "cURLpp não encontrado")
endif ()

include_directories(${CURL_INCLUDE_DIRS} ${CURLPP_INCLUDE_DIR})

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(bible-tui ${SOURCES})

target_link_libraries(bible-tui ${CURL_LIBRARIES} ${CURLPP_LIBRARY} nlohmann_json::nlohmann_json)

install(TARGETS bible-tui DESTINATION bin)

