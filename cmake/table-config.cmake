# TABLE CMake configuration file:
# This file is meant to be placed in a cmake subfolder of TABLE-devel-2.x.y-VC

cmake_minimum_required(VERSION 3.0)

include(FeatureSummary)
set_package_properties(TABLE PROPERTIES
    URL "https://g10.app/status/#abstract_data_ii"
    DESCRIPTION "table ADT"
)

# Copied from `configure_package_config_file`
macro(set_and_check _var _file)
    set(${_var} "${_file}")
    if(NOT EXISTS "${_file}")
        message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
    endif()
endmacro()

# Copied from `configure_package_config_file`
macro(check_required_components _NAME)
    foreach(comp ${${_NAME}_FIND_COMPONENTS})
        if(NOT ${_NAME}_${comp}_FOUND)
            if(${_NAME}_FIND_REQUIRED_${comp})
                set(${_NAME}_FOUND FALSE)
            endif()
        endif()
    endforeach()
endmacro()

set(TABLE_FOUND TRUE)

# For compatibility with autotools TABLE-config.cmake, provide TABLE_* variables.

set_and_check(TABLE_PREFIX       "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(TABLE_EXEC_PREFIX  "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(TABLE_INCLUDE_DIR  "${TABLE_PREFIX}/include")
set(TABLE_INCLUDE_DIRS           "${TABLE_INCLUDE_DIR}")
set_and_check(TABLE_BINDIR       "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/")
set_and_check(TABLE_LIBDIR       "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/")

set(TABLE_LIBRARIES table::table)

# All targets are created, even when some might not be requested though COMPONENTS.
# This is done for compatibility with CMake generated TABLE-target.cmake files.

set(_TABLE_library     "${TABLE_LIBDIR}/table.lib")
set(_TABLE_dll_library "${TABLE_BINDIR}/table.dll")
if(EXISTS "${_TABLE_library}" AND EXISTS "${_TABLE_dll_library}")
    if(NOT TARGET table::table)
        add_library(table::table SHARED IMPORTED)
        set_target_properties(table::table
            PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${TABLE_INCLUDE_DIRS}"
                IMPORTED_IMPLIB "${_TABLE_library}"
                IMPORTED_LOCATION "${_TABLE_dll_library}"
                COMPATIBLE_INTERFACE_BOOL "TABLE_SHARED"
                INTERFACE_TABLE_SHARED "ON"
        )
    endif()
    set(TABLE_TABLE_FOUND TRUE)
else()
    set(TABLE_TABLE_FOUND FALSE)
endif()
unset(_TABLE_library)
unset(_TABLE_dll_library)

check_required_components(TABLE)
