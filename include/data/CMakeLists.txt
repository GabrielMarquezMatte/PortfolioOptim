add_library(date_utils STATIC date_utils.cpp)
add_library(download_data STATIC download_data.cpp)
target_link_libraries(download_data PRIVATE CURL::libcurl date_utils)
target_include_directories(download_data PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})