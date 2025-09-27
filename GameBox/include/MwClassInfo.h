#ifndef __MWCLASSINFO_H__
#define __MWCLASSINFO_H__

#include "GbxTypes.h"
#include "FastArray.h"

class CMwClassInfo
{
    public:
        void** vtable;
        Natural m_ClassId;
        CMwClassInfo * m_ParentClass;
        SFastArray<CMwClassInfo*> m_Childs;
        char* m_ClassName;
        CMwClassInfo * m_NextClass;
        int u3;
        CMwNod* (__stdcall* m_Constructor)(void);
        SMwMemberInfo** m_MemberInfos;
        Natural m_MemberCount;
};

#endif // __GBXTOOLS_H__
