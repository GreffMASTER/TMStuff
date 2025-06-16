#ifndef __GBXTOOLS_H__
#define __GBXTOOLS_H__

#include <iostream>
#include <string>
#include <sstream>
#include "GbxTypes.h"
#include "../../imgui/imgui.h"
#include <d3d9.h>

#define GBX_ISTEXT   0b0001
#define GBX_COMPRESS 0b0010
#define GBX_EMBEDDED 0b0100
#define GBX_ISR      0b1000

extern void** vtable_CMwNod;

namespace GbxTools
{
    LPDIRECT3D9 GetD3D9();
    LPDIRECT3DDEVICE9 GetD3DDevice9();
    CMwNodMain * GetMainNod();
    CMwEngineMain * GetEngineMain();
    CMwEngineManager * GetEngineManager();
    CGameProcess * GetTrackManiaNod();
    SFastBuffer<CFastString> * GetLookbackStrings();
    CMwClassInfo * GetClassInfoByMwClassId(int class_id);
    void CreateMwClassInfo(CMwClassInfo* new_class, int class_id, CMwClassInfo *parent_class, char *class_name, void *constructor);
    SMwMemberInfo* GetMemberInfoById(CMwNod* nod, int member_id);
    void PrintNodInfo(CMwNod* nod);
    CMwId* GetMwId(CMwId* other, char* str);
    CFastString* GetStringById(CMwId* id);

    int LoadNod(CMwNod** nod, char* path);
    int LoadNod2(CMwNod** nod, char* path);
    int SaveNod(CMwNod* nod, char* path, int flags);
    int SaveNod2(CMwNod* nod, char* path, int flags);

    int MwDestroy(CMwNod* nod);
    CMwNod* CreateByMwClassId(int id);
    CMwNod* CreateByMwClassInfo(CMwClassInfo* classinfo);
    bool MwCheckThis(CMwNod* nod, int nod_size); // returns true if is oke
    void IncrementRefCount(CMwNod* nod);
    int VirtualParam_Get_Fast(CMwNod* nod, int member_id, void** value); // returns 0 if ok
    int VirtualParam_Get_Fast(CMwNod* nod, SMwMemberInfo* member_info, void** value); // returns 0 if ok
    int VirtualParam_Set_Fast(CMwNod* nod, int member_id, void** value); // returns 0 if ok
    int VirtualParam_Set_Fast(CMwNod* nod, SMwMemberInfo* member_info, void** value); // returns 0 if ok
    int VirtualParam_Set_Fast(CMwNod* nod, SMwMemberInfo* member_info, void** value, int index); // returns 0 if ok

    int VirtualParam_Set_SuperFast(CMwNod* nod, void** value, int argc, ...); // returns 0 if ok

    int VirtualParam_Add_Fast(CMwNod* nod, SMwMemberInfo* member_info, void** value); // returns 0 if ok

    int VirtualParam_Add_SuperFast(CMwNod* nod, void** value, int argc, ...); // returns 0 if ok

    int VirtualParam_Sub_Fast(CMwNod* nod, SMwMemberInfo* member_info, void** value); // returns 0 if ok
    bool SetTreePickableRecursive(CMwNod* tree);

    void SetMwMemberInfo(
        SMwMemberInfo* member_info,
        SMwMemberInfo::eType member_type,
        int member_id,
        int member_offset,
        char* member_name1,
        char* member_name2,
        char* member_name3,
        int member_flags
    );

    void SetMwMemberInfo(
        SMwClassMemberInfo* member_info,
        int member_id,
        int member_offset,
        char* member_name1,
        char* member_name2,
        char* member_name3,
        int member_flags,
        CMwClassInfo* member_class
    );

    void SetMwMemberInfo(
        SMwActionInfo* member_info,
        int member_id,
        char* member_name1,
        char* member_name2,
        char* member_name3,
        int member_flags,
        void* member_method
    );

    void SetMwMemberInfo(
        SMwMemberInfoRealRange* member_info,
        int member_id,
        int member_offset,
        char* member_name1,
        char* member_name2,
        char* member_name3,
        int member_flags,
        float value_min,
        float value_max
    );
}

#endif // __GBXTOOLS_H__
