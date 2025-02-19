#ifndef MWENGINEINFO_H_INCLUDED
#define MWENGINEINFO_H_INCLUDED

#include "GbxTypes.h"

class CMwEngineInfo
{
    public:
        pFun* vftable;

        int m_EngineId;
        char* m_EngineName;
        pFun* u1;
        SFastArray<CMwClassInfo*> m_Classes;


};

#endif // MWENGINEINFO_H_INCLUDED
