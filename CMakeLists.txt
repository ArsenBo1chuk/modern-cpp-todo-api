cmake_minimum_required(VERSION 3.15)
project(TodoServer CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CROW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CROW_BUILD_TESTS OFF CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(
    sqlite3
    GIT_REPOSITORY https://github.com/azadkuh/sqlite-amalgamation.git
    GIT_TAG master
)
FetchContent_MakeAvailable(sqlite3)

FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-28-0
)
FetchContent_MakeAvailable(asio)

set(ASIO_INCLUDE_DIR "${asio_SOURCE_DIR}/asio/include" CACHE PATH "" FORCE)

FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG v1.2.0
)
FetchContent_MakeAvailable(Crow)

add_executable(todo_server main.cpp)

target_include_directories(todo_server PRIVATE ${sqlite3_SOURCE_DIR} ${ASIO_INCLUDE_DIR})
target_link_libraries(todo_server PRIVATE Crow::Crow SQLite3 mswsock ws2_32)