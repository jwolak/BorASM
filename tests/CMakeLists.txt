#===============================================================================
#                           BorASM Tests CMakeLists.txt
#===============================================================================
# CMake configuration for BorASM project tests
# Supports Google Test framework
#===============================================================================

cmake_minimum_required(VERSION 3.10)

# Test project settings
set(TEST_PROJECT_NAME "BorASM-Tests.x64")
message(STATUS "${Green}Configuring tests: ${TEST_PROJECT_NAME}${ColourReset}")

#-------------------------------------------------------------------------------
#                           Find Required Packages
#-------------------------------------------------------------------------------
# Find Google Test
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(GTEST gtest)
    pkg_check_modules(GTEST_MAIN gtest_main)
endif()

# If pkg-config didn't find GTest, try find_package
if(NOT GTEST_FOUND)
    find_package(GTest QUIET)
    if(GTest_FOUND OR GTEST_FOUND)
        set(GTEST_LIBRARIES ${GTEST_LIBRARIES} gtest)
        set(GTEST_MAIN_LIBRARIES ${GTEST_MAIN_LIBRARIES} gtest_main)
        message(STATUS "${Green}Found Google Test via find_package${ColourReset}")
    endif()
endif()

# Find Threads
find_package(Threads REQUIRED)

#-------------------------------------------------------------------------------
#                              Test Sources
#-------------------------------------------------------------------------------
# Collect test source files
file(GLOB_RECURSE TEST_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# Main project sources (excluding main.cpp to avoid multiple main functions)
file(GLOB MAIN_PROJECT_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
list(FILTER MAIN_PROJECT_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

message(STATUS "Test sources found: ${TEST_SOURCES}")
message(STATUS "Main project sources for testing: ${MAIN_PROJECT_SOURCES}")

#-------------------------------------------------------------------------------
#                            Include Directories
#-------------------------------------------------------------------------------
set(TEST_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/ThirdParty/logger/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add system GTest includes if found
if(GTEST_FOUND OR GTest_FOUND)
    list(APPEND TEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIRS})
endif()

#-------------------------------------------------------------------------------
#                             Library Setup
#-------------------------------------------------------------------------------
# Set up logger library path based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(TEST_LOGGER_LIBS_PATH ${CMAKE_SOURCE_DIR}/ThirdParty/logger/x64/Debug/libspdlogd.so.1.15)
    set(TEST_COMPILER_FLAGS ${CXX_COMPILER_FLAGS_DEBUG})
    message(STATUS "${Blue}Tests using Debug logger: ${TEST_LOGGER_LIBS_PATH}${ColourReset}")
else()
    set(TEST_LOGGER_LIBS_PATH ${CMAKE_SOURCE_DIR}/ThirdParty/logger/x64/Release/libspdlog.so.1.15)
    set(TEST_COMPILER_FLAGS ${CXX_COMPILER_FLAGS_RELEASE})
    message(STATUS "${Blue}Tests using Release logger: ${TEST_LOGGER_LIBS_PATH}${ColourReset}")
endif()

#-------------------------------------------------------------------------------
#                            Build Configuration
#-------------------------------------------------------------------------------
if(TEST_SOURCES AND (GTEST_FOUND OR GTest_FOUND))
    # Create test executable
    add_executable(${TEST_PROJECT_NAME} ${TEST_SOURCES} ${MAIN_PROJECT_SOURCES})
    
    # Set include directories
    target_include_directories(${TEST_PROJECT_NAME} PRIVATE ${TEST_INCLUDE_DIRS})
    
    # Set compiler flags - parse the string properly
    string(REPLACE " " ";" TEST_CXX_FLAGS_LIST ${TEST_COMPILER_FLAGS})
    target_compile_options(${TEST_PROJECT_NAME} PRIVATE ${TEST_CXX_FLAGS_LIST})
    
    # Link libraries
    target_link_libraries(${TEST_PROJECT_NAME} 
        PRIVATE
        ${GTEST_LIBRARIES}
        ${GTEST_MAIN_LIBRARIES}
        Threads::Threads
        ${TEST_LOGGER_LIBS_PATH}
        rt
    )
    
    # Set output directory for test executable
    set_target_properties(${TEST_PROJECT_NAME} 
        PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )
    
    message(STATUS "${Green}Test executable configured: ${TEST_PROJECT_NAME}${ColourReset}")
    message(STATUS "Test binary will be created at: ${CMAKE_CURRENT_BINARY_DIR}/bin/${TEST_PROJECT_NAME}")

    #---------------------------------------------------------------------------
    #                            Enable Testing
    #---------------------------------------------------------------------------
    enable_testing()
    
    # Add test to CTest
    add_test(
        NAME ${TEST_PROJECT_NAME}
        COMMAND ${TEST_PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
    )
    
    # Set test properties
    set_tests_properties(${TEST_PROJECT_NAME}
        PROPERTIES
        TIMEOUT 300
        LABELS "unit;gtest"
        ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_SOURCE_DIR}/ThirdParty/logger/x64/$<IF:$<CONFIG:Debug>,Debug,Release>"
    )
    
    #---------------------------------------------------------------------------
    #                           Custom Targets
    #---------------------------------------------------------------------------
    # Target to run tests directly
    add_custom_target(run-tests
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/bin/${TEST_PROJECT_NAME}
        DEPENDS ${TEST_PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
        COMMENT "${Blue}Running BorASM tests...${ColourReset}"
        VERBATIM
    )
    
    # Target to run tests with verbose output
    add_custom_target(test-verbose
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/bin/${TEST_PROJECT_NAME} 
                --gtest_output=xml:${CMAKE_CURRENT_BINARY_DIR}/test_results.xml 
                --gtest_color=yes
                --gtest_print_time=1
        DEPENDS ${TEST_PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
        COMMENT "${Blue}Running BorASM tests with verbose output...${ColourReset}"
        VERBATIM
    )
    
    # Target to run tests with coverage (if gcov is available)
    find_program(GCOV_PATH gcov)
    if(GCOV_PATH)
        add_custom_target(test-coverage
            COMMAND ${CMAKE_CURRENT_BINARY_DIR}/bin/${TEST_PROJECT_NAME}
            COMMAND gcov ${TEST_SOURCES} ${MAIN_PROJECT_SOURCES}
            DEPENDS ${TEST_PROJECT_NAME}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "${Blue}Running tests with coverage analysis...${ColourReset}"
            VERBATIM
        )
    endif()

elseif(NOT TEST_SOURCES)
    message(STATUS "${Yellow}No test source files found in ${CMAKE_CURRENT_SOURCE_DIR}/src/${ColourReset}")
    message(STATUS "${Yellow}Create test files in tests/src/ directory to enable testing${ColourReset}")
    
    # Create a helper target to generate sample test
    add_custom_target(generate-sample-test
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/src
        COMMAND ${CMAKE_COMMAND} -E echo "Creating sample test file..."
        COMMENT "${Blue}Creating sample test file...${ColourReset}"
        VERBATIM
    )
    
elseif(NOT GTEST_FOUND AND NOT GTest_FOUND)
    message(WARNING "${Red}Google Test not found! Please install libgtest-dev${ColourReset}")
    message(STATUS "To install on Ubuntu/Debian: sudo apt-get install libgtest-dev cmake")
    message(STATUS "After installation, reconfigure with: cmake ..")
    
    # Create installation helper target
    add_custom_target(install-gtest-deps
        COMMAND sudo apt-get update
        COMMAND sudo apt-get install -y libgtest-dev cmake build-essential
        COMMENT "${Blue}Installing Google Test dependencies...${ColourReset}"
        VERBATIM
    )
endif()

#-------------------------------------------------------------------------------
#                              Helper Script
#-------------------------------------------------------------------------------


#-------------------------------------------------------------------------------
#                            Final Status
#-------------------------------------------------------------------------------
message(STATUS "${Magenta}===============================================================================${ColourReset}")
message(STATUS "${Magenta}                          BorASM Tests Configuration${ColourReset}")
message(STATUS "${Magenta}===============================================================================${ColourReset}")

if(TEST_SOURCES AND (GTEST_FOUND OR GTest_FOUND))
    message(STATUS "${Green}✓ Tests configured successfully${ColourReset}")
    message(STATUS "  Test executable: ${TEST_PROJECT_NAME}")
    list(LENGTH TEST_SOURCES TEST_SOURCES_COUNT)
    list(LENGTH MAIN_PROJECT_SOURCES MAIN_SOURCES_COUNT)
    message(STATUS "  Test sources: ${TEST_SOURCES_COUNT} files")
    message(STATUS "  Main sources: ${MAIN_SOURCES_COUNT} files")
    message(STATUS "")
    message(STATUS "Build and run commands:")
    message(STATUS "  make ${TEST_PROJECT_NAME}  # Build tests")
    message(STATUS "  make run-tests             # Build and run tests")
    message(STATUS "  make test-verbose          # Run with verbose output")
    message(STATUS "  ctest                      # Run via CTest")
else()
    message(STATUS "${Yellow}⚠ Tests not fully configured${ColourReset}")
    if(NOT TEST_SOURCES)
        message(STATUS "  Issue: No test source files found")
        message(STATUS "  Solution: make generate-sample-test")
    endif()
    if(NOT GTEST_FOUND AND NOT GTest_FOUND)
        message(STATUS "  Issue: Google Test not found")
        message(STATUS "  Solution: make install-gtest-deps")
    endif()
endif()

message(STATUS "${Magenta}===============================================================================${ColourReset}")
