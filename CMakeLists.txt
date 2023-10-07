# Need CMake 3.20
cmake_minimum_required (VERSION 3.20)

# This is the name of the repository
project ("table")

# Set the correct paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set debug mode
if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif()

# Find the tuple module
if ( NOT "${HAS_TUPLE}")

    # Log
    message("[table] Adding tuple project")

    # Add the tuple directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/tuple ${CMAKE_SOURCE_DIR}/build/tuple)

    # Let any parent project know that this project has tuple
    set(HAS_TUPLE true PARENT_SCOPE)
endif()

# Add the table project
if ( NOT "${HAS_TABLE}")

    # Log
    message("[table] Adding table project")

    # Let any parent project know that it has the table project
    set(HAS_TABLE true PARENT_SCOPE)
endif()

# Add source to this project's executable.
add_executable(table_example "main.c" "table.c")
add_dependencies(table_example sync tuple)
target_include_directories(table_example PUBLIC include include/table ${CMAKE_SOURCE_DIR}/extern/sync/include/ ${CMAKE_SOURCE_DIR}/extern/tuple/include/)
target_link_libraries(table_example sync tuple)

# TODO: Add source to the tester
#add_executable (table_test "table_test.c" "table.c")
#add_dependencies(table_test sync crypto)
#target_include_directories(table_test PUBLIC include include/table ${CMAKE_SOURCE_DIR}/extern/sync/include/ ${CMAKE_SOURCE_DIR}/extern/crypto/include/)
#target_link_libraries(table_test sync crypto)

# Add source to the library
add_library(table SHARED "table.c")
add_dependencies(table sync tuple)
target_include_directories(table PUBLIC include include/table ${CMAKE_SOURCE_DIR}/extern/sync/include/ ${CMAKE_SOURCE_DIR}/extern/tuple/include/)
target_link_libraries(table sync tuple)
