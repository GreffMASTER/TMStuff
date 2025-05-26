#include "TMStuffImGuiStyle.h"

CMwClassInfo TMStuff::CTMStuffImGuiStyle::m_MwClassInfo_TMStuffImGuiStyle;
SMwActionInfo CTMStuffImGuiStyle_Capture;
SMwActionInfo CTMStuffImGuiStyle_Set;
SMwActionInfo CTMStuffImGuiStyle_Edit;

void TMStuff::CTMStuffImGuiStyle::Init()
{
    printf("Creating TMStuffConfigImGui class...\n");

    CMwClassInfo* MwClassInfo_CMwNod = GbxTools::GetClassInfoByMwClassId(0x01001000);
    GbxTools::CreateMwClassInfo(&m_MwClassInfo_TMStuffImGuiStyle, 0x48002000, MwClassInfo_CMwNod, "CTMStuffImGuiStyle", (void*)MwNewCTMStuffImGuiStyle);

    m_MwClassInfo_TMStuffImGuiStyle.m_MemberCount = 3;
    m_MwClassInfo_TMStuffImGuiStyle.m_MemberInfos = (SMwMemberInfo**)malloc(sizeof(SMwMemberInfo*) * m_MwClassInfo_TMStuffImGuiStyle.m_MemberCount);

    GbxTools::SetMwMemberInfo(&CTMStuffImGuiStyle_Capture, 0x48002000, "Capture", "m_Capture", "Capture", SMwMemberInfo::eFlags::SET, (void*)TMStuff::CTMStuffImGuiStyle::Capture);
    m_MwClassInfo_TMStuffImGuiStyle.m_MemberInfos[0] = &CTMStuffImGuiStyle_Capture;

    GbxTools::SetMwMemberInfo(&CTMStuffImGuiStyle_Set, 0x48002001, "Set", "m_Set", "Set", SMwMemberInfo::eFlags::SET, (void*)TMStuff::CTMStuffImGuiStyle::Set);
    m_MwClassInfo_TMStuffImGuiStyle.m_MemberInfos[1] = &CTMStuffImGuiStyle_Set;

    GbxTools::SetMwMemberInfo(&CTMStuffImGuiStyle_Edit, 0x48002002, "Edit", "m_Edit", "Edit", SMwMemberInfo::eFlags::SET, (void*)TMStuff::CTMStuffImGuiStyle::Edit);
    m_MwClassInfo_TMStuffImGuiStyle.m_MemberInfos[2] = &CTMStuffImGuiStyle_Edit;


    // add it to the engine
    CMwEngineManager* engine_man = GbxTools::GetEngineManager();
    CMwEngineManager::AddClass(engine_man, &m_MwClassInfo_TMStuffImGuiStyle);
}

void** vtable_TMStuffImGuiStyle = nullptr;

TMStuff::CTMStuffImGuiStyle* TMStuff::CTMStuffImGuiStyle::MwNewCTMStuffImGuiStyle()
{
    if(!vtable_TMStuffImGuiStyle) {
        // prep vtable for first use
        vtable_TMStuffImGuiStyle = (void**)malloc(0x80);
        if(memcpy_s(vtable_TMStuffImGuiStyle, 0x80, vtable_CMwNod, 0x80)) // copy vtable from CMwNod
        {
            printf("err: %s\n", strerror(errno));
        }

        vtable_TMStuffImGuiStyle[0] = (void*)TMStuff::CTMStuffImGuiStyle::vDelete;
        vtable_TMStuffImGuiStyle[1] = (void*)TMStuff::CTMStuffImGuiStyle::vMwGetClassInfo;
        vtable_TMStuffImGuiStyle[2] = (void*)TMStuff::CTMStuffImGuiStyle::vGetMwClassId;
        vtable_TMStuffImGuiStyle[3] = (void*)TMStuff::CTMStuffImGuiStyle::vMwIsKindOf;
        vtable_TMStuffImGuiStyle[4] = (void*)TMStuff::CTMStuffImGuiStyle::vMwGetId;
        vtable_TMStuffImGuiStyle[13] = (void*)TMStuff::CTMStuffImGuiStyle::vGetUidParamFromIndex;
        vtable_TMStuffImGuiStyle[14] = (void*)TMStuff::CTMStuffImGuiStyle::vGetParamCount;
        vtable_TMStuffImGuiStyle[16] = (void*)TMStuff::CTMStuffImGuiStyle::vChunk;
        vtable_TMStuffImGuiStyle[17] = (void*)TMStuff::CTMStuffImGuiStyle::vGetChunkInfo;
        vtable_TMStuffImGuiStyle[18] = (void*)TMStuff::CTMStuffImGuiStyle::vGetUidChunkFromIndex;
        vtable_TMStuffImGuiStyle[19] = (void*)TMStuff::CTMStuffImGuiStyle::vGetChunkCount;
    }
    // create nod
    TMStuff::CTMStuffImGuiStyle* nod = (TMStuff::CTMStuffImGuiStyle*)malloc(sizeof(TMStuff::CTMStuffImGuiStyle));
    if(nod != nullptr) {
        memset(nod, 0, sizeof(TMStuff::CTMStuffImGuiStyle));
        void (__thiscall* callable)(CMwNod* in_nod) = (void (__thiscall* )(CMwNod* in_nod))0x004d6140; // CMwNod constructor
        callable(nod); // CMwNod(nod);
        void** new_class_vtable = (void**)nod;
        new_class_vtable[0] = vtable_TMStuffImGuiStyle;
        // prep members
        nod->m_Style = ImGuiStyle();
        nod->m_ShowEditor = false;
        return nod;
    }
    return nullptr;
}

TMStuff::CTMStuffImGuiStyle* __thiscall TMStuff::CTMStuffImGuiStyle::vDelete(CTMStuffImGuiStyle* nod, byte purge)
{
    void (__thiscall* vDelCMwNod)(CMwNod* nod) = (void (__thiscall*)(CMwNod* nod))0x004d61a0;
    void** new_class_vtable = (void**)nod;
    new_class_vtable[0] = vtable_TMStuffImGuiStyle;

    vDelCMwNod(nod);
    if((purge & 1) != 0) {
        free(nod);
    }
    return nod;
}

void __thiscall TMStuff::CTMStuffImGuiStyle::vChunk(TMStuff::CTMStuffImGuiStyle* nod, CClassicArchive* pA, int chunkId)
{
    void (__thiscall* CMwId_Archive)(CMwId* id, CClassicArchive* pA) = (void (__thiscall* )(CMwId* id, CClassicArchive* pA))0x004dd680;
    void (__thiscall* DoBool)(CClassicArchive* pArchive, Bool* value, int amount) = (void (__thiscall* )(CClassicArchive* pArchive, Bool* value, int amount))0x0041eb80;
    void (__thiscall* DoReal)(CClassicArchive* pArchive, Real* value, int amount) = (void (__thiscall* )(CClassicArchive* pArchive, Real* value, int amount))0x0041eb20;
    void (__thiscall* DoInteger)(CClassicArchive* pArchive, Integer* value, int amount, Bool isHex) = (void (__thiscall* )(CClassicArchive* pArchive, Integer* value, int amount, Bool isHex))0x0041ea40;
    void (__thiscall* DoNatural)(CClassicArchive* pArchive, Natural* value, int amount, Bool isHex) = (void (__thiscall* )(CClassicArchive* pArchive, Natural* value, int amount, Bool isHex))0x0041e9e0;
    void (__thiscall* CMwNod_Chunk)(CMwNod* nod, CClassicArchive* pA, int chunkId) = (void (__thiscall* )(CMwNod* nod, CClassicArchive* pA, int chunkId))vtable_CMwNod[16];

    switch(chunkId) {
        case 0x48002000: {
            CMwId_Archive(&nod->m_IdName, pA);
            break;
        }
        case 0x48002001: {
            DoReal(pA, &nod->m_Style.Alpha, 1);
            DoReal(pA, &nod->m_Style.DisabledAlpha, 1);
            DoReal(pA, &nod->m_Style.WindowPadding.x, 1);
            DoReal(pA, &nod->m_Style.WindowPadding.y, 1);
            DoReal(pA, &nod->m_Style.WindowRounding, 1);
            DoReal(pA, &nod->m_Style.WindowBorderSize, 1);
            DoReal(pA, &nod->m_Style.WindowMinSize.x, 1);
            DoReal(pA, &nod->m_Style.WindowMinSize.y, 1);
            DoReal(pA, &nod->m_Style.WindowTitleAlign.x, 1);
            DoReal(pA, &nod->m_Style.WindowTitleAlign.y, 1);
            DoInteger(pA, (Integer*)&nod->m_Style.WindowMenuButtonPosition, 1, 0);
            DoReal(pA, &nod->m_Style.ChildRounding, 1);
            DoReal(pA, &nod->m_Style.ChildBorderSize, 1);
            DoReal(pA, &nod->m_Style.PopupRounding, 1);
            DoReal(pA, &nod->m_Style.PopupBorderSize, 1);
            DoReal(pA, &nod->m_Style.FramePadding.x, 1);
            DoReal(pA, &nod->m_Style.FramePadding.y, 1);
            DoReal(pA, &nod->m_Style.FrameRounding, 1);
            DoReal(pA, &nod->m_Style.FrameBorderSize, 1);
            DoReal(pA, &nod->m_Style.ItemSpacing.x, 1);
            DoReal(pA, &nod->m_Style.ItemSpacing.y, 1);
            DoReal(pA, &nod->m_Style.ItemInnerSpacing.x, 1);
            DoReal(pA, &nod->m_Style.ItemInnerSpacing.y, 1);
            DoReal(pA, &nod->m_Style.CellPadding.x, 1);
            DoReal(pA, &nod->m_Style.CellPadding.y, 1);
            DoReal(pA, &nod->m_Style.TouchExtraPadding.x, 1);
            DoReal(pA, &nod->m_Style.TouchExtraPadding.y, 1);
            DoReal(pA, &nod->m_Style.IndentSpacing, 1);
            DoReal(pA, &nod->m_Style.ColumnsMinSpacing, 1);
            DoReal(pA, &nod->m_Style.ScrollbarSize, 1);
            DoReal(pA, &nod->m_Style.ScrollbarRounding, 1);
            DoReal(pA, &nod->m_Style.GrabMinSize, 1);
            DoReal(pA, &nod->m_Style.GrabRounding, 1);
            DoReal(pA, &nod->m_Style.LogSliderDeadzone, 1);
            DoReal(pA, &nod->m_Style.TabRounding, 1);
            DoReal(pA, &nod->m_Style.TabBorderSize, 1);
            DoReal(pA, &nod->m_Style.TabMinWidthForCloseButton, 1);
            DoReal(pA, &nod->m_Style.TabBarBorderSize, 1);
            DoReal(pA, &nod->m_Style.TabBarOverlineSize, 1);
            DoReal(pA, &nod->m_Style.TableAngledHeadersAngle, 1);
            DoReal(pA, &nod->m_Style.TableAngledHeadersTextAlign.x, 1);
            DoReal(pA, &nod->m_Style.TableAngledHeadersTextAlign.y, 1);
            DoInteger(pA, (Integer*)&nod->m_Style.ColorButtonPosition, 1, 0);
            DoReal(pA, &nod->m_Style.ButtonTextAlign.x, 1);
            DoReal(pA, &nod->m_Style.ButtonTextAlign.y, 1);
            DoReal(pA, &nod->m_Style.SelectableTextAlign.x, 1);
            DoReal(pA, &nod->m_Style.SelectableTextAlign.y, 1);
            DoReal(pA, &nod->m_Style.SeparatorTextBorderSize, 1);
            DoReal(pA, &nod->m_Style.SeparatorTextAlign.x, 1);
            DoReal(pA, &nod->m_Style.SeparatorTextAlign.y, 1);
            DoReal(pA, &nod->m_Style.SeparatorTextPadding.x, 1);
            DoReal(pA, &nod->m_Style.SeparatorTextPadding.y, 1);
            DoReal(pA, &nod->m_Style.DisplayWindowPadding.x, 1);
            DoReal(pA, &nod->m_Style.DisplayWindowPadding.y, 1);
            DoReal(pA, &nod->m_Style.DisplaySafeAreaPadding.x, 1);
            DoReal(pA, &nod->m_Style.DisplaySafeAreaPadding.y, 1);
            DoReal(pA, &nod->m_Style.MouseCursorScale, 1);
            DoBool(pA, (Bool*)&nod->m_Style.AntiAliasedLines, 1);
            DoBool(pA, (Bool*)&nod->m_Style.AntiAliasedLinesUseTex, 1);
            DoBool(pA, (Bool*)&nod->m_Style.AntiAliasedFill, 1);
            DoReal(pA, &nod->m_Style.CurveTessellationTol, 1);
            DoReal(pA, &nod->m_Style.CircleTessellationMaxError, 1);
            return;
        }
        case 0x48002002: {
            int cnt = ImGuiCol_COUNT;
            DoInteger(pA, &cnt, 1, 0);
            for(int i=0;i<cnt;i++) {
                DoReal(pA, &nod->m_Style.Colors[i].x, 1);
                DoReal(pA, &nod->m_Style.Colors[i].y, 1);
                DoReal(pA, &nod->m_Style.Colors[i].z, 1);
                DoReal(pA, &nod->m_Style.Colors[i].w, 1);
            }
            return;
        }
        case 0x48002003: {
            DoReal(pA, &nod->m_Style.HoverStationaryDelay, 1);
            DoReal(pA, &nod->m_Style.HoverDelayShort, 1);
            DoReal(pA, &nod->m_Style.HoverDelayNormal, 1);
            DoNatural(pA, (Natural*)&nod->m_Style.HoverFlagsForTooltipMouse, 1, 0);
            DoNatural(pA, (Natural*)&nod->m_Style.HoverFlagsForTooltipNav, 1, 0);
            return;
        }
    }

    CMwNod_Chunk(nod, pA, chunkId);
    return;
}

int __stdcall TMStuff::CTMStuffImGuiStyle::vGetChunkInfo(int chunkId) {
    int (__stdcall* CMwNod_GetChunkInfo)(int chunkId) = (int (__stdcall* )(int chunkId))vtable_CMwNod[17];

    switch(chunkId) {
        case 0x48002000:
        case 0x48002001:
        case 0x48002002:
        case 0x48002003:
            return 3;
    }

    int res = CMwNod_GetChunkInfo(chunkId);
    return res;
};

void __fastcall TMStuff::CTMStuffImGuiStyle::Capture(CTMStuffImGuiStyle* nod)
{
    printf("Capturing ImGuiStyle...\n");
    ImGuiStyle& style = ImGui::GetStyle();

    nod->m_Style = style;
}

void __fastcall TMStuff::CTMStuffImGuiStyle::Set(CTMStuffImGuiStyle* nod)
{
    printf("Setting ImGuiStyle...\n");
    ImGuiStyle& style = ImGui::GetStyle();

    style = nod->m_Style;
}

void __fastcall TMStuff::CTMStuffImGuiStyle::Edit(CTMStuffImGuiStyle* nod)
{
    nod->m_ShowEditor = !nod->m_ShowEditor;
}


