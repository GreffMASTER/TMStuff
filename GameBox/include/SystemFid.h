#ifndef SYSTEMFID_H_INCLUDED
#define SYSTEMFID_H_INCLUDED

#include "MwNod.h"

class CSystemFids;

class CSystemFid : public CMwNod
{
    public:
        Natural u2;
        Natural flags;
        CSystemFids* m_ParentFids;
        Natural u3;
        Natural u4;
        Natural m_ClassId;
        CMwNod* m_Nod;
        Bool bool1;
};

#endif // SYSTEMFID_H_INCLUDED
