#include "../third_party/googletest-src/googletest/include/gtest/gtest.h"
// #include "gtest/gtest.h"
// #include <ctime>
// #include <iostream>
// #include <string>
// #include <filesystem>
#include "config.h"

// Main function for Google Test
int main(int argc, char **argv)
{
    // ::testing::GTEST_FLAG(output) = "xml:logs/test_results.xml";
    ::testing::GTEST_FLAG(output) = "xml:" + getTimestampedFilename();

    ::testing::InitGoogleTest(&argc, argv);

    // std::cout << "Current working directory: " << std::filesystem::current_path() << '\n';

    // std::filesystem::create_directories("logs");

    // Generate a timestamped XML log file
    // std::string timestamp = std::to_string(std::time(nullptr));
    // ::testing::GTEST_FLAG(output) = "xml:logs/test_results_" + timestamp + ".xml";

    return RUN_ALL_TESTS();
}
