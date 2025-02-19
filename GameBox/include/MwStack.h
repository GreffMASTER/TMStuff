#ifndef MWSTACK_H_INCLUDED
#define MWSTACK_H_INCLUDED

#include "MwNod.h"
#include "MwMemberInfo.h"

class CMwStack : public CMwNod {
    public:
        int u2;
        int m_Size;
        SMwMemberInfo** ppMemberInfos;
        void** ppTypes;
        int iCurrentPos;
};

#endif // MWSTACK_H_INCLUDED
