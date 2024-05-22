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
add_executable(code ${SOURCES})
# cmake_minimum_required(VERSION 3.12)
# project(TicketSystem)

# # include_directories(./third_party)
# set(CMAKE_CXX_STANDARD 20)
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -Ofast")

# add_executable(code ${src_dir} src/code/main.cpp)


# set(test_prefix "${PROJECT_SOURCE_DIR}/data")
# file(GLOB tests RELATIVE "${test_prefix}" data/*)

# foreach (test-name IN ITEMS ${tests})
#     if (NOT IS_DIRECTORY "${test_prefix}/${test-name}")
#         continue()
#     endif ()
#     add_test(NAME "${test-name}"
#             COMMAND bash ../run_test.sh "${test_prefix}" "${test-name}" "$<TARGET_FILE:code>"
#             WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/run
#             )
# endforeach ()

# enable_testing()
