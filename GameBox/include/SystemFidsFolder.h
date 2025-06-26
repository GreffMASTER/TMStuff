#ifndef SYSTEMFIDSFOLDER_H_INCLUDED
#define SYSTEMFIDSFOLDER_H_INCLUDED

#include "SystemFids.h"

class CSystemFidsFolder : public CSystemFids
{
    public:
        static CFastString* GetFolderName(CSystemFidsFolder* folder)
        {
            CFastString* str = (CFastString*)((int)folder + 0x0000005c);
            return str;
        }
};

#endif // SYSTEMFIDSFOLDER_H_INCLUDED
