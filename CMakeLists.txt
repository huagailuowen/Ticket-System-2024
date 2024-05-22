cmake_minimum_required(VERSION 3.12)
project(TicketSystem)

# add_compile_options("-fsanitize=address")
set(CMAKE_CXX_STANDARD 20)
# Enable debug mode and disable optimization
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall  -Wextra ")
# Enable optimization level 2
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
# Add the include directory
include_directories(include)

# Add the source files
file(GLOB SOURCES "src/code/*.cpp")
# Add the executable
add_executable(TicketSystem ${SOURCES})