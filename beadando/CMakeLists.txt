cmake_minimum_required(VERSION 3.8) # CXX_STANDARD 17

project(template LANGUAGES CXX)

if (MSVC)
  string(REGEX REPLACE "/W[0-9]" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif (MSVC)


file(MAKE_DIRECTORY data)

add_executable(gas_in_gravitational_field main.cpp)

set_target_properties(gas_in_gravitational_field PROPERTIES CXX_STANDARD 17
                                                 CXX_STANDARD_REQUIRED ON
                                                 CXX_EXTENSIONS OFF)

target_compile_options(gas_in_gravitational_field PRIVATE $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -pedantic>
                                               $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->)
if (UNIX)
find_package(X11 REQUIRED)
endif ()
target_include_directories (gas_in_gravitational_field PRIVATE
$<$<PLATFORM_ID:Linux>:${X11_INCLUDE_DIR}>)
if (UNIX)
target_link_libraries (gas_in_gravitational_field PRIVATE ${X11_LIBRARIES})
endif ()


