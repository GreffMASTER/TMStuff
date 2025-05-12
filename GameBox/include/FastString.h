#ifndef __FASTSTRING_H__
#define __FASTSTRING_H__

class CFastString
{
    public:
        virtual Delete(byte flags);
        Natural m_Size;
        char* m_Str;

        static CFastString* NewCFastString(char* text)
        {
            CFastString* str = (CFastString*)malloc(sizeof(CFastString));
            CFastString* (__thiscall* callable)(CFastString*, char*) = (CFastString* (__thiscall*)(CFastString*, char*))0x004186c0;
            return callable(str, text);
        }
};

#endif // __FASTSTRING_H__
