#ifndef KNOCK_CONFIG_CONFIG_HPP
#define KNOCK_CONFIG_CONFIG_HPP

#if defined(WIN32) || defined(_WIN32)
    #define KNOCK_OS_WIN
#elif defined(WIN64)
    #define KNOCK_OS_WIN
#else
    #define KNOCK_OS_LINUX
#endif

#ifdef KNOCK_OS_WIN
    #define NOMINMAX
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

#endif // End of include guard