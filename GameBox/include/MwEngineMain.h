#ifndef MWENGINEMAIN_H_INCLUDED
#define MWENGINEMAIN_H_INCLUDED

#include "MwEngine.h"

class CMwEngineMain : public CMwEngine
{
    public:
        CFastArray<CMwEngine*> m_Engines;
        Natural m_EngineCountUO;
        CMwEngine** m_EnginesUO;
        int u3;

        static void AddEngine(CMwEngineMain* main, int engineId, CMwEngine* engine)
        {
            void (__thiscall* add_engine)(CMwEngineMain* main, int engineId, CMwEngine* engine) = (void (__thiscall* )(CMwEngineMain* main, int engineId, CMwEngine* engine))0x004d7250;
            add_engine(main, engineId, engine);
            return;
        }
};

#endif // MWENGINEMAIN_H_INCLUDED
