#-------------------------------------------------------------------------------
#                                CMAKE settings
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.10)

# Project declaration
project(BorASM VERSION 0.0.1 LANGUAGES C CXX)

string(ASCII 27 Esc)
set(Red "${Esc}[31m")
set(Green "${Esc}[32m")
set(Yellow "${Esc}[33m")
set(Blue "${Esc}[34m")
set(Magenta "${Esc}[35m")
set(ColourReset "${Esc}[m")

#-------------------------------------------------------------------------------
#                                Project settings & Version Management
#-------------------------------------------------------------------------------
# Include version management
include(${CMAKE_SOURCE_DIR}/cmake/version.cmake)

# Extract version from package.json
extract_version_from_json(
    ${CMAKE_SOURCE_DIR}/package.json 
    PROJECT_VERSION_MAJOR
    PROJECT_VERSION_MINOR 
    PROJECT_VERSION_PATCH
)

# Set project version
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# Generate version header
set(VERSION_HEADER_FILE "${CMAKE_SOURCE_DIR}/include/version.h")
generate_version_header(
    ${PROJECT_VERSION_MAJOR}
    ${PROJECT_VERSION_MINOR}
    ${PROJECT_VERSION_PATCH}
    ${VERSION_HEADER_FILE}
)

set (OUTPUT_BINARY_NAME BorASM.x64)
message(STATUS "Version: " ${PROJECT_VERSION})
message(STATUS "Binary name: " ${OUTPUT_BINARY_NAME})
message(STATUS "Processing CMakeLists.txt for: " ${PROJECT_NAME} " " ${PROJECT_VERSION})
message(STATUS "CMAKE_SOURCE_DIR:    " ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Separate flags for C and C++ compilers
set(C_COMPILER_FLAGS_DEBUG "-O0 -g3 -Wall -Wextra -fmessage-length=0 -Wno-psabi -pthread -lrt")
set(C_COMPILER_FLAGS_RELEASE "-O2 -DNDEBUG -Wall -fmessage-length=0 -Wno-psabi -pthread -lrt")
set(CXX_COMPILER_FLAGS_DEBUG "-O0 -g3 -Wall -Wextra -fmessage-length=0 -Wno-psabi -pthread -lrt")
set(CXX_COMPILER_FLAGS_RELEASE "-O2 -DNDEBUG -Wall -fmessage-length=0 -Wno-psabi -pthread -lrt")

message(STATUS "C++ standard set to: " ${CMAKE_CXX_STANDARD})
message(STATUS "C compiler flags for debug set to: " ${C_COMPILER_FLAGS_DEBUG})
message(STATUS "C compiler flags for release set to: " ${C_COMPILER_FLAGS_RELEASE})
message(STATUS "C++ compiler flags for debug set to: " ${CXX_COMPILER_FLAGS_DEBUG})
message(STATUS "C++ compiler flags for release set to: " ${CXX_COMPILER_FLAGS_RELEASE})

# Set the C compiler flags for Debug build.
set(CMAKE_C_FLAGS_DEBUG ${C_COMPILER_FLAGS_DEBUG})

# Set the C compiler flags for Release build.
set(CMAKE_C_FLAGS_RELEASE ${C_COMPILER_FLAGS_RELEASE})

# Set the CXX compiler flags for Debug build.
set(CMAKE_CXX_FLAGS_DEBUG ${CXX_COMPILER_FLAGS_DEBUG})

# Set the CXX compiler flags for Release build.
set(CMAKE_CXX_FLAGS_RELEASE ${CXX_COMPILER_FLAGS_RELEASE})

#-------------------------------------------------------------------------------
#                                Project options
#-------------------------------------------------------------------------------
option(PROJECT_TESTS         "Build tests"           OFF)

if(PROJECT_TESTS)
    message(STATUS "Build test enabled")
endif()

# Define the destination for the binary object
set (BUILD_DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Define the location/search path for the libraries
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Build type is: " ${CMAKE_BUILD_TYPE})
    set (LOGGER_LIBS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/logger/x64/Debug/libspdlogd.so.1.15)
    message(STATUS "Linking to: " ${LOGGER_LIBS_PATH})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set (LOGGER_LIBS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/logger/x64/Release/libspdlog.so.1.15)
    message(STATUS "Linking to: " ${LOGGER_LIBS_PATH})
endif()

#------------------------------------------------------------------------------------------
#                                Project include
#------------------------------------------------------------------------------------------
set(PROJECT_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
message(STATUS "Set include directory: " ${PROJECT_INCLUDE})
include_directories(
    ${PROJECT_INCLUDE}
)

#-------------------------------------------------------------------------------
#                                Project third-party include
#-------------------------------------------------------------------------------
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/logger/include
)

#-------------------------------------------------------------------------------
#                                Project source directory
#-------------------------------------------------------------------------------
set (PROJECT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
message(STATUS "Set source directory: " ${PROJECT_SRC_DIR})

#-------------------------------------------------------------------------------
#                                Project source
#-------------------------------------------------------------------------------
set (SOURCE_LIST
	${PROJECT_SRC_DIR}/main.cpp
    ${PROJECT_SRC_DIR}/BorAsm.cpp
    ${PROJECT_SRC_DIR}/CmdArgumentsParser.cpp
    ${PROJECT_SRC_DIR}/ArgumentsParserLogic.cpp
    ${PROJECT_SRC_DIR}/Tools.cpp
)
message(STATUS "\n *********** Source files list *********** \n")
foreach(src_file IN LISTS SOURCE_LIST)
    get_filename_component(file_name ${src_file} NAME)
    message(STATUS "  ${Magenta} ${file_name} ${ColourReset}")
endforeach()
message(STATUS "\n *********** End source files list *********** \n")

#-------------------------------------------------------------------------------
#                                Project links directories
#-------------------------------------------------------------------------------
link_directories(${LOGGER_LIBS_PATH})
message(STATUS "link_directories: " ${LOGGER_LIBS_PATH})

#-------------------------------------------------------------------------------
#                                Project build
#-------------------------------------------------------------------------------
add_executable(${OUTPUT_BINARY_NAME} ${SOURCE_LIST})
set_target_properties(${OUTPUT_BINARY_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BUILD_DESTINATION})
message(STATUS "Binary destination: " ${BUILD_DESTINATION})
message(STATUS "Binary name: " ${OUTPUT_BINARY_NAME})


#-------------------------------------------------------------------------------
#                                Project links
#-------------------------------------------------------------------------------
target_link_libraries(${OUTPUT_BINARY_NAME} PUBLIC ${LOGGER_LIBS_PATH})

#-------------------------------------------------------------------------------
#                                Project tests
#-------------------------------------------------------------------------------
if(PROJECT_TESTS)
    add_subdirectory(tests)
    message(STATUS "Tests subdirectory added")
endif(PROJECT_TESTS)
