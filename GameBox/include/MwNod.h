#ifndef __MWNOD_H__
#define __MWNOD_H__

#include "GbxTypes.h"

class CMwNod
{
    public:
        virtual Delete(byte flags);
        virtual CMwClassInfo* MwGetClassInfo(void);
        virtual Natural GetMwClassId(void);
        virtual bool MwIsKindOf(Natural classId);
        virtual CMwId* MwGetId(void);
        virtual void SetIdName(char* name);
        virtual void MwIsKilled(void);
        virtual void MwIsUnreferenced(void);
        virtual int VirtualParam_Get(CMwStack* stacc, void ** out); // 0 succ, 1 not succ
        virtual int VirtualParam_Set(CMwStack* stacc, void ** in); // 0 succ, 1 not succ
        virtual int VirtualParam_Add(CMwStack* stacc, void ** in); // 0 succ, 1 not succ
        virtual int VirtualParam_Sub(CMwStack* stacc, void ** in); // 0 succ, 1 not succ
        virtual void FUN_004d6c40(void);
        virtual Natural GetUidParamFromIndex(Natural index);
        virtual Natural GetParamCount(void);
        virtual Natural Archive(void);
        virtual Natural Chunk(void);
        virtual Natural GetChunkInfo(Natural chunkId);
        virtual Natural GetUidChunkFromIndex(Natural index);
        virtual Natural GetChunkCount(void);
        virtual void OnNodLoaded(void);
        virtual void CreateDefaultData(void);
        // TODO add the rest of virtual functions
        virtual void FUN2(void);
        virtual void FUN3(void);
        virtual void FUN4(void);
        virtual void FUN5(void);
        virtual Natural HeaderChunk(void);
        virtual Natural GetUidHeaderChunkFromIndex(Natural index);
        virtual Natural GetHeaderChunkCount(void);
        virtual void FUN6(void);
        virtual void FUN7(void);
        virtual void FUN8(void);

        Natural m_ReferenceCount;
        CSystemFid* m_SystemFid;
        SFastBuffer<CMwNod*>* m_Dependants;
        Natural u1;

        static int MwRelease(CMwNod* nod)
        {
            int (__thiscall* callable)(CMwNod*) = (int (__thiscall*)(CMwNod*))0x004d6680;
            return callable(nod);
        }

        static int Param_Get(CMwNod* nod, CMwStack* stacc, void** value)
        {
            int (__thiscall* callable)(CMwNod*, CMwStack*, void**) = (int (__thiscall*)(CMwNod*, CMwStack*, void**))0x004d6cb0;
            return callable(nod, stacc, value);
        }

        static int Param_Set(CMwNod* nod, CMwStack* stacc, void** value)
        {
            int (__thiscall* callable)(CMwNod*, CMwStack*, void**) = (int (__thiscall*)(CMwNod*, CMwStack*, void**))0x004d6db0;
            return callable(nod, stacc, value);
        }

        static int Param_Add(CMwNod* nod, CMwStack* stacc, void** value)
        {
            int (__thiscall* callable)(CMwNod*, CMwStack*, void**) = (int (__thiscall*)(CMwNod*, CMwStack*, void**))0x004d6e20;
            return callable(nod, stacc, value);
        }

        static int Param_Sub(CMwNod* nod, CMwStack* stacc, void** value)
        {
            int (__thiscall* callable)(CMwNod*, CMwStack*, void**) = (int (__thiscall*)(CMwNod*, CMwStack*, void**))0x004d6e70;
            return callable(nod, stacc, value);
        }

        static int Param_Check(CMwNod* nod, CMwStack* stacc)
        {
            int (__thiscall* callable)(CMwNod*, CMwStack*) = (int (__thiscall*)(CMwNod*, CMwStack*))0x004d6ec0;
            return callable(nod, stacc);
        }

};

#endif // __MWNOD_H__
