cmake_minimum_required(VERSION 2.6)
project(monitor)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES})

 ## Define this to run on Xenial using old code.
 ## If using more modern compiler, comment this out to use Filesystem
add_compile_definitions(BROKEN_CXX17_IMPL)
target_compile_options(monitor PRIVATE -Wall -Wextra -Werror) # warnings as errors
