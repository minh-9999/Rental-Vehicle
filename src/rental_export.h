#pragma once

#ifdef _WIN32
    #ifdef RENTAL_LIB_EXPORTS
        #define RENTAL_API __declspec(dllexport)
        // #pragma message("🔧 RENTAL_LIB_EXPORTS is defined → Exporting symbols")
    #else
        #define RENTAL_API __declspec(dllimport)
    #endif

#else
    #ifdef __GNUC__
        #define RENTAL_API __attribute__( (visibility("default")) )
    #else
        #define RENTAL_API

    #endif

#endif