cmake_minimum_required(VERSION 3.20)

project(Rental-Vehicle-Tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing() # enable CTest

# fix warning for GoogleTest
add_definitions(
  -DGTEST_LINKED_AS_SHARED_LIBRARY=0
  -DGTEST_CREATE_SHARED_LIBRARY=0
)

# get GoogleTest if not found
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/third_party")
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git

  # ### Do not use newer versions (v1.15.0 or later) due to linking errors.
  GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

if(NOT TARGET GTest::gtest)
  message(FATAL_ERROR "GTest target was not found! Check if GTest was downloaded correctly.")
endif()

# find_package(GTest CONFIG REQUIRED)

# Link fmt if needed, remove this line if you're not using system-installed fmt
# find_package(fmt REQUIRED)

# Add include_directories manually if needed
# include_directories(${PROJECT_SOURCE_DIR}/third_party/fmt/include)

# ==================== UNIT TEST ====================
add_executable(
  UnitTests
  test_UnitVehicle.cc
  test_UnitDataJson.cc
  test_main.cc
)

# fix warning for GoogleTest
# target_compile_definitions(
# UnitTests PRIVATE
# GTEST_LINKED_AS_SHARED_LIBRARY=0
# GTEST_CREATE_SHARED_LIBRARY=0
# )

# Link libraries for UnitTests
target_link_libraries(
  UnitTests
  PRIVATE rental_lib

  # GTest::gtest_main
  GTest::gtest
  fmt::fmt-header-only
)

# ==================== INTEGRATION TEST ====================
add_executable(IntegrationTests test_integration.cc)

# fix warning for GoogleTest
# target_compile_definitions(
# IntegrationTests PRIVATE
# GTEST_LINKED_AS_SHARED_LIBRARY=0
# GTEST_CREATE_SHARED_LIBRARY=0
# )

# Link libraries for IntegrationTests
target_link_libraries(
  IntegrationTests
  PRIVATE rental_lib
  GTest::gtest_main
  fmt::fmt-header-only
)

# ==================== CTEST CONFIG ====================
add_test(
  NAME UnitTest
  COMMAND UnitTests --gtest_color=yes
)

add_test(
  NAME IntegrationTest
  COMMAND IntegrationTests --gtest_color=yes
)

# ==================== proceed the shared library ====================
# if(WIN32)
# # Copy rental_lib.dll into UnitTests and IntegrationTests
# add_custom_command(TARGET UnitTests POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rental_lib> $<TARGET_FILE_DIR:UnitTests>)
# add_custom_command(TARGET IntegrationTests POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rental_lib> $<TARGET_FILE_DIR:IntegrationTests>)
# else()
# # setup LD_LIBRARY_PATH for Linux/macOS when running tests
# set_tests_properties(UnitTest PROPERTIES ENVIRONMENT "LD_LIBRARY_PATH=$<TARGET_FILE_DIR:rental_lib>")
# set_tests_properties(IntegrationTest PROPERTIES ENVIRONMENT "LD_LIBRARY_PATH=$<TARGET_FILE_DIR:rental_lib>")
# endif()

# comfigure run tests and export results to XML
add_test(
  NAME IntegrationTests
  COMMAND integration_tests --gtest_output=xml:${CMAKE_BINARY_DIR}/logs/integration_test_results.xml
)

# ==================== HTML TEST REPORT ====================
# add_custom_command(TARGET UnitTests POST_BUILD
# COMMAND UnitTests --gtest_output=xml:logs/test_results.xml
# COMMAND xsltproc ${CMAKE_SOURCE_DIR}/gtest.xsl logs/test_results.xml > logs/test_results.html
# COMMENT "Generating HTML test report..."
# )
