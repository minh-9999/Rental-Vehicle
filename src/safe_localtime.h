#pragma once

#ifdef _WIN32
#define SAFE_LOCALTIME(timep, result) localtime_s(result, timep)
#else
#define SAFE_LOCALTIME(timep, result) localtime_r(timep, result)
#endif
