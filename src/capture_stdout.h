#ifndef UTILS_CAPTURE_STDOUT_H
#define UTILS_CAPTURE_STDOUT_H

#include <cstdio>
#include <cstdlib>
#include <functional>
#include <stdexcept>
#include <string>

#ifdef _WIN32
// #define _CRT_SECURE_NO_WARNINGS
#include <io.h>
#define fileno _fileno
#define dup _dup
#define dup2 _dup2
#else
#include <unistd.h>
#endif

#include "../third_party/fmt-src/include/fmt/core.h"
#include <sstream>
#include <iostream>

using namespace std;

#define CAPTURE_STDOUT(output_var, lambda_body) \
    string output_var = ::utils::capture_stdout([&]() { lambda_body; });

namespace utils
{

    inline string capture_stdout(const function<void()> &func)
    {
        // save the present stdout
        FILE *original_stdout = stdout;

        // Backup stdout file descriptor
        int stdout_fd = fileno(stdout);
        int backup_fd = dup(stdout_fd);
        if (backup_fd == -1)
            throw runtime_error("Failed to backup stdout file descriptor! \n");

        // make a temporary file to capture the output
        FILE *temp_file = nullptr;

#ifdef _WIN32
        if (tmpfile_s(&temp_file) || !temp_file)
            throw runtime_error(" Cannot create temporary file! \n");

#else
        temp_file = tmpfile();
        if (!temp_file)
            throw runtime_error(" Cannot create temporary file! \n");

#endif

        // redirect stdout to the temporary file
        fflush(stdout);
        dup2(fileno(temp_file), fileno(stdout));

        // execute the function
        func();

        // flush output and reset original stdout
        fflush(stdout);
        dup2(fileno(original_stdout), fileno(stdout));

#ifdef _WIN32
        _close(backup_fd);
#else
        close(backup_fd);
#endif

        // Read from temp file
        string output;
        char buffer[1024];
        rewind(temp_file);
        while (fgets(buffer, sizeof(buffer), temp_file))
            output += buffer;

        fclose(temp_file);

        // Debugging output
        cout << "\n\t Captured output: " << output << "\n";

        return output;
    }

    inline string capture_stdout_with_fmt(const function<void()> &func)
    {
        stringstream buffer;

        // Save original stdout
        auto old_buf = cout.rdbuf();
        cout.rdbuf(buffer.rdbuf());

        // Execute the function
        func();

        // Restore stdout
        cout.rdbuf(old_buf);

        return buffer.str();
    }
} // namespace utils

#endif // UTILS_CAPTURE_STDOUT_H
