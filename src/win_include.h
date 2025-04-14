#pragma once

#ifdef _WIN32
// prevent include many unimportant headers
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// prevent conflict with std::min / std::max
#ifndef NOMINMAX
#define NOMINMAX
#endif

// prevent conflict 'byte' between Windows API to std::byte
#define byte windows_byte_workaround
#include <windows.h>
#undef byte

#endif // _WIN32
