#include "../include/GbxTools.h"

const int NODMAIN       = 0x0097b108;
const int GAMED3D9      = 0x00988074;
const int LOOKBACKS     = 0x00980600; // SFastBuffer<CFastString>**
const int ENGINEMAIN    = 0x009804b8; // CMwEngineMain**
const int ENGINEMAN     = 0x00980494; // CMwEngineManager*

FILE* pLogFile = NULL;

void** vtable_CMwNod = (void**)(0x0087ec60);

LPDIRECT3D9 GbxTools::GetD3D9()
{
    LPDIRECT3D9* dx9 = (LPDIRECT3D9*)GAMED3D9;
    return *dx9;
}

LPDIRECT3DDEVICE9 GbxTools::GetD3DDevice9()
{
    CMwNodMain* main = GbxTools::GetMainNod();
    if(main == nullptr)
        return nullptr;
    if(main->m_Viewport == nullptr)
        return nullptr;
    int* viewport = (int*)main->m_Viewport;                         // :monkaW:
    LPDIRECT3DDEVICE9* dev = (LPDIRECT3DDEVICE9*)viewport + 403;    //
    return *dev;
}

CMwNodMain * GbxTools::GetMainNod()
{
    CMwNodMain * nodmain = (CMwNodMain*)NODMAIN;
    return nodmain;
}

CMwEngineMain * GbxTools::GetEngineMain()
{
    CMwEngineMain ** enginemain = (CMwEngineMain **)ENGINEMAIN;
    return *enginemain;
}

CMwEngineManager * GbxTools::GetEngineManager()
{
    CMwEngineManager* engine_man = (CMwEngineManager*)ENGINEMAN;
    return engine_man;
}

CGameProcess * GbxTools::GetTrackManiaNod()
{
    CMwNodMain * nodmain = (CMwNodMain*)NODMAIN;
    return nodmain->m_Game;
}

void GbxTools::PrintNodInfo(CMwNod* nod)
{
    CMwClassInfo* class_info = nod->MwGetClassInfo();
    void* id = nod->MwGetId();
    printf("-==================-\n");
    printf("ClassId: %08X (%s)\n", nod->GetMwClassId(), class_info->m_ClassName);
    printf("Id: %08X\n", id);
    printf("@ %08X\n", nod);
}

CMwId* GbxTools::GetMwId(CMwId* other, char* str)
{

    CMwId* (__cdecl* callable)(CMwId* in_other, char* in_str) = (CMwId* (__cdecl* )(CMwId* in_other, char* in_str))0x0043ad70; // Call function at this address
    //printf("calling\n");
    return callable(other, str);
}

CFastString* GbxTools::GetStringById(CMwId* id)
{
    CFastString* (__fastcall* callable)(CMwId* in_id) = (CFastString* (__fastcall* )(CMwId* in_id))0x004dd5d0; // Call function at this address
    return callable(id);
}

SFastBuffer<CFastString>* GbxTools::GetLookbackStrings()
{
    SFastBuffer<CFastString>** buf = (SFastBuffer<CFastString>**)LOOKBACKS;
    return *buf;
}

CMwClassInfo * GbxTools::GetClassInfoByMwClassId(int class_id)
{
    CMwEngineManager* engine_man = (CMwEngineManager*)ENGINEMAN;
	for (int i = 0; i < engine_man->m_Engines.numElems; i++) { // for each engine
		CMwEngineInfo* engineinfo = engine_man->m_Engines.pElems[i];
		if (engineinfo != NULL) {
			for (int j = 0; j < engineinfo->m_Classes.numElems; j++) { // for each class
				CMwClassInfo* classinfo = engineinfo->m_Classes.pElems[j];
				if (classinfo != NULL) {
					if (classinfo->m_ClassId == class_id) {
						return classinfo;
					}
				}
			}
		}
	}
	return NULL;
}

void GbxTools::CreateMwClassInfo(CMwClassInfo* new_class, int class_id, CMwClassInfo *parent_class, char *class_name, void *constructor)
{
    printf("Preparing new class \"%s\", id %08X...\n", class_name, class_id);
    new_class->m_ClassId = class_id;
    new_class->m_Constructor = (CMwNod* (__stdcall*)(void))constructor;
    new_class->m_ParentClass = parent_class;
    new_class->m_ClassName = class_name;
    new_class->m_MemberInfos = nullptr;
    new_class->m_MemberCount = 0;
    printf("Setting vtable...\n");
    void** new_class_vtable = (void**)new_class;
    new_class_vtable[0] = (void**)0x00880144;
    printf("Created \"%s\"!\n", class_name);
}

SMwMemberInfo * GbxTools::GetMemberInfoById(CMwNod* nod, int member_id)
{
    CMwClassInfo* class_info = nod->MwGetClassInfo();
    while(class_info != nullptr) {
        for(int i=0;i<class_info->m_MemberCount;i++) {
            SMwMemberInfo* member = class_info->m_MemberInfos[i];
            if(member->memberId == member_id)
                return member;
        }
        class_info = class_info->m_ParentClass;
    }
    return nullptr;
}

CSystemFidsDrive* GbxTools::GetGameDataDrive()
{
    CSystemFidsDrive* (__fastcall* get_drive)(CMwEngine* in_engine) = (CSystemFidsDrive* (__fastcall* )(CMwEngine* in_engine))0x00616e30;
    CMwEngineMain* e = GbxTools::GetEngineMain();
    CMwEngine* s = e->m_Engines.pElems[0xb];
    return get_drive(s);

}

int GbxTools::LoadNod(CMwNod** nod, char* path)
{
    // define functions

    int (__cdecl* load)( char* path, CMwNod** nod, CSystemFidsDrive* drive, int earchive, int u1) =
        (int (__cdecl* )( char* path, CMwNod** nod, CSystemFidsDrive* drive, int earchive, int u1))0x00459460;

    // rest
    int res = 0;


    CSystemFidsDrive* drive = GbxTools::GetGameDataDrive();

    printf("Loading nod from \"%s\"...\n", path);
    load(path, nod, drive, 7, 1);
    if(*nod)
        res = 1;
    printf("Result: %i\n", res);
    return res; // 1 = success
}

int GbxTools::LoadNod2(CMwNod** nod, char* path)
{
    int (__cdecl* load)( char* path, CMwNod** nod, CSystemFid* drive, int earchive, int u1) =
    (int (__cdecl* )( char* path, CMwNod** nod, CSystemFid* drive, int earchive, int u1))0x00459460;

    // rest
    int res = 0;

    printf("Loading nod from \"%s\"...\n", path);
    load(path, nod, NULL, 7, 1);
    if(*nod)
        res = 1;
    printf("Result: %i\n", res);
    return res; // 1 = success
}

int GbxTools::SaveNod(CMwNod* nod, char* path, int flags)
{
    // define functions
    CSystemFid* (__fastcall* get_drive)(CMwEngine* in_engine) = (CSystemFid* (__fastcall* )(CMwEngine* in_engine))0x00616e30;
    void (__thiscall* create_archive)(CMwNod*) = (void (__thiscall* )(CMwNod*))0x00618280;
    int (__thiscall* save)( CMwNod*,char* path, CMwNod* nod, CSystemFid* drive, int flags, int earchive, int u1, int u2) =
    (int (__thiscall* )(CMwNod*, char* path, CMwNod* nod, CSystemFid* drive, int flags, int earchive, int u1, int u2))0x0061ab40;

    // rest
    int res = 0;
    CMwEngineMain* e = GbxTools::GetEngineMain();
    CMwEngine* s = e->m_Engines.pElems[0xb];
    CSystemFid* drive = get_drive(s);
    CMwNod* archive = (CMwNod*)malloc(0x200);
    create_archive(archive);
    printf("Saving nod to \"%s\" with flags %08X...\n", path, flags);
    res = save(archive, path, nod, drive, flags, 7, 1, 1);
    printf("Result: %i\n", res);
    delete archive;
    return res; // 1 = success
}

int GbxTools::SaveNod2(CMwNod* nod, char* path, int flags)
{
    // define functions
    void (__thiscall* create_archive)(CMwNod*) = (void (__thiscall* )(CMwNod*))0x00618280;
    int (__thiscall* save)( CMwNod*,char* path, CMwNod* nod, CSystemFid* drive, int flags, int earchive, int u1, int u2) =
    (int (__thiscall* )(CMwNod*, char* path, CMwNod* nod, CSystemFid* drive, int flags, int earchive, int u1, int u2))0x0061ab40;

    // rest
    int res = 0;
    CMwNod* archive = (CMwNod*)malloc(0x200);
    create_archive(archive);
    printf("Saving nod to \"%s\"...\n", path);
    res = save(archive, path, nod, NULL, flags, 7, 1, 1);
    printf("Result: %i\n", res);
    delete archive;
    return res; // 1 = success
}

int GbxTools::ReSaveNod(CMwNod* nod)
{
    // define functions
    void (__thiscall* create_archive)(CMwNod*) = (void (__thiscall* )(CMwNod*))0x00618280;
    int (__thiscall* resave)(CMwNod*,CMwNod*) = (int (__thiscall* )(CMwNod*,CMwNod*))0x00619ef0;

    // rest
    int res = 0;
    CMwNod* archive = (CMwNod*)malloc(0x200);
    create_archive(archive);
    printf("ReSaving nod...\n");
    res = resave(archive, nod);
    printf("Result: %i\n", res);
    delete archive;
    return res; // 1 = success
}

int GbxTools::LoadFromFid(CMwNod** ppNod, CSystemFid* pFid, int eArchive)
{
    int (__cdecl* load_from_fid)(CMwNod**, CSystemFid*, int) = (int (__cdecl* )(CMwNod**, CSystemFid*, int))0x00421820;
    printf("Calling LoadFromFid...\n");
    return load_from_fid(ppNod, pFid, eArchive);
}

int GbxTools::MwDestroy(CMwNod* nod)
{
    int (__fastcall* destroy)(CMwNod* in_nod) = (int (__fastcall* )(CMwNod* in_nod))0x004d6680;
    return destroy(nod);
}

CMwNod* GbxTools::CreateByMwClassId(int id)
{

    CMwClassInfo* classinfo = GetClassInfoByMwClassId(id);
    if(!classinfo)
    {
        printf("Could not find engine for class %08X\n", id);
        return nullptr;
    }
    if(classinfo->m_Constructor == nullptr){
        printf("Could not find constructor for class %08X\n", id);
        return nullptr;
    }
    printf("Creating %08X\n", id);
    CMwNod* nod = classinfo->m_Constructor();

	printf("%08X\n", nod);
	return nod;
}

CMwNod* GbxTools::CreateByMwClassInfo(CMwClassInfo* classinfo)
{
    if(classinfo->m_Constructor == nullptr){
        printf("Could not find constructor for class %s\n", classinfo->m_ClassName);
        return nullptr;
    }
    printf("Creating %s\n", classinfo->m_ClassName);
    CMwNod* nod = classinfo->m_Constructor();

	printf("%08X\n", nod);
	return nod;
}

bool GbxTools::MwCheckThis(CMwNod* nod, int nod_size) // returns true if valid
{
    if(!nod)
        return false;
    BOOL ok = IsBadWritePtr(nod, nod_size); // returns 0 if succ
    if(ok != 0)
        return false;
    ok = IsBadReadPtr(nod, 4);
    if(ok != 0)
        return false;
    FARPROC* lp = (FARPROC*)nod;
    ok = IsBadCodePtr(*lp);
    return (bool)(1 - (ok != 0));
}

void GbxTools::IncrementRefCount(CMwNod* nod)
{
    void (__fastcall* increment_ref_count)(CMwNod* in_nod) = (void (__fastcall* )(CMwNod* in_nod))0x004d6670;
    increment_ref_count(nod);
    return;
}

int GbxTools::VirtualParam_Get_Fast(CMwNod* nod, int member_id, void** value)
{
    int result = 1;
    // prepare the stacc
    CMwStack* stacc = (CMwStack*)malloc(sizeof(CMwStack));
    if(stacc == nullptr)
        return 1;
    stacc->m_Size = 1;
    stacc->ppMemberInfos = (SMwMemberInfo**)malloc(sizeof(SMwMemberInfo*) * stacc->m_Size);
    SMwMemberInfo* member_info = GbxTools::GetMemberInfoById(nod, member_id);
    if(member_info) {
        stacc->ppMemberInfos[0] = member_info;
        stacc->iCurrentPos = 0;
        result = nod->VirtualParam_Get(stacc, value);
    }
    free(stacc);
    return result;
}

int GbxTools::VirtualParam_Get_Fast(CMwNod* nod, SMwMemberInfo* member_info, void** value)
{
    int result = 1;
    // prepare the stacc
    CMwStack* stacc = (CMwStack*)malloc(sizeof(CMwStack));
    if(stacc == nullptr)
        return 1;
    stacc->m_Size = 1;
    stacc->ppMemberInfos = (SMwMemberInfo**)malloc(sizeof(SMwMemberInfo*) * stacc->m_Size);
    if(member_info) {
        stacc->ppMemberInfos[0] = member_info;
        stacc->iCurrentPos = 0;
        result = nod->VirtualParam_Get(stacc, value);
    }
    free(stacc);
    return result;
}

int GbxTools::VirtualParam_Set_Fast(CMwNod* nod, int member_id, void** value)
{
    int result = 1;
    // prepare the stacc
    CMwStack* stacc = (CMwStack*)malloc(sizeof(CMwStack));
    if(stacc == nullptr)
        return 1;
    stacc->m_Size = 1;
    stacc->ppMemberInfos = (SMwMemberInfo**)malloc(sizeof(SMwMemberInfo*) * stacc->m_Size);
    SMwMemberInfo* member_info = GbxTools::GetMemberInfoById(nod, member_id);
    if(member_info) {
        stacc->ppMemberInfos[0] = member_info;
        stacc->iCurrentPos = 0;
        result = nod->VirtualParam_Set(stacc, value);
    }
    free(stacc);
    return result;
}

int GbxTools::VirtualParam_Set_Fast(CMwNod* nod, SMwMemberInfo* member_info, void** value)
{
    int result = 1;
    // prepare the stacc
    CMwStack* stacc = (CMwStack*)malloc(sizeof(CMwStack));
    if(stacc == nullptr)
        return 1;
    stacc->m_Size = 1;
    stacc->ppMemberInfos = (SMwMemberInfo**)malloc(sizeof(SMwMemberInfo*) * stacc->m_Size);
    if(member_info) {
        stacc->ppMemberInfos[0] = member_info;
        stacc->iCurrentPos = 0;
        result = nod->VirtualParam_Set(stacc, value);
    }
    free(stacc);
    return result;
}

int GbxTools::VirtualParam_Set_Fast(CMwNod* nod, SMwMemberInfo* member_info, void** value, int index)
{
    int result = 1;
    // prepare the stacc
    CMwStack* stacc = (CMwStack*)malloc(sizeof(CMwStack));
    if(stacc == nullptr)
        return 1;
    stacc->m_Size = 2;
    stacc->ppMemberInfos = (SMwMemberInfo**)malloc(sizeof(SMwMemberInfo*) * stacc->m_Size);
    if(member_info) {
        stacc->ppMemberInfos[0] = member_info;
        stacc->ppMemberInfos[1] = (SMwMemberInfo*)&index;
        stacc->iCurrentPos = 1;
        result = nod->VirtualParam_Set(stacc, value);
    }
    free(stacc);
    return result;
}

int GbxTools::VirtualParam_Set_SuperFast(CMwNod* nod, void** value, int argc, ...)
{
    printf("Calling Set super fast!\n");
    va_list argptr;
    int result = 1;

    // prepare the stacc
    CMwStack* stacc = (CMwStack*)malloc(sizeof(CMwStack));
    if(stacc == nullptr)
        return 1;
    stacc->m_Size = argc;

    stacc->ppMemberInfos = (SMwMemberInfo**)malloc(sizeof(SMwMemberInfo*) * stacc->m_Size);

    va_start(argptr, argc);
    for(int i=0;i<stacc->m_Size;i++) {
        printf("Preparing arg no. %i\n", i);
        SMwMemberInfo* info = va_arg(argptr, SMwMemberInfo*);
        printf("%08X\n", info);
        stacc->ppMemberInfos[i] = info;
    }
    stacc->iCurrentPos = stacc->m_Size - 1;
    printf("size: %i, position: %i\n", stacc->m_Size, stacc->iCurrentPos);
    result = nod->VirtualParam_Set(stacc, value);

    va_end(argptr);
    free(stacc->ppMemberInfos);
    free(stacc);

    return result;
}

int GbxTools::VirtualParam_Add_SuperFast(CMwNod* nod, void** value, int argc, ...)
{
    printf("Calling Add super fast!\n");
    va_list argptr;
    int result = 1;

    // prepare the stacc
    CMwStack* stacc = (CMwStack*)malloc(sizeof(CMwStack));
    if(stacc == nullptr)
        return 1;
    stacc->m_Size = argc;

    stacc->ppMemberInfos = (SMwMemberInfo**)malloc(sizeof(SMwMemberInfo*) * stacc->m_Size);

    va_start(argptr, argc);
    for(int i=0;i<stacc->m_Size;i++) {
        printf("Preparing arg no. %i\n", i);
        SMwMemberInfo* info = va_arg(argptr, SMwMemberInfo*);
        printf("%08X\n", info);
        stacc->ppMemberInfos[i] = info;
    }
    stacc->iCurrentPos = stacc->m_Size - 1;
    printf("size: %i, position: %i\n", stacc->m_Size, stacc->iCurrentPos);
    result = nod->VirtualParam_Add(stacc, value);


    free(stacc->ppMemberInfos);
    free(stacc);

    return result;
}

int GbxTools::VirtualParam_Add_Fast(CMwNod* nod, SMwMemberInfo* member_info, void** value)
{
    int result = 1;
    // prepare the stacc
    CMwStack* stacc = (CMwStack*)malloc(sizeof(CMwStack));
    if(stacc == nullptr)
        return 1;
    stacc->m_Size = 1;
    stacc->ppMemberInfos = (SMwMemberInfo**)malloc(sizeof(SMwMemberInfo*) * stacc->m_Size);
    if(member_info) {
        stacc->ppMemberInfos[0] = member_info;
        stacc->iCurrentPos = 0;
        result = nod->VirtualParam_Add(stacc, value);
    }
    free(stacc);
    return result;
}

int GbxTools::VirtualParam_Sub_Fast(CMwNod* nod, SMwMemberInfo* member_info, void** value)
{
    int result = 1;
    // prepare the stacc
    CMwStack* stacc = (CMwStack*)malloc(sizeof(CMwStack));
    if(stacc == nullptr)
        return 1;
    stacc->m_Size = 1;
    stacc->ppMemberInfos = (SMwMemberInfo**)malloc(sizeof(SMwMemberInfo*) * stacc->m_Size);
    if(member_info) {
        stacc->ppMemberInfos[0] = member_info;
        stacc->iCurrentPos = 0;
        result = nod->VirtualParam_Sub(stacc, value);
    }
    free(stacc);
    return result;
}

bool GbxTools::SetTreePickableRecursive(CMwNod* tree)
{
    // Set this tree
    bool toggle = true;
    if(GbxTools::VirtualParam_Set_Fast(tree, 0x0904f009, (void**)&toggle))
        printf("Failed to set IsPickable for %08X\n", tree);
    // Handle mip levels
    if(tree->MwIsKindOf(0x09015000)) {
        CFastArray<CMwNod*>* Mips = (CFastArray<CMwNod*>*)((int)tree + 0xbc);
        if(Mips == nullptr)
            return false;
        for(int i=0;i<Mips->numElems;i++) {
            CMwNod* Mip = (CMwNod*)Mips->pElems[i];
            if(!GbxTools::MwCheckThis(Mip, sizeof(CMwNod*)))
                continue;
            GbxTools::SetTreePickableRecursive(Mip);
        }
    }
    // Handle childs
    SFastBuffer<CMwNod*>* Childs = (SFastBuffer<CMwNod*>*)((int)tree + 0x28);
    if(Childs == nullptr)
        return false;

    for(int i=0;i<Childs->numElems;i++) {
        CMwNod* Child = (CMwNod*)Childs->pElems[i];
        if(!GbxTools::MwCheckThis(Child, sizeof(CMwNod*)))
            continue;
        GbxTools::SetTreePickableRecursive(Child);
    }



    return true;
}

void GbxTools::SetMwMemberInfo(
        SMwMemberInfo* member_info,
        SMwMemberInfo::eType member_type,
        int member_id,
        int member_offset,
        char* member_name1,
        char* member_name2,
        char* member_name3,
        int member_flags
)
{
    member_info->type = member_type;
    member_info->memberId = member_id;
    member_info->fieldOffset = member_offset;
    member_info->pszName = member_name1;
    member_info->pszName2 = member_name2;
    member_info->pszName3 = member_name3;
    member_info->flags = member_flags;
    member_info->flags2 = 0x3F80;
    member_info->pParam = NULL;
}

void GbxTools::SetMwMemberInfo(
        SMwClassMemberInfo* member_info,
        int member_id,
        int member_offset,
        char* member_name1,
        char* member_name2,
        char* member_name3,
        int member_flags,
        CMwClassInfo* member_class
)
{
    member_info->type = SMwMemberInfo::eType::CLASS;
    member_info->memberId = member_id;
    member_info->fieldOffset = member_offset;
    member_info->pszName = member_name1;
    member_info->pszName2 = member_name2;
    member_info->pszName3 = member_name3;
    member_info->flags = member_flags;
    member_info->flags2 = 0x3F80;
    member_info->pClassInfo = member_class;
    member_info->pParam = NULL;
}

void GbxTools::SetMwMemberInfo(
        SMwActionInfo* member_info,
        int member_id,
        char* member_name1,
        char* member_name2,
        char* member_name3,
        int member_flags,
        void* member_method
)
{
    member_info->type = SMwMemberInfo::eType::ACTION;
    member_info->memberId = member_id;
    member_info->pParam = NULL;
    member_info->fieldOffset = 0xFFFFFFFF;
    member_info->pszName = member_name1;
    member_info->pszName2 = member_name2;
    member_info->pszName3 = member_name3;
    member_info->flags = member_flags;
    member_info->flags2 = 0x3F80;
    member_info->pFun = (void (__fastcall*)(CMwNod* nod))member_method;
}

void GbxTools::SetMwMemberInfo(
        SMwMemberInfoRealRange* member_info,
        int member_id,
        int member_offset,
        char* member_name1,
        char* member_name2,
        char* member_name3,
        int member_flags,
        float value_min,
        float value_max
)
{
    member_info->type = SMwMemberInfo::REALRANGE;
    member_info->memberId = member_id;
    member_info->fieldOffset = member_offset;
    member_info->pParam = NULL;
    member_info->pszName = member_name1;
    member_info->pszName2 = member_name2;
    member_info->pszName3 = member_name3;
    member_info->flags = member_flags;
    member_info->flags2 = 0x3F80;
    member_info->min = value_min;
    member_info->max = value_max;
}
