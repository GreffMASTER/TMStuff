#ifndef MWENGINEMANAGER_H_INCLUDED
#define MWENGINEMANAGER_H_INCLUDED

#include "GbxTypes.h"

class CMwEngineManager
{
    public:
        pFun* vftable;

        CMwClassInfo* u1;
        CMwClassInfo* u2;
        pFun* u3;
        SFastArray<CMwEngineInfo*> m_Engines;

        static void AddClass(CMwEngineManager *man, CMwClassInfo *classInfo)
        {
            void (__thiscall* add_class)(CMwEngineManager *man, CMwClassInfo *classInfo) = (void (__thiscall* )(CMwEngineManager *man, CMwClassInfo *classInfo))0x004e5960;
            add_class(man, classInfo);
        }
};

#endif // MWENGINEMANAGER_H_INCLUDED
