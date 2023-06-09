cmake_minimum_required (VERSION 3.5)
project(CxxCanard)
include(CTest)

# execute process to generate libcanard headers using dronecan_dsdlc/dronecan_dsdlc.py
execute_process(COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/dronecan_dsdlc/dronecan_dsdlc.py
    -O ${CMAKE_CURRENT_BINARY_DIR}/dsdlc_generated
    DSDL/uavcan
    DSDL/dronecan
    DSDL/ardupilot
    DSDL/com
    )

if (CANARD_ENABLE_CANFD)
    add_definitions(-DCANARD_ENABLE_CANFD=1)
endif()

# generate dsdl messages
include_directories(${CMAKE_CURRENT_BINARY_DIR}/dsdlc_generated/include)

# add local directory to include path
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../)

# add libcanard source files
file(GLOB LIBCANARD_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../../*.c)
list(APPEND SRC_FILES ${LIBCANARD_SOURCES})

# glob all generated dsdlc files
file(GLOB DSDL_GENERATED_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dsdlc_generated/src/*.c*)

# append DSDL_GENERATED_SOURCES_C to SRC_FILES
list(APPEND SRC_FILES ${DSDL_GENERATED_SOURCES})

add_compile_options(-Wall -m32 -g)
add_link_options(-m32)

add_subdirectory(googletest)

enable_testing()

set(SRC_FILES_TEST ${SRC_FILES}
                   cxx_test_interface.cpp
                   test_cxx_wrappers.cpp)
set(SRC_FILES_CANARD ${SRC_FILES}
                     canard_interface.cpp
                     test_canard_interface.cpp)

include_directories(googletest/googletest/include)

if (CANARD_ENABLE_COVERAGE)
    add_compile_options(-fprofile-arcs -ftest-coverage)
    add_link_options(-fprofile-arcs -ftest-coverage)
endif()

# create test target
add_executable(test_cf ${SRC_FILES_TEST})
add_executable(test_canard ${SRC_FILES_CANARD})

target_link_libraries(test_cf GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(test_cf)

target_link_libraries(test_canard GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(test_canard)

if (CANARD_ENABLE_COVERAGE)
    add_custom_target(coverage
                        COMMAND lcov --directory . --capture --output-file coverage.info
                        COMMAND lcov --remove coverage.info '/usr/*' '*gtest*' '*tests/*' '*dsdlc_generated/*' --output-file coverage.info
                        COMMAND lcov --list coverage.info
                        COMMAND genhtml coverage.info --output-directory coverage
                        COMMENT echo "Coverage report generated at ${CMAKE_BINARY_DIR}/coverage/index.html"
                        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    add_custom_command(TARGET coverage POST_BUILD COMMAND ;)
endif()
