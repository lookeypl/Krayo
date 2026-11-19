#pragma once

#ifdef WIN32
    #ifdef KRAYO_EXPORTS
        #define KRAYO_API __declspec(dllexport)
    #else
        #define KRAYO_API __declspec(dllimport)
    #endif
#else
    #error "Target platform not supported"
#endif
