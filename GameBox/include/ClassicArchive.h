#ifndef CLASSICARCHIVE_H_INCLUDED
#define CLASSICARCHIVE_H_INCLUDED

#include "GbxTypes.h"

class CClassicArchive
{
    public:
        virtual Delete(void);
        virtual void DoNod(CMwNod** nod);
        virtual void DoFid(CMwNod** nod);

        void* Buffer;
        Bool m_IsWriting;
        Bool m_IsTextMode;
        Bool m_IsR;
        Bool m_IsCompressed;
        // TODO fill the rest xdd
};

#endif // CLASSICARCHIVE_H_INCLUDED
