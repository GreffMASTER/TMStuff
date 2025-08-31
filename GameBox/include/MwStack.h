#ifndef MWSTACK_H_INCLUDED
#define MWSTACK_H_INCLUDED

#include "FastString.h"
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

        static int Allocate(CMwStack* stacc, int newSize)
        {
            int (__thiscall* callable)(CMwStack*, int) = (int (__thiscall*)(CMwStack*, int))0x004def90;
            return callable(stacc, newSize);
        }


        static int Push(CMwStack* stacc, int member_id, CMwNod* nod, CFastString* str)
        {
            int (__thiscall* callable)(CMwStack*, int, CMwNod*, CFastString*) = (int (__thiscall*)(CMwStack*, int, CMwNod*, CFastString*))0x004df4e0;
            int res = callable(stacc, member_id, nod, str);
            if(res == 0)
                stacc->iCurrentPos = stacc->m_Size - 1;
            return res;
        }

        static CMwStack* NewCMwStackFastString(char* str, CMwNod* nod)
        {
            CMwStack* stacc = NewCMwStack();
            CFastString* stacc_str = CFastString::NewCFastString(str);
            int res = Push(stacc, 0xFFFFFFFF, nod, stacc_str);
            stacc_str->Delete(1);
            if(res == 0)
                return stacc;

            CMwNod::MwRelease(stacc);
            return nullptr;
        }
};

// NOTES
// type 0 = CMwMemberInfo*
// type 1 = Index (like in array)
// type 2 = MwId


#endif // MWSTACK_H_INCLUDED
