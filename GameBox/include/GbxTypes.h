#ifndef GBXTYPES_H_INCLUDED
#define GBXTYPES_H_INCLUDED

#include <windows.h>
#include <iostream>

typedef int Integer;
typedef unsigned int Bool;
typedef unsigned char Nat8;
typedef unsigned short Nat16;
typedef unsigned int Natural, Nat32;
typedef float Real, Real32;

typedef void(*pFun)(void*);

class GmVec2;
class GmVec3;
class GmVec4;

class CMwId;
class CMwClassInfo;
class CFastString;
class CMwClassInfo;

class CClassicArchive;
class CClassicLog;
// nods
class CMwNod;
class CMwStack;
class CMwParam;
class CMwParamClass;
class CMwParamStruct;
class CMwParamBool;
class CMwNodMain;
class CMwCmdContainer;
class CGameProcess;
class CMwRefBuffer;
class CSystemFid;
class CHmsViewport;
class CVisionViewport;
class CVisionViewportDx9;
class CMwEngine;
class CMwEngineMain;


#include "GmVec2.h"
#include "GmVec3.h"
#include "GmVec4.h"
#include "GmIso4.h"

#include "MwId.h"
#include "FastString.h"
#include "FastBuffer.h"
#include "FastArray.h"

#include "ClassicArchive.h"
#include "ClassicLog.h"

#include "MwMemberInfo.h"
#include "MwClassInfo.h"
#include "MwEngineInfo.h"
#include "MwEngineManager.h"
// Nods
#include "MwNod.h"
#include "MwStack.h"
#include "MwParam.h"
#include "MwParamClass.h"
#include "MwParamStruct.h"
#include "MwParamBool.h"
#include "MwNodMain.h"
#include "MwCmdContainer.h"
#include "GameProcess.h"
#include "MwRefBuffer.h"
#include "SystemFid.h"
#include "HmsViewport.h"
#include "VisionViewport.h"
#include "VisionViewportDx9.h"
#include "MwEngine.h"
#include "MwEngineMain.h"

#include "SystemFid.h"
#include "SystemFidFile.h"
#include "SystemFids.h"
#include "SystemFidsFolder.h"
#include "SystemFidsDrive.h"


typedef struct SVec2 {
    union {
        struct {
            float x;
            float y;
        };
        struct {
            float array[2];
        };
    };
}SVec2;

typedef struct SVec3 {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        struct {
            float array[3];
        };
    };
}SVec3;

typedef struct SVec4 {
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        struct {
            float array[4];
        };
    };
}SVec4;

typedef struct SIso4 {
    union {
        struct {
            SVec3 s1;
            SVec3 s2;
            SVec3 s3;
            SVec3 s4;
        };
        struct {
            float array1[3];
            float array2[3];
            float array3[3];
            float array4[3];
        };
    };
}SIso4;

#endif // GBXTYPES_H_INCLUDED
