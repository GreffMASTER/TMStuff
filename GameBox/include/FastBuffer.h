#ifndef FASTBUFFER_H_INCLUDED
#define FASTBUFFER_H_INCLUDED

template <typename T>
struct SFastBuffer
{
    Natural numElems;
    T* pElems;
    Natural cap;
};

#endif // FASTBUFFER_H_INCLUDED
