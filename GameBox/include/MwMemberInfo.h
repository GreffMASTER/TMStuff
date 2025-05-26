#ifndef MWMEMBERINFO_H_INCLUDED
#define MWMEMBERINFO_H_INCLUDED

#include "GbxTypes.h"

typedef struct SMwMemberInfo
{
    enum eType {
        ACTION = 0,
        BOOL = 1,
        BOOLARRAY = 2,
        BOOLBUFFER = 3,
        BOOLBUFFERCAT = 4,
        CLASS = 5,
        CLASSARRAY = 6,
        CLASSBUFFER = 7,
        CLASSBUFFERCAT = 8,
        COLOR = 9,
        COLORARRAY = 10,
        COLORBUFFER = 11,
        COLORBUFFERCAT = 12,
        ENUM = 13,

        INT = 14,
        INTARRAY = 15,
        INTBUFFER = 16,
        INTBUFFERCAT = 17,
        INTRANGE = 18,

        ISO4 = 19,
        ISO4ARRAY = 20,
        ISO4BUFFER = 21,
        ISO4BUFFERCAT = 22,
        ISO3 = 23,
        ISO3ARRAY = 24,
        ISO3BUFFER = 25,
        ISO3BUFFERCAT = 26,
        NATURAL = 27,
        NATURALARRAY = 28,
        NATURALBUFFER = 29,
        NATURALBUFFERCAT = 30,
        NATURALRANGE = 31,
        REAL = 32,
        REALARRAY = 33,
        REALBUFFER = 34,
        REALBUFFERCAT = 35,
        REALRANGE = 36,
        ID = 37,
        IDARRAY = 38,
        IDBUFFER = 39,
        IDBUFFERCAT = 40,
        VEC2 = 41,
        VEC2ARRAY = 42,
        VEC2BUFFER = 43,
        VEC2BUFFERCAT = 44,
        VEC3 = 45,
        VEC3ARRAY = 46,
        VEC3BUFFER = 47,
        VEC3BUFFERCAT = 48,
        VEC4 = 49,
        VEC4ARRAY = 50,
        VEC4BUFFER = 51,
        VEC4BUFFERCAT = 52,
        NAT3 = 53,
        NAT3ARRAY = 54,
        NAT3BUFFER = 55,
        NAT3BUFFERCAT = 56,
        PROC = 57
    };
    enum eFlags {
        GET         = 0b00000001,
        SET         = 0b00000010,
        ADD         = 0b00000100,
        SUB         = 0b00001000,
        VIRTUAL_GET = 0b00010000,
        VIRTUAL_SET = 0b00100000,
        VIRTUAL_ADD = 0b01000000,
        VIRTUAL_SUB = 0b10000000,
    };
    eType type;
    Natural memberId;
    CMwParam* pParam;
    int fieldOffset;
    char* pszName;
    char* pszName2;
    char* pszName3;
    int flags;
    int flags2;
}SMwMemberInfo;

typedef struct SMwActionInfo : public SMwMemberInfo
{
    void (__fastcall* pFun)(CMwNod* nod);
}SMwActionInfo;

typedef struct SMwClassMemberInfo : public SMwMemberInfo
{
    CMwClassInfo* pClassInfo;
}SMwClassMemberInfo;

typedef struct SMwClassArrayMemberInfo : public SMwMemberInfo
{
    int u1;
    char* pszFriendlyClassName;
    int u2;
    CMwClassInfo* pClassInfo;
}SMwClassArrayMemberInfo;

typedef struct SMwMemberInfoIntRange : public SMwMemberInfo
{
    Integer min;
    Integer max;
}SMwMemberInfoIntRange;

typedef struct SMwMemberInfoRealRange : public SMwMemberInfo
{
    Real min;
    Real max;
}SMwMemberInfoRealRange;

typedef struct SMwEnumInfo : public SMwMemberInfo
{
    char* pszEnumName;
    int numValues;
    char** ppszEnumValues;
}SMwEnumInfo;

#endif // MWMEMBERINFO_H_INCLUDED
