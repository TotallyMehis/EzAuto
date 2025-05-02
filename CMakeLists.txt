cmake_minimum_required(VERSION 3.29.5)

project(ezauto VERSION 1.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE sources src/*.cpp)
list(FILTER sources EXCLUDE REGEX main\.cpp|tests)
file(GLOB_RECURSE headers src/*.h)


add_library(ezauto_lib ${sources} ${headers} src/icon_rc.rc)
add_executable(ezauto_exe src/main.cpp src/icon_rc.rc)
target_link_libraries(ezauto_exe ezauto_lib)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ezauto_exe)
# CWD to ./ezauto for debugging
set_target_properties(ezauto_exe PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ezauto OUTPUT_NAME ezauto)

if(NOT DEFINED EZAUTO_BUILD)
    set(EZAUTO_BUILD "-manual")
    message("Configuring as manual build")
else()
    message("EZAUTO_BUILD is ${EZAUTO_BUILD}")
endif()

target_compile_definitions(ezauto_lib PUBLIC EZAUTO_VERSION="${CMAKE_PROJECT_VERSION}${EZAUTO_BUILD}")

# Copy to ./ezauto
add_custom_command(
    TARGET ezauto_exe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ezauto_exe> ${CMAKE_CURRENT_SOURCE_DIR}/ezauto
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:ezauto_exe> ${CMAKE_CURRENT_SOURCE_DIR}/ezauto
    COMMAND_EXPAND_LISTS
)

#
# Test project
#
include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/79219e26e0e36b415a5804b6b017ad6c6cd99ad8.zip)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE tests src/tests/*.cpp)

add_executable(ezauto_test ${tests})

target_include_directories(ezauto_test PRIVATE src)
target_link_libraries(ezauto_test ezauto_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(ezauto_test)
