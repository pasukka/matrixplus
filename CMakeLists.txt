cmake_minimum_required(VERSION 3.20)
project(s21_matrix_oop)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
add_compile_options("-Wall" "-Werror" "-Wextra" "-Wpedantic" "-Weffc++")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/modules)

add_subdirectory(dependencies)

# ---- LIB COMPILATION ----
file(GLOB SRC_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/lib/*.h)
file(GLOB SRC_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/lib/*.cc)
add_library(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/lib)

# ---- TEST COMPILATION ----
file(GLOB TEST_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/test/*.cc)
add_executable(test ${TEST_FILES})
target_link_libraries(test PUBLIC ${PROJECT_NAME} gtest gtest_main)

# ---- GCOV-REPORT ----
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND CMAKE_BUILD_TYPE STREQUAL "Debug")
include(CodeCoverage)
append_coverage_compiler_flags_to_target(test)
append_coverage_compiler_flags_to_target(${PROJECT_NAME})
setup_target_for_coverage_lcov(
            NAME test_coverage
            EXECUTABLE test
            EXCLUDE ${CMAKE_BINARY_DIR}/* "${PROJECT_SOURCE_DIR}/dependencies/*" "/Applications/*" "/usr/local/*" "/usr/include/*"
            DEPENDENCIES test
    )
endif()

# ---- FORMATTING ----
set(CLANG_FORMAT_EXCLUDE_PATTERNS  "build/" "dependencies/" ${CMAKE_BINARY_DIR})
find_package(ClangFormat)
