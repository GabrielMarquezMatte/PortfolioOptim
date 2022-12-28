add_library(optimization STATIC optimization.cpp optimization.hpp)
target_include_directories(optimization PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(NOT WIN32)
    find_package(Threads REQUIRED)
    target_link_libraries(optimization Threads::Threads)
endif()