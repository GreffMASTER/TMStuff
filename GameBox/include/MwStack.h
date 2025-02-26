#ifndef MWSTACK_H_INCLUDED
#define MWSTACK_H_INCLUDED

#include "MwNod.h"
#include "MwMemberInfo.h"

// TODO figure out how to properly use this thing

class CMwStack : public CMwNod {
    public:
        int u2;
        int m_Size;
        SMwMemberInfo** ppMemberInfos;
        int* ppTypes;
        int iCurrentPos;
};

// NOTES
// type 0 = CMwMemberInfo*(?)
// type 2 = MwId(?)


#endif // MWSTACK_H_INCLUDED
