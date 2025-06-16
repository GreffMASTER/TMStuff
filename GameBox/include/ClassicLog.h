#ifndef CLASSICLOG_H_INCLUDED
#define CLASSICLOG_H_INCLUDED

#include "GbxTypes.h"

class CClassicLog
{
    public:
        static void Setup(CClassicLog* log, char* log_name, int stuff)
        {
            void (__thiscall* callable)(CClassicLog*, char*, int) = (void (__thiscall*)(CClassicLog*, char*, int))0x0041d2b0;
            callable(log, log_name, stuff);
        }

        static CClassicLog* GetClassicLog()
        {
            return (CClassicLog*)0x0097c22c;
        }
};

#endif // CLASSICLOG_H_INCLUDED
