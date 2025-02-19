#ifndef FASTARRAY_H_INCLUDED
#define FASTARRAY_H_INCLUDED

template <typename T>
struct CFastArray
{
    void** vftable;
    Natural numElems;
    T* pElems;
};

template <typename T>
struct SFastArray
{
    Natural numElems;
    T* pElems;
};

#endif // FASTARRAY_H_INCLUDED
