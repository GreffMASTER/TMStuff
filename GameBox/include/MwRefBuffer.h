#ifndef MWREFBUFFER_H_INCLUDED
#define MWREFBUFFER_H_INCLUDED

#include "MwNod.h"

class CMwRefBuffer : public CMwNod
{
    public:
        SFastBuffer<CMwNod*> m_Nods;
        Natural u2;
        Natural m_NodsClassId;
};

#endif // MWREFBUFFER_H_INCLUDED
