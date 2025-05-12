#ifndef MWPARAM_H_INCLUDED
#define MWPARAM_H_INCLUDED

#include "GbxTypes.h"

class CMwParam : public CMwNod
{
    public:
        virtual int U32(void); // 32
        virtual int U33(void);
        virtual int U34(void); // 34
        virtual int U35(void);
        virtual int U36(void); // 36
        virtual int U37(void);
        virtual int StructGetCount(void); // 38
        virtual int VGetValue(void** offset, CMwStack* stacc, void** out);
        virtual int VSetValue(void** offset, CMwStack* stacc, void** in); // 40
        virtual int VAddValue(void** offset, CMwStack* stacc, void** in);
        virtual int VSubValue(void** offset, CMwStack* stacc, void** in); // 42
        virtual int VGetNextNod(CMwNod* nod, CMwStack* stacc, CMwNod** out);
        virtual int U44(void); // 44
        virtual int U45(void);
        virtual void GetValueFromString(CFastString* one, CFastString* two); // 46
        virtual void GetStringFromValue(CFastString* one, CFastString* two);
        virtual void GetIcon(int* one, int* two); // 48
        virtual void ParseSubParams(CMwNod *nod, CMwStack* stacc, CMwStack* stacc2, CMwNod* nod2, void* func, void* idk, int idk2); // 49
};

// GetIcon() - Sets int values to two pointers, values range from 1 to 4
// What is the purpose of this function? Debugger never hits a breakpoint at this address, therefore it must be unsed.
// Theory: it's used in the GameBox editor to maybe choose which icon to display next to the entry in the node/member list(?)
// Correlations:
//      CMwParam:            *one = 3, *two = 4
//      CMwParamClass:       *one = 2, *two = 2
//      CMwParamRefBuffer:   *one = 1, *two = 2

#endif // MWPARAM_H_INCLUDED
