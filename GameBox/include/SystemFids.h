#ifndef SYSTEMFIDS_H_INCLUDED
#define SYSTEMFIDS_H_INCLUDED

#include "GbxTypes.h"

class CSystemFids : public CMwNod
{
    public:
        Natural u2;
        Natural u3;
        CSystemFids* m_ParentFids;
        SFastBuffer<CSystemFid*> m_Leaves;
        SFastBuffer<CSystemFids*> m_Trees;
};

#endif // SYSTEMFIDS_H_INCLUDED
