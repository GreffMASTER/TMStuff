#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include <iostream>
#include "GameBox/include/GbxTools.h"

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

#define DllFuncExport extern "C" __declspec( dllexport )


#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
