cmake_minimum_required(VERSION 3.20)

project(Rental_Vehicle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NAME ${PROJECT_NAME})
message(STATUS "Project Name: ${PROJECT_NAME}")

# --------------------------- Add third party ---------------------------
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/third_party")
include(FetchContent)

# --------------------------- JSON ---------------------------
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

# --------------------------- FMT ---------------------------
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 11.1.4
)

# --------------------------- SPDLOG ---------------------------
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.1
)

FetchContent_makeavailable(json fmt spdlog)

# --------------------------- Add main library ---------------------------
add_library(
    rental_lib
    src/config.cc
    src/Vehicles.cc
    src/dataJson.cc
    src/RentalTransaction.cc
)

# add_library(
# rental_lib SHARED
# src/Vehicles.cc
# src/dataJson.cc
# src/RentalTransaction.cc
# )

# make sure the shared library is output to the bin directory
# set_target_properties(
# rental_lib PROPERTIES
# LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
# RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
# )

# Include directories for rental_lib
target_include_directories(
    rental_lib
    PUBLIC ${CMAKE_SOURCE_DIR}/src

    # PRIVATE ${json_BINARY_DIR}/single_include
    PRIVATE ${json_BINARY_DIR}/include
)

# Compile definitions
target_compile_definitions(rental_lib PRIVATE FMT_HEADER_ONLY=1)

# target_compile_definitions(
# rental_lib
# PRIVATE RENTAL_LIB_EXPORTS FMT_HEADER_ONLY = 1
# )

# Link external libraries to rental_lib
target_link_libraries(
    rental_lib
    PUBLIC fmt::fmt-header-only
    PRIVATE spdlog::spdlog_header_only
)

# target_link_libraries (
# rental_lib SHARED
# PRIVATE fmt::fmt
# PRIVATE spdlog::spdlog_header_only
# )

# --------------------------- Make executable ---------------------------
# add_executable(Rental-Vehicle src/main.cc)
if(WIN32)
    # make sure file .rc is embedded in the executable
    set(RESOURCE_FILE "${CMAKE_SOURCE_DIR}/resource/icon.rc")

    # add executable with resource file
    add_executable(${NAME} src/main.cc ${RESOURCE_FILE})

    # Link user32 library for Windows
    target_link_libraries(${NAME} PRIVATE user32)
else()
    add_executable(${NAME} src/main.cc)
endif()

if(NOT WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(${NAME} PROPERTIES INSTALL_RPATH "$ORIGIN")
endif()

# Include directories for the executable
target_include_directories(
    ${NAME}

    # PRIVATE ${json_BINARY_DIR}/single_include
    PRIVATE ${json_BINARY_DIR}/include
)

# Link rental_lib to executable
target_link_libraries(${NAME} PRIVATE rental_lib)

# --------------------------- Copy shared library ---------------------------
# if(WIN32)
# add_custom_command(TARGET Rental_Vehicle POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rental_lib> $<TARGET_FILE_DIR:Rental_Vehicle>)
# endif()

# --------------------------- Build test ---------------------------
enable_testing()
add_subdirectory(tests)
