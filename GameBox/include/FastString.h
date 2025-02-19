#ifndef __FASTSTRING_H__
#define __FASTSTRING_H__

class CFastString
{
    public:
        virtual ~CFastString();
        Natural m_Size;
        char* m_Str;
};

#endif // __FASTSTRING_H__
