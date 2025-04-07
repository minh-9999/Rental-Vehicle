
#include "config.h"
#include <sstream>
#include <iomanip>
#include "safe_localtime.h"

string getTimestampedFilename()
{
    time_t now = time(nullptr);
    char buf[64];
    tm lctm;
    // strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", localtime(&now));
    SAFE_LOCALTIME(&now, &lctm);
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &lctm);
    return string("logs/test_results_") + buf + ".xml";
}

string formatCurrency(double value)
{
    ostringstream oss;
    try
    {
        oss.imbue(locale("en_US.UTF-8"));
    }
    catch (const exception &)
    {
        oss.imbue(locale(""));
    }
    oss << fixed << setprecision(2) << value;
    return oss.str();
}

time_t stringToTime(const string &timeStr)
{
    tm tmt = {};
    istringstream ss(timeStr);
    ss >> get_time(&tmt, "%d-%m-%Y %H:%M:%S");

    if (ss.fail() || !ss.eof()) // check if the entire string was parsed
        throw runtime_error("Error: Invalid time format. Expected format: dd-mm-yyyy hh:mm:ss");

    time_t t = mktime(&tmt);

    if (t == -1)
        throw runtime_error("Error: Could not convert to time_t");

    if (t < 0)
        throw runtime_error("Error: Time conversion resulted in invalid epoch time.");

    return t;
}

time_t convertStringToTime(const string &timeStr)
{
    tm tmt = {};
    int day, month, year, hour, minute, second;

    if (SAFE_SSCANF(timeStr.c_str(), "%d-%d-%d %d:%d:%d", &day, &month, &year, &hour, &minute, &second) != 6)
        throw runtime_error("Error: Invalid time format. Expected format: dd-mm-yyyy hh:mm:ss");

    tmt.tm_mday = day;
    tmt.tm_mon = month - 1;    // Adjust for tm structure (months are 0-11)
    tmt.tm_year = year - 1900; // Adjust for tm structure (years are since 1900)
    tmt.tm_hour = hour;
    tmt.tm_min = minute;
    tmt.tm_sec = second;

    time_t t = mktime(&tmt);
    if (t == -1)
        throw runtime_error("Error: Could not convert to time_t");

    return t;
}