#include <functional>
#include <string>
#include <regex>

using namespace std;

#ifdef _WIN32
#define SAFE_STRCPY(dest, size, src) strcpy_s(dest, size, src)

#define SAFE_SSCANF(str, format, ...) \
    sscanf_s(str, format, __VA_ARGS__)

#else
#define SAFE_STRCPY(dest, size, src) strcpy(dest, src)
#include <unistd.h>

#define SAFE_SSCANF(str, format, ...) \
    sscanf(str, format, __VA_ARGS__)

#endif

inline bool checkPattern_1(const string &str, const string &pattern)
{
    regex regex_pattern(pattern, regex_constants::icase); // icase for case-insensitive matching
    return regex_search(str, regex_pattern);
}

inline bool checkPattern_2(const string &str, const string &pattern)
{
    return str.find(pattern) != string::npos;
}

inline string trim(const string &str)
{
    size_t first = str.find_first_not_of(" \n\r\t");
    size_t last = str.find_last_not_of(" \n\r\t");

    if (first == string::npos || last == string::npos)
        return "";

    return str.substr(first, last - first + 1);
}

string getTimestampedFilename();

string formatCurrency(double);

time_t stringToTime(const string &);

time_t convertStringToTime(const string &);
