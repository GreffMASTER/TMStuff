#ifndef MWSTACK_H_INCLUDED
#define MWSTACK_H_INCLUDED

#include "MwNod.h"
#include "MwMemberInfo.h"

// TODO figure out how to properly use this thing
class CMwStack : public CMwNod {
    public:
        int m_numElems;
        int m_Size;
        SMwMemberInfo** ppMemberInfos;
        int* ppTypes;
        int iCurrentPos;

        static CMwStack* NewCMwStack()
        {
            CMwStack* stacc = (CMwStack*)malloc(sizeof(CMwStack));
            CMwStack* (__thiscall* callable)(CMwStack*) = (CMwStack* (__thiscall*)(CMwStack*))0x004dec80;
            return callable(stacc);
        }

        static int Push(CMwStack* stacc, int member_id, CMwNod* nod, CFastString* str)
        {
            int (__thiscall* callable)(CMwStack*, int, CMwNod*, CFastString*) = (int (__thiscall*)(CMwStack*, int, CMwNod*, CFastString*))0x004df4e0;
            return callable(stacc, member_id, nod, str);

        }
};

// NOTES
// type 0 = CMwMemberInfo*
// type 1 = Index (like in array)
// type 2 = MwId


#endif // MWSTACK_H_INCLUDED
