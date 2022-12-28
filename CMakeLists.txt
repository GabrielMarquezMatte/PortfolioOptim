cmake_minimum_required(VERSION 3.22.0)
project(PortfolioOptim VERSION 0.1.0 LANGUAGES CXX)
include(${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

find_package(CURL CONFIG REQUIRED)

add_subdirectory(include)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE download_data date_utils optimization CURL::libcurl)
target_include_directories(${PROJECT_NAME} PRIVATE include)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
