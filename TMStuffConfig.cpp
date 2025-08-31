#include "TMStuffConfig.h"
#include <errno.h>

CMwClassInfo TMStuff::CTMStuffConfig::m_MwClassInfo_TMStuffConfig;

SMwClassMemberInfo TMStuffConfig_ImGuiStyle;
SMwMemberInfo TMStuffConfig_ShowUi;
SMwMemberInfo TMStuffConfig_ShowTM;
SMwMemberInfo TMStuffConfig_ShowResave;
SMwMemberInfo TMStuffConfig_ShowPicker;
SMwMemberInfo TMStuffConfig_ShowConfig;
SMwMemberInfo TMStuffConfig_ShowScreenShot;

void TMStuff::CTMStuffConfig::Init()
{
    printf("Creating TMStuffConfig class...\n");

    CMwClassInfo* MwClassInfo_CMwNod = GbxTools::GetClassInfoByMwClassId(0x01001000);
    printf("Creating ClassInfo for TMStuff...\n");
    GbxTools::CreateMwClassInfo(&m_MwClassInfo_TMStuffConfig, 0x48001000, MwClassInfo_CMwNod, "CTMStuffConfig", (void*)MwNewCTMStuffConfig);

    m_MwClassInfo_TMStuffConfig.m_MemberCount = 6;
    m_MwClassInfo_TMStuffConfig.m_MemberInfos = (SMwMemberInfo**)malloc(sizeof(SMwMemberInfo*) * m_MwClassInfo_TMStuffConfig.m_MemberCount);

    GbxTools::SetMwMemberInfo(
        &TMStuffConfig_ImGuiStyle,
        0x48001000,
        offsetof(TMStuff::CTMStuffConfig, m_ImGuiStyle),
        "ImGuiStyle", "m_ImGuiStyle", "ImGuiStyle",
        SMwMemberInfo::eFlags::GET | SMwMemberInfo::eFlags::SET,
        &TMStuff::CTMStuffImGuiStyle::m_MwClassInfo_TMStuffImGuiStyle
    );
    m_MwClassInfo_TMStuffConfig.m_MemberInfos[0] = &TMStuffConfig_ImGuiStyle;

    GbxTools::SetMwMemberInfo(
        &TMStuffConfig_ShowUi,
        SMwMemberInfo::BOOL,
        0x48001001,
        offsetof(TMStuff::CTMStuffConfig, m_ShowUi),
        "ShowUi", "m_ShowUi", "ShowUi",
        SMwMemberInfo::eFlags::GET | SMwMemberInfo::eFlags::SET
    );
    m_MwClassInfo_TMStuffConfig.m_MemberInfos[1] = &TMStuffConfig_ShowUi;

    GbxTools::SetMwMemberInfo(
        &TMStuffConfig_ShowTM,
        SMwMemberInfo::BOOL,
        0x48001002,
        offsetof(TMStuff::CTMStuffConfig, m_ShowTrackManiaNod),
        "ShowTrackManiaNod", "m_ShowTrackManiaNod", "ShowTrackManiaNod",
        SMwMemberInfo::eFlags::GET | SMwMemberInfo::eFlags::SET
    );
    m_MwClassInfo_TMStuffConfig.m_MemberInfos[2] = &TMStuffConfig_ShowTM;

    GbxTools::SetMwMemberInfo(
        &TMStuffConfig_ShowResave,
        SMwMemberInfo::BOOL,
        0x48001003,
        offsetof(TMStuff::CTMStuffConfig, m_ShowResave),
        "ShowResave", "m_ShowResave", "ShowResave",
        SMwMemberInfo::eFlags::GET | SMwMemberInfo::eFlags::SET
    );
    m_MwClassInfo_TMStuffConfig.m_MemberInfos[3] = &TMStuffConfig_ShowResave;

    GbxTools::SetMwMemberInfo(
        &TMStuffConfig_ShowPicker,
        SMwMemberInfo::BOOL,
        0x48001004,
        offsetof(TMStuff::CTMStuffConfig, m_ShowPicker),
        "ShowPicker", "m_ShowPicker", "ShowPicker",
        SMwMemberInfo::eFlags::GET | SMwMemberInfo::eFlags::SET
    );
    m_MwClassInfo_TMStuffConfig.m_MemberInfos[4] = &TMStuffConfig_ShowPicker;

    GbxTools::SetMwMemberInfo(
        &TMStuffConfig_ShowScreenShot,
        SMwMemberInfo::BOOL,
        0x48001005,
        offsetof(TMStuff::CTMStuffConfig, m_ShowScreenShot),
        "ShowScreenShot", "m_ShowScreenShot", "ShowScreenShot",
        SMwMemberInfo::eFlags::GET | SMwMemberInfo::eFlags::SET
    );
    m_MwClassInfo_TMStuffConfig.m_MemberInfos[5] = &TMStuffConfig_ShowScreenShot;


    // prepare the engine

    CMwEngineManager* engine_man = GbxTools::GetEngineManager();
    CMwEngineManager::AddClass(engine_man, &m_MwClassInfo_TMStuffConfig);
}

void** vtable_TMStuffConfig = nullptr;

TMStuff::CTMStuffConfig* TMStuff::CTMStuffConfig::MwNewCTMStuffConfig()
{
    if(!vtable_TMStuffConfig) {
        // prep vtable for first use
        vtable_TMStuffConfig = (void**)malloc(0x80);
        if(memcpy_s(vtable_TMStuffConfig, 0x80, vtable_CMwNod, 0x80)) // copy vtable from CMwNod
        {
            printf("err: %s\n", strerror(errno));
        }
        vtable_TMStuffConfig[0] = (void*)TMStuff::CTMStuffConfig::vDelete;
        vtable_TMStuffConfig[1] = (void*)TMStuff::CTMStuffConfig::vMwGetClassInfo;
        vtable_TMStuffConfig[2] = (void*)TMStuff::CTMStuffConfig::vGetMwClassId;
        vtable_TMStuffConfig[3] = (void*)TMStuff::CTMStuffConfig::vMwIsKindOf;
        vtable_TMStuffConfig[13] = (void*)TMStuff::CTMStuffConfig::vGetUidParamFromIndex;
        vtable_TMStuffConfig[14] = (void*)TMStuff::CTMStuffConfig::vGetParamCount;
        vtable_TMStuffConfig[16] = (void*)TMStuff::CTMStuffConfig::vChunk;
        vtable_TMStuffConfig[17] = (void*)TMStuff::CTMStuffConfig::vGetChunkInfo;
        vtable_TMStuffConfig[18] = (void*)TMStuff::CTMStuffConfig::vGetUidChunkFromIndex;
        vtable_TMStuffConfig[19] = (void*)TMStuff::CTMStuffConfig::vGetChunkCount;
        vtable_TMStuffConfig[20] = (void*)TMStuff::CTMStuffConfig::vOnNodLoaded;
    }
    // create nod
    TMStuff::CTMStuffConfig* nod = (TMStuff::CTMStuffConfig*)malloc(sizeof(TMStuff::CTMStuffConfig));
    if(nod != nullptr) {
        memset(nod, 0, sizeof(TMStuff::CTMStuffConfig));
        void (__thiscall* callable)(CMwNod* in_nod) = (void (__thiscall* )(CMwNod* in_nod))0x004d6140; // CMwNod constructor
        callable(nod); // CMwNod(nod);
        void** new_class_vtable = (void**)nod;
        new_class_vtable[0] = vtable_TMStuffConfig;
        // prep members
        nod->m_ShowUi = 1;
        nod->m_ShowTrackManiaNod = 0;
        nod->m_ShowPicker = 0;
        nod->m_ShowScreenShot = 0;
        nod->m_ImGuiStyle = TMStuff::CTMStuffImGuiStyle::MwNewCTMStuffImGuiStyle();
        return nod;
    }
    return nullptr;
}

TMStuff::CTMStuffConfig* __thiscall TMStuff::CTMStuffConfig::vDelete(CTMStuffConfig* nod, byte purge)
{
    void (__thiscall* vDelCMwNod)(CMwNod* nod) = (void (__thiscall*)(CMwNod* nod))0x004d61a0;

    // CTMStuffConfig
    void** new_class_vtable = (void**)nod;
    new_class_vtable[0] = vtable_TMStuffConfig;

    if(nod->m_ImGuiStyle != nullptr) {
        GbxTools::MwDestroy(nod->m_ImGuiStyle);
    }
    vDelCMwNod(nod);
    // **************
    if((purge & 1) != 0) {
        free(nod);
    }
    return nod;
}

void __thiscall TMStuff::CTMStuffConfig::vChunk(TMStuff::CTMStuffConfig* nod, CClassicArchive* pA, int chunkId)
{
    void (__thiscall* DoBool)(CClassicArchive* pArchive, Bool* value, int amount) = (void (__thiscall* )(CClassicArchive* pArchive, Bool* value, int amount))0x0041eb80;
    void (__thiscall* CMwNod_Chunk)(CMwNod* nod, CClassicArchive* pA, int chunkId) = (void (__thiscall* )(CMwNod* nod, CClassicArchive* pA, int chunkId))vtable_CMwNod[16];

    switch(chunkId) {
        case 0x48001000: {
            pA->DoNod((CMwNod**)&nod->m_ImGuiStyle);
            return;
        }
        case 0x48001001: {
            DoBool(pA, &nod->m_ShowUi, 1);
            DoBool(pA, &nod->m_ShowTrackManiaNod, 1);
            DoBool(pA, &nod->m_ShowResave, 1);
            DoBool(pA, &nod->m_ShowPicker, 1);
            return;
        }
        case 0x48001002: {
            DoBool(pA, &nod->m_ShowScreenShot, 1);
            return;
        }
    }

    CMwNod_Chunk(nod, pA, chunkId);
    return;
}

int __stdcall TMStuff::CTMStuffConfig::vGetChunkInfo(int chunkId)
{
    int (__stdcall* CMwNod_GetChunkInfo)(int chunkId) = (int (__stdcall* )(int chunkId))vtable_CMwNod[17];

    switch(chunkId) {
        case 0x48001000:
        case 0x48001001:
        case 0x48001002:
            return 3;
    }
    int res = CMwNod_GetChunkInfo(chunkId);
    return res;
};

void __thiscall TMStuff::CTMStuffConfig::vOnNodLoaded(TMStuff::CTMStuffConfig* nod)
{
    if(nod->m_ImGuiStyle == nullptr)
    {
        nod->m_ImGuiStyle = TMStuff::CTMStuffImGuiStyle::MwNewCTMStuffImGuiStyle();
    }
}

