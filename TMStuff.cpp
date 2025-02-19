#include "TMStuff.h"

TMStuff::CTMStuffConfig* TMStuff::m_Config = nullptr;
bool TMStuff::m_Ready = false;
char** TMStuff::m_EngineNames = nullptr;
int TMStuff::m_NumEngines = 0;
char*** TMStuff::m_ClassNames = nullptr;
int* TMStuff::m_NumClasses = nullptr;
CMwClassInfo*** TMStuff::m_Classes = nullptr;
char** TMStuff::m_DriveNames = nullptr;

void SetupImGuiStyle()
{
	// Classic Steam style by metasprite from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 0.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.2862745225429535f, 0.3372549116611481f, 0.2588235437870026f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.2862745225429535f, 0.3372549116611481f, 0.2588235437870026f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.239215686917305f, 0.2666666805744171f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.5372549295425415f, 0.5686274766921997f, 0.5098039507865906f, 0.5f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.1372549086809158f, 0.1568627506494522f, 0.1098039224743843f, 0.5199999809265137f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.239215686917305f, 0.2666666805744171f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2666666805744171f, 0.2980392277240753f, 0.2274509817361832f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2980392277240753f, 0.3372549116611481f, 0.2588235437870026f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.239215686917305f, 0.2666666805744171f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2862745225429535f, 0.3372549116611481f, 0.2588235437870026f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.239215686917305f, 0.2666666805744171f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.3490196168422699f, 0.4196078479290009f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.2784313857555389f, 0.3176470696926117f, 0.239215686917305f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.2470588237047195f, 0.2980392277240753f, 0.2196078449487686f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.2274509817361832f, 0.2666666805744171f, 0.2078431397676468f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.5882353186607361f, 0.5372549295425415f, 0.1764705926179886f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.3490196168422699f, 0.4196078479290009f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5372549295425415f, 0.5686274766921997f, 0.5098039507865906f, 0.5f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.2862745225429535f, 0.3372549116611481f, 0.2588235437870026f, 0.4000000059604645f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3490196168422699f, 0.4196078479290009f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.5372549295425415f, 0.5686274766921997f, 0.5098039507865906f, 0.5f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.3490196168422699f, 0.4196078479290009f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.3490196168422699f, 0.4196078479290009f, 0.3098039329051971f, 0.6000000238418579f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.5372549295425415f, 0.5686274766921997f, 0.5098039507865906f, 0.5f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.1372549086809158f, 0.1568627506494522f, 0.1098039224743843f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.5372549295425415f, 0.5686274766921997f, 0.5098039507865906f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.5882353186607361f, 0.5372549295425415f, 0.1764705926179886f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1882352977991104f, 0.2274509817361832f, 0.1764705926179886f, 0.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.5372549295425415f, 0.5686274766921997f, 0.5098039507865906f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.5882353186607361f, 0.5372549295425415f, 0.1764705926179886f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.3490196168422699f, 0.4196078479290009f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.5372549295425415f, 0.5686274766921997f, 0.5098039507865906f, 0.7799999713897705f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.5882353186607361f, 0.5372549295425415f, 0.1764705926179886f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.239215686917305f, 0.2666666805744171f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.3490196168422699f, 0.4196078479290009f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.5882353186607361f, 0.5372549295425415f, 0.1764705926179886f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.0f, 0.7764706015586853f, 0.2784313857555389f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.5882353186607361f, 0.5372549295425415f, 0.1764705926179886f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.729411780834198f, 0.6666666865348816f, 0.239215686917305f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.5882353186607361f, 0.5372549295425415f, 0.1764705926179886f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}

void TMStuff::Init()
{
    printf("Initiating TMStuff...\n");
    // Init classes
    TMStuff::CTMStuffConfig::Init();
    TMStuff::CTMStuffImGuiStyle::Init();
    // Set new engine name
    CMwEngineManager* engine_man = GbxTools::GetEngineManager();
    int j = 0;
    for (int i = 0; i < engine_man->m_Engines.numElems; i++) { // for each engine
		CMwEngineInfo* engineinfo = engine_man->m_Engines.pElems[i];
		if (engineinfo != NULL) {
            j++;
			if(engineinfo->m_EngineId == 0x48000000) {
                engineinfo->m_EngineName = "TMStuff";
			}
		}
	}
	TMStuff::m_NumEngines = j;
	TMStuff::m_EngineNames = (char**)malloc(sizeof(char*) * TMStuff::m_NumEngines);
	TMStuff::m_ClassNames = (char***)malloc(sizeof(char**) * TMStuff::m_NumEngines);
	TMStuff::m_NumClasses = (int*)malloc(sizeof(int) * TMStuff::m_NumEngines);
	TMStuff::m_Classes = (CMwClassInfo***)malloc(sizeof(CMwClassInfo**) * TMStuff::m_NumEngines);

	for(int i=0;i<TMStuff::m_NumEngines;i++) {
        TMStuff::m_NumClasses[i] = 0;
	}
	j = 0;
	for (int i = 0; i < engine_man->m_Engines.numElems; i++) { // for each engine
		CMwEngineInfo* engineinfo = engine_man->m_Engines.pElems[i];
		if (engineinfo != NULL) {
			TMStuff::m_EngineNames[j] = engineinfo->m_EngineName;
			for(int k=0;k<engineinfo->m_Classes.numElems;k++) {
                CMwClassInfo* classinfo = engineinfo->m_Classes.pElems[k];
                if(classinfo) {
                    if(classinfo->m_Constructor) {
                        TMStuff::m_NumClasses[j] = TMStuff::m_NumClasses[j] + 1;
                    }
                }
			}
			j++;
		}
	}

	for(int i=0;i<TMStuff::m_NumEngines;i++) {
        TMStuff::m_ClassNames[i] = (char**)malloc(sizeof(char**) * TMStuff::m_NumClasses[i]);
        TMStuff::m_Classes[i] = (CMwClassInfo**)malloc(sizeof(CMwClassInfo**) * TMStuff::m_NumClasses[i]);
	}

	j = 0;
	int l = 0;
    for (int i = 0; i < engine_man->m_Engines.numElems; i++) { // for each engine
		CMwEngineInfo* engineinfo = engine_man->m_Engines.pElems[i];
		if (engineinfo != NULL) {
            char** class_names = TMStuff::m_ClassNames[j];
            CMwClassInfo** classes = TMStuff::m_Classes[j];
			for(int k=0; k<engineinfo->m_Classes.numElems; k++) {
                CMwClassInfo* classinfo = engineinfo->m_Classes.pElems[k];
                if(classinfo) {
                    if(classinfo->m_Constructor) {
                        class_names[l] = classinfo->m_ClassName;
                        classes[l] = classinfo;
                        l++;
                    }
                }
			}
			l=0;
			j++;
		}
	}
	TMStuff::m_DriveNames = (char**)malloc(sizeof(char*) * 2);
	TMStuff::m_DriveNames[0] = "GameData/";
	TMStuff::m_DriveNames[1] = "PC:/";

    TMStuff::m_Ready = true;
}

void TMStuff::InitImGui() // is called AFTER Init()
{
    //SetupImGuiStyle();
    // Load config
    // Load/Create config
    if(!GbxTools::LoadNod((CMwNod**)&TMStuff::m_Config, "Config.TMStuff.Gbx")) {
        printf("No config found, creating...\n");
        TMStuff::m_Config = TMStuff::CTMStuffConfig::MwNewCTMStuffConfig();
        TMStuff::m_Config->m_ImGuiStyle->Capture(TMStuff::m_Config->m_ImGuiStyle);
        printf("Config created @ %08X\n", TMStuff::m_Config);
	}

	if(!TMStuff::m_Config->m_ImGuiStyle->MwIsKindOf(0x48002000)) {
        GbxTools::MwDestroy(TMStuff::m_Config->m_ImGuiStyle);
        TMStuff::m_Config->m_ImGuiStyle = TMStuff::CTMStuffImGuiStyle::MwNewCTMStuffImGuiStyle();
        TMStuff::m_Config->m_ImGuiStyle->Capture(TMStuff::m_Config->m_ImGuiStyle);
	}

	TMStuff::m_Config->m_ImGuiStyle->Set(TMStuff::m_Config->m_ImGuiStyle);

	GbxTools::SaveNod(TMStuff::m_Config, "Config.TMStuff.Gbx", GBX_ISR | GBX_EMBEDDED);
}

void TMStuff::Terminate()
{
    printf("Stopping TMStuff...\n");
    GbxTools::SaveNod(TMStuff::m_Config, "Config.TMStuff.Gbx", GBX_ISR | GBX_EMBEDDED);
    TMStuff::m_Config->Delete(1);
    TMStuff::m_Config = nullptr;
    TMStuff::m_Ready = false;
    free(TMStuff::m_EngineNames);
    for(int i=0;i<TMStuff::m_NumEngines;i++) {
        free(TMStuff::m_ClassNames);
        free(TMStuff::m_Classes);
	}
	free(TMStuff::m_NumClasses);
	free(TMStuff::m_DriveNames);
}

static int ImGuiCFastStringResize(ImGuiInputTextCallbackData* data)
{
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        CFastString* my_str = (CFastString*)data->UserData;
        my_str->m_Size = data->BufSize;
        realloc(my_str->m_Str, my_str->m_Size);
        my_str->m_Str[my_str->m_Size] = '\0';
        //IM_ASSERT(my_str->begin() == data->Buf);
        //my_str->resize(data->BufSize); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
        //data->Buf = my_str->begin();
    }
    return 0;
}

void TMStuff::CopyToClipboard(char* str, int str_size)
{
    HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, str_size);
    memcpy(GlobalLock(hMem), str, str_size);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
}

void TMStuff::DoNodCopyButton(CMwNod* nod, SMwClassMemberInfo* memberinfo, MwNodWindow* nodwindow, CMwNod* parent_nod)
{
    ImGui::PushID(nod);
    if(ImGui::Button("Copy")) {
        char* hex = (char*)malloc(32);
        sprintf_s(hex, 32, "%09X", nod);
        CopyToClipboard(hex, 32);
        printf("Address copied to clipboard: \"%s\"\n", hex);
        free(hex);
    }

    if(nod) {
        ImGui::SameLine();
        if(ImGui::Button("X"))
        {
            nod->Delete(0);
        }
    }
    ImGui::SameLine();
    if(ImGui::Button("Set")) {
        nodwindow->m_TargetMember = memberinfo;
        nodwindow->m_IsSetDialog = true;
        nodwindow->m_ParentNod = parent_nod;
    }
    ImGui::PopID();
}

void TMStuff::DoNodCopyButton(CMwNod** nod, SMwClassMemberInfo* memberinfo, MwNodWindow* nodwindow, CMwNod* parent_nod)
{
    ImGui::PushID(*nod);
    CMwNod* my_nod = *nod;
    if(my_nod) {
        if(ImGui::Button("Copy")) {
            char* hex = (char*)malloc(32);
            sprintf_s(hex, 32, "%09X", my_nod);
            CopyToClipboard(hex, 32);
            printf("Address copied to clipboard: \"%s\"\n", hex);
            free(hex);
        }
        ImGui::SameLine();
        if(ImGui::Button("Del"))
        {
            GbxTools::MwDestroy(*nod);
            *nod = nullptr;
        }
    } else {
        ImGui::SameLine();
        if(ImGui::Button("New"))
        {
            CMwNod* new_nod = GbxTools::CreateByMwClassInfo(memberinfo->pClassInfo);
            GbxTools::IncrementRefCount(new_nod);
            *nod = new_nod;
        }
    }
    ImGui::SameLine();
    if(ImGui::Button("Set")) {
        nodwindow->m_TargetMember = memberinfo;
        nodwindow->m_IsSetDialog = true;
        nodwindow->m_ParentNod = parent_nod;
    }
    ImGui::PopID();
}

void TMStuff::DoNodCopyButton(CMwNod* nod, bool killable)
{
    ImGui::PushID(nod);
    if(ImGui::Button("Copy")) {
        char* hex = (char*)malloc(32);
        sprintf_s(hex, 32, "%09X", nod);
        CopyToClipboard(hex, 32);
        printf("Address copied to clipboard: \"%s\"\n", hex);
        free(hex);
    }

    if(nod && killable) {
        ImGui::SameLine();
        if(ImGui::Button("X"))
        {
            nod->Delete(0);
        }
    }
    ImGui::PopID();
}

void DoClassAuto(CMwNod* nod, CMwClassInfo* nod_class_info, TMStuff::MwNodWindow* nodwindow);
void DoNod(CMwNod* nod, TMStuff::MwNodWindow* nodwindow);

void DoMemberClass(CMwNod* nod, SMwClassMemberInfo* info, TMStuff::MwNodWindow* nodwindow, CMwNod* parent_nod)
{
    TMStuff::DoNodCopyButton(nod, info, nodwindow, parent_nod);
    ImGui::SameLine();
    if(nod == nullptr) {
        ImGui::Text("%s<%s> @ %08X", info->pszName, info->pClassInfo->m_ClassName, nod);
        return;
    }
    if(ImGui::TreeNode(nod, "%s<%s> @ %08X", info->pszName, info->pClassInfo->m_ClassName, nod)) {
        DoNod(nod, nodwindow);
        ImGui::TreePop();
    }
}

void DoMemberClass(CMwNod** nod, SMwClassMemberInfo* info, TMStuff::MwNodWindow* nodwindow, CMwNod* parent_nod)
{
    TMStuff::DoNodCopyButton(nod, info, nodwindow, parent_nod);
    ImGui::SameLine();
    if(*nod == nullptr) {
        ImGui::Text("%s<%s> @ %08X", info->pszName, info->pClassInfo->m_ClassName, *nod);
        return;
    }
    if(ImGui::TreeNode(*nod, "%s<%s> @ %08X", info->pszName, info->pClassInfo->m_ClassName, *nod)) {
        DoNod(*nod, nodwindow);
        ImGui::TreePop();
    }
}

void DoMemberClass(CMwNod* nod, char* name, char* type, TMStuff::MwNodWindow* nodwindow)
{
    TMStuff::DoNodCopyButton(nod, true);
    ImGui::SameLine();
    if(nod == nullptr) {
        ImGui::Text("%s<%s> @ %08X", name, type, nod);
        return;
    }
    if(ImGui::TreeNode(nod, "%s<%s> @ %08X", name, type, nod)) {
        DoNod(nod, nodwindow);
        ImGui::TreePop();
    }
}

char* GetFlagStr(int flags, SMwMemberInfo::eFlags mask)
{
    if(flags & mask & SMwMemberInfo::eFlags::READ)
        return "R";
    if(flags & mask & SMwMemberInfo::eFlags::WRITE)
        return "W";
    if(flags & mask & SMwMemberInfo::eFlags::U1)
        return "1";
    if(flags & mask & SMwMemberInfo::eFlags::U2)
        return "2";
    if(flags & mask & SMwMemberInfo::eFlags::VIRTUAL_GET)
        return "G";
    if(flags & mask & SMwMemberInfo::eFlags::VIRTUAL_SET)
        return "S";
    if(flags & mask & SMwMemberInfo::eFlags::VIRTUAL_ADD)
        return "A";
    if(flags & mask & SMwMemberInfo::eFlags::VIRTUAL_SUB)
        return "S";
    return "_";
}

void DoClassAuto(CMwNod* nod, CMwClassInfo* nod_class_info, TMStuff::MwNodWindow* nodwindow)
{
    int* nod_valk = (int*)nod;
    for(int i=0;i<nod_class_info->m_MemberCount;i++)
    {
        SMwMemberInfo* member = nod_class_info->m_MemberInfos[i];
        switch(member->memberId) // member id blacklist for broken members (THANKS NANDO!)
        {
            case 0x904f010: // when trying to access it, it forcfully tries to get a value from an object that might be null, fantastic
                continue;
        }
        ImGui::Text("(%s%s %s%s %s%s %s%s)",
            GetFlagStr(member->flags, SMwMemberInfo::eFlags::READ),
            GetFlagStr(member->flags, SMwMemberInfo::eFlags::WRITE),
            GetFlagStr(member->flags, SMwMemberInfo::eFlags::U1),
            GetFlagStr(member->flags, SMwMemberInfo::eFlags::U2),
            GetFlagStr(member->flags, SMwMemberInfo::eFlags::VIRTUAL_GET),
            GetFlagStr(member->flags, SMwMemberInfo::eFlags::VIRTUAL_SET),
            GetFlagStr(member->flags, SMwMemberInfo::eFlags::VIRTUAL_ADD),
            GetFlagStr(member->flags, SMwMemberInfo::eFlags::VIRTUAL_SUB)
        );
        ImGui::SameLine();

        /////////////////
        // doin
        /////////////////

        if(member->fieldOffset == -1 && member->type != 0) // virtual memebers
        {
            ImGui::PushID((int)member+(int)nod);
            CMwStack* stacc = (CMwStack*)malloc(sizeof(CMwStack));
            stacc->m_Size = 1;
            stacc->ppMemberInfos = (SMwMemberInfo**)malloc(sizeof(SMwMemberInfo*) * stacc->m_Size);
            stacc->ppMemberInfos[0] = member;
            stacc->iCurrentPos = 0;
            switch(member->type)
            {
                case SMwMemberInfo::BOOL:
                {
                    Bool* b = nullptr;
                    nod->VirtualParam_Get(stacc, (void**)&b);
                    if(ImGui::Checkbox(member->pszName, (bool*)b)) {
                        stacc->iCurrentPos = 0;
                        nod->VirtualParam_Set(stacc, (void**)b);
                    }
                    break;
                }
                case SMwMemberInfo::CLASS:
                {
                    CMwNod* new_nod = nullptr;
                    nod->VirtualParam_Get(stacc, (void**)&new_nod);
                    DoMemberClass(new_nod, (SMwClassMemberInfo*)member, nodwindow, nod);
                    break;
                }
                case SMwMemberInfo::CLASSARRAY:
                {
                    SMwClassArrayMemberInfo* classmember = (SMwClassArrayMemberInfo*)member;
                    CFastArray<CMwNod*>* buf = nullptr;
                    nod->VirtualParam_Get(stacc, (void**)&buf);
                    if(member->flags & SMwMemberInfo::eFlags::VIRTUAL_ADD) {
                        if(ImGui::Button("Add"))
                        {
                            nodwindow->m_ParentNod = nod;
                            nodwindow->m_TargetMember = (SMwClassMemberInfo*)member;
                            nodwindow->m_IsAddDialog = true;
                        }
                        ImGui::SameLine();
                    }
                    if(member->flags & SMwMemberInfo::eFlags::VIRTUAL_SUB) {
                        if(ImGui::Button("Sub") && buf->numElems > 0)
                        {
                            int pos = buf->numElems - 1;
                            GbxTools::VirtualParam_Sub_Fast(nod, member, (void**)&pos);
                        }
                        ImGui::SameLine();
                    }
                    if(ImGui::TreeNode(buf, "%s<%s> (%i)", member->pszName, classmember->pClassInfo->m_ClassName, buf->numElems)) {
                        for(int j=0;j<buf->numElems;j++)
                        {
                            CMwNod* new_nod = buf->pElems[j];
                            ImGui::PushID(j);
                            char* class_name = "???";
                            CMwId* id = nullptr;
                            char* idname = "???";
                            if(new_nod) {
                                class_name = new_nod->MwGetClassInfo()->m_ClassName;
                                id = new_nod->MwGetId();
                                if(id) {
                                    CFastString* idstr = GbxTools::GetStringById(id);
                                    if(idstr)
                                        idname = idstr->m_Str;
                                    else
                                        idname = "Unassigned";
                                }
                            }
                            ImGui::Text("%i:", j);
                            ImGui::SameLine();
                            if(member->flags & SMwMemberInfo::eFlags::VIRTUAL_SET) {
                                if(ImGui::Button("Set")) {
                                    nodwindow->m_ParentNod = nod;
                                    nodwindow->m_IsSetDialog = true;
                                    nodwindow->m_TargetMember = (SMwClassMemberInfo*)member;
                                    nodwindow->m_TargetMemberIndex = j;
                                }
                                ImGui::SameLine();
                            }
                            DoMemberClass(new_nod, idname, class_name, nodwindow);
                            ImGui::PopID();
                        }
                        ImGui::TreePop();
                    }
                    break;
                }
                case SMwMemberInfo::CLASSBUFFER:
                {
                    SMwClassArrayMemberInfo* classmember = (SMwClassArrayMemberInfo*)member;
                    SFastBuffer<CMwNod*>* buf = nullptr;
                    nod->VirtualParam_Get(stacc, (void**)&buf);
                    if(member->flags & SMwMemberInfo::eFlags::VIRTUAL_ADD) {
                        if(ImGui::Button("Add"))
                        {
                            nodwindow->m_ParentNod = nod;
                            nodwindow->m_TargetMember = (SMwClassMemberInfo*)member;
                            nodwindow->m_IsAddDialog = true;
                        }
                        ImGui::SameLine();
                    }
                    if(member->flags & SMwMemberInfo::eFlags::VIRTUAL_SUB) {
                        if(ImGui::Button("Sub") && buf->numElems > 0)
                        {
                            int pos = buf->numElems - 1;
                            GbxTools::VirtualParam_Sub_Fast(nod, member, (void**)&pos);
                        }
                        ImGui::SameLine();
                    }
                    if(ImGui::TreeNode(buf, "%s<%s> (%i)", member->pszName, classmember->pClassInfo->m_ClassName, buf->numElems)) {
                        for(int j=0;j<buf->numElems;j++)
                        {
                            CMwNod* new_nod = buf->pElems[j];
                            ImGui::PushID(j);
                            char* class_name = "???";
                            CMwId* id = nullptr;
                            char* idname = "???";
                            if(new_nod) {
                                class_name = new_nod->MwGetClassInfo()->m_ClassName;
                                id = new_nod->MwGetId();
                                if(id) {
                                    CFastString* idstr = GbxTools::GetStringById(id);
                                    if(idstr)
                                        idname = idstr->m_Str;
                                    else
                                        idname = "Unassigned";
                                }
                            }
                            ImGui::Text("%i:", j);
                            ImGui::SameLine();
                            if(member->flags & SMwMemberInfo::eFlags::VIRTUAL_SET) {
                                if(ImGui::Button("Set")) {
                                    nodwindow->m_ParentNod = nod;
                                    nodwindow->m_IsSetDialog = true;
                                    nodwindow->m_TargetMember = (SMwClassMemberInfo*)member;
                                    nodwindow->m_TargetMemberIndex = j;
                                }
                                ImGui::SameLine();
                            }
                            DoMemberClass(new_nod, idname, class_name, nodwindow);
                            ImGui::PopID();
                        }
                        ImGui::TreePop();
                    }
                    break;
                }
                case SMwMemberInfo::COLOR:
                {
                    SVec3* vec = nullptr;
                    nod->VirtualParam_Get(stacc, (void**)&vec);
                    if(ImGui::ColorEdit3(member->pszName, vec->array, 0)) {
                        stacc->iCurrentPos = 0;
                        nod->VirtualParam_Set(stacc, (void**)vec);
                    }
                    break;
                }
                case SMwMemberInfo::ENUM:
                {
                    SMwEnumInfo* enuminfo = (SMwEnumInfo*)member;
                    int* nval = nullptr;
                    nod->VirtualParam_Get(stacc, (void**)&nval);
                    if(ImGui::Combo(member->pszName, nval, enuminfo->ppszEnumValues, enuminfo->numValues, enuminfo->numValues)) {
                        stacc->iCurrentPos = 0;
                        nod->VirtualParam_Set(stacc, (void**)nval);
                    }
                    break;
                }
                case SMwMemberInfo::INT:
                case SMwMemberInfo::NATURAL:
                {
                    Natural* nval = nullptr;
                    nod->VirtualParam_Get(stacc, (void**)&nval);
                    if(ImGui::InputInt(member->pszName, (int*)nval, 1, 1, 0)) {
                        stacc->iCurrentPos = 0;
                        nod->VirtualParam_Set(stacc, (void**)nval);
                    }
                    break;
                }
                case SMwMemberInfo::INTRANGE:
                case SMwMemberInfo::NATURALRANGE:
                {
                    SMwMemberInfoIntRange* range_member = (SMwMemberInfoIntRange*)member;
                    Natural* nval = nullptr;
                    nod->VirtualParam_Get(stacc, (void**)&nval);
                    if(ImGui::SliderInt(member->pszName, (int*)nval, range_member->min, range_member->max)) {
                        stacc->iCurrentPos = 0;
                        nod->VirtualParam_Set(stacc, (void**)nval);
                    }
                    break;
                }
                case SMwMemberInfo::ISO4:
                {
                    bool edit = false;
                    SIso4* iso = nullptr;
                    nod->VirtualParam_Get(stacc, (void**)&iso);
                    ImGui::PushID(iso);
                    edit |= ImGui::InputFloat3(member->pszName, iso->array1, "%f", 0);
                    ImGui::PopID();
                    ImGui::PushID(iso + 1);
                    ImGui::Text("             ");
                    ImGui::SameLine();
                    edit |= ImGui::InputFloat3("", iso->array2, "%f", 0);
                    ImGui::PopID();
                    ImGui::PushID(iso + 2);
                    ImGui::Text("             ");
                    ImGui::SameLine();
                    edit |= ImGui::InputFloat3("", iso->array3, "%f", 0);
                    ImGui::PopID();
                    ImGui::PushID(iso + 3);
                    ImGui::Text("             ");
                    ImGui::SameLine();
                    edit |= ImGui::InputFloat3("", iso->array4, "%f", 0);
                    ImGui::PopID();
                    if(edit) {
                        stacc->iCurrentPos = 0;
                        nod->VirtualParam_Set(stacc, (void**)iso);
                    }
                    break;
                }
                case SMwMemberInfo::REAL:
                {
                    Real* fval = nullptr;
                    nod->VirtualParam_Get(stacc, (void**)&fval);
                    ImGui::PushID(fval);
                    if(ImGui::InputFloat(member->pszName, fval, 0.01, 0.1, "%f", 0)) {
                        stacc->iCurrentPos = 0;
                        nod->VirtualParam_Set(stacc, (void**)fval);
                    }
                    ImGui::PopID();
                    break;
                }
                case SMwMemberInfo::REALRANGE:
                {
                    SMwMemberInfoRealRange* range_member = (SMwMemberInfoRealRange*)member;
                    Natural* nval = nullptr;
                    nod->VirtualParam_Get(stacc, (void**)&nval);
                    if(ImGui::SliderFloat(member->pszName, (float*)nval, range_member->min, range_member->max)) {
                        stacc->iCurrentPos = 0;
                        nod->VirtualParam_Set(stacc, (void**)nval);
                    }
                    break;
                }
                case SMwMemberInfo::ID:
                {
                    CFastString* str = nullptr;
                    nod->VirtualParam_Get(stacc, (void**)&str);
                    ImGui::Text("%s = \"%s\"", member->pszName, str->m_Str);
                    ImGui::SameLine();
                    if(ImGui::Button("Set")) {
                        nodwindow->m_ParentNod = nod;
                        nodwindow->m_TargetMember = (SMwClassMemberInfo*)member;
                        nodwindow->m_IsSetIdDialog = true;
                    }
                    break;
                }
                case SMwMemberInfo::VEC2:
                {
                    SVec2* vec = nullptr;
                    nod->VirtualParam_Get(stacc, (void**)&vec);
                    if(ImGui::InputFloat2(member->pszName, vec->array, "%f", 0)) {
                        stacc->iCurrentPos = 0;
                        nod->VirtualParam_Set(stacc, (void**)vec);
                    }

                    break;
                }
                case SMwMemberInfo::VEC3:
                {
                    SVec3* vec = nullptr;
                    nod->VirtualParam_Get(stacc, (void**)&vec);
                    if(ImGui::InputFloat3(member->pszName, vec->array, "%f", 0)) {
                        stacc->iCurrentPos = 0;
                        nod->VirtualParam_Set(stacc, (void**)vec);
                    }
                    break;
                }
                case SMwMemberInfo::VEC4:
                {
                    SVec4* vec = nullptr;
                    nod->VirtualParam_Get(stacc, (void**)&vec);
                    if(ImGui::InputFloat4(member->pszName, vec->array, "%f", 0)) {
                        stacc->iCurrentPos = 0;
                        nod->VirtualParam_Set(stacc, (void**)vec);
                    }
                    break;
                }
                default:
                {
                    ImGui::Text("(Virtual) %s [%i]", member->pszName, member->type);
                    break;
                }
            }
            free(stacc->ppMemberInfos);
            free(stacc);
            ImGui::PopID();
            continue;
        }
        //////////////////////////
        // Non-virtual
        //////////////////////////
        ImGui::PushID((int)member+(int)nod);
        switch(member->type)
        {
            case SMwMemberInfo::ACTION:
            {
                SMwActionInfo* action_member = (SMwActionInfo*)member;
                if(ImGui::Button(member->pszName) && action_member->pFun != nullptr)
                    action_member->pFun(nod);
                break;
            }
            case SMwMemberInfo::BOOL:
            {
                Bool* b = (Bool*)(nod_valk + (member->fieldOffset / 4));
                ImGui::Checkbox(member->pszName, (bool*)b);
                break;
            }
            case SMwMemberInfo::CLASS:
            {
                CMwNod** new_nod = (CMwNod**)nod + (member->fieldOffset / 4);
                DoMemberClass(new_nod, (SMwClassMemberInfo*)member, nodwindow, nod);
                break;
            }
            case SMwMemberInfo::CLASSARRAY:
            {
                SMwClassArrayMemberInfo* classmember = (SMwClassArrayMemberInfo*)member;
                CFastArray<CMwNod*>* buf = (CFastArray<CMwNod*>*)(nod_valk + (member->fieldOffset / 4));
                if(member->flags & SMwMemberInfo::eFlags::U1 || member->flags & SMwMemberInfo::eFlags::VIRTUAL_ADD) {
                    if(ImGui::Button("Add"))
                    {
                        nodwindow->m_ParentNod = nod;
                        nodwindow->m_TargetMember = (SMwClassMemberInfo*)member;
                        nodwindow->m_IsAddDialog = true;
                    }
                }
                ImGui::SameLine();
                if(member->flags & SMwMemberInfo::eFlags::VIRTUAL_SUB) {
                    if(ImGui::Button("Sub") && buf->numElems > 0)
                    {
                        int pos = buf->numElems - 1;
                        GbxTools::VirtualParam_Sub_Fast(nod, member, (void**)&pos);
                    }
                    ImGui::SameLine();
                }
                if(ImGui::TreeNode(buf, "%s<%s> (%i)", member->pszName, classmember->pClassInfo->m_ClassName, buf->numElems)) {
                    for(int j=0;j<buf->numElems;j++)
                    {
                        CMwNod* new_nod = buf->pElems[j];
                        ImGui::PushID(j);
                        char* class_name = "???";
                        CMwId* id = nullptr;
                        char* idname = "???";
                        if(new_nod) {
                            class_name = new_nod->MwGetClassInfo()->m_ClassName;
                            id = new_nod->MwGetId();
                            if(id) {
                                CFastString* idstr = GbxTools::GetStringById(id);
                                if(idstr)
                                    idname = idstr->m_Str;
                                else
                                    idname = "Unassigned";
                            }
                        }
                        ImGui::Text("%i:", j);
                        ImGui::SameLine();
                        if(member->flags & SMwMemberInfo::eFlags::VIRTUAL_SET) {
                            if(ImGui::Button("Set")) {
                                nodwindow->m_ParentNod = nod;
                                nodwindow->m_IsSetDialog = true;
                                nodwindow->m_TargetMember = (SMwClassMemberInfo*)member;
                                nodwindow->m_TargetMemberIndex = j;
                            }
                            ImGui::SameLine();
                        }
                        DoMemberClass(new_nod, idname, class_name, nodwindow);
                        ImGui::PopID();
                    }
                    ImGui::TreePop();
                }
                break;
            }
            case SMwMemberInfo::CLASSBUFFER:
            {
                SMwClassArrayMemberInfo* classmember = (SMwClassArrayMemberInfo*)member;
                SFastBuffer<CMwNod*>* buf = (SFastBuffer<CMwNod*>*)(nod_valk + (member->fieldOffset / 4));
                if(member->flags & SMwMemberInfo::eFlags::VIRTUAL_ADD) {
                    if(ImGui::Button("Add"))
                    {
                        nodwindow->m_ParentNod = nod;
                        nodwindow->m_TargetMember = (SMwClassMemberInfo*)member;
                        nodwindow->m_IsAddDialog = true;
                    }
                    ImGui::SameLine();
                }
                if(member->flags & SMwMemberInfo::eFlags::VIRTUAL_SUB) {
                    if(ImGui::Button("Sub") && buf->numElems > 0)
                    {
                        int pos = buf->numElems - 1;
                        GbxTools::VirtualParam_Sub_Fast(nod, member, (void**)&pos);
                    }
                    ImGui::SameLine();
                }
                if(ImGui::TreeNode(buf, "%s<%s> (%i)", member->pszName, classmember->pClassInfo->m_ClassName, buf->numElems)) {
                    for(int j=0;j<buf->numElems;j++)
                    {
                        CMwNod* new_nod = buf->pElems[j];
                        ImGui::PushID(j);
                        char* class_name = "???";
                        CMwId* id = nullptr;
                        char* idname = "???";
                        if(new_nod) {
                            class_name = new_nod->MwGetClassInfo()->m_ClassName;
                            id = new_nod->MwGetId();
                            if(id) {
                                CFastString* idstr = GbxTools::GetStringById(id);
                                if(idstr)
                                    idname = idstr->m_Str;
                                else
                                    idname = "Unassigned";
                            }
                        }
                        ImGui::Text("%i:", j);
                        ImGui::SameLine();
                        if(member->flags & SMwMemberInfo::eFlags::VIRTUAL_SET) {
                            if(ImGui::Button("Set")) {
                                nodwindow->m_ParentNod = nod;
                                nodwindow->m_IsSetDialog = true;
                                nodwindow->m_TargetMember = (SMwClassMemberInfo*)member;
                                nodwindow->m_TargetMemberIndex = j;
                            }
                            ImGui::SameLine();
                        }
                        DoMemberClass(new_nod, idname, class_name, nodwindow);
                        ImGui::PopID();
                    }
                    ImGui::TreePop();
                }
                break;
            }
            case SMwMemberInfo::COLOR:
            {
                SVec3* vec = (SVec3*)(nod_valk + (member->fieldOffset / 4));
                ImGui::ColorEdit3(member->pszName, vec->array, 0);
                break;
            }
            case SMwMemberInfo::ENUM:
            {
                SMwEnumInfo* enuminfo = (SMwEnumInfo*)member;
                int* nval = (int*)nod + (member->fieldOffset / 4);
                ImGui::Combo(member->pszName, nval, enuminfo->ppszEnumValues, enuminfo->numValues, enuminfo->numValues);
                break;
            }
            case SMwMemberInfo::INT:
            case SMwMemberInfo::NATURAL:
            {
                Natural* nval = (Natural*)nod + (member->fieldOffset / 4);
                ImGui::InputInt(member->pszName, (int*)nval, 1, 1, 0);
                break;
            }
            case SMwMemberInfo::INTRANGE:
            case SMwMemberInfo::NATURALRANGE:
            {
                SMwMemberInfoIntRange* range_member = (SMwMemberInfoIntRange*)member;
                Natural* nval = (Natural*)nod + (member->fieldOffset / 4);
                ImGui::SliderInt(member->pszName, (int*)nval, range_member->min, range_member->max);
                break;
            }
            case SMwMemberInfo::INTARRAY:
            case SMwMemberInfo::NATURALARRAY:
            {
                CFastArray<Integer>* buf = (CFastArray<Integer>*)(nod_valk + (member->fieldOffset / 4));
                if(ImGui::TreeNode(buf, "%s (%i)", member->pszName, buf->numElems)) {
                    for(int j=0;j<buf->numElems;j++)
                    {
                        ImGui::PushID(j);
                        ImGui::Text("\t%i: ", j);
                        ImGui::SameLine();
                        ImGui::InputInt("", &buf->pElems[j], 1, 1, 0);
                        ImGui::PopID();
                    }
                    ImGui::TreePop();
                }
                break;
            }
            case SMwMemberInfo::ISO4:
            {
                int* goffs = (nod_valk + (member->fieldOffset / 4));
                SIso4* iso = (SIso4*)goffs;
                ImGui::PushID(goffs);
                ImGui::InputFloat3(member->pszName, iso->array1, "%f", 0);
                ImGui::PopID();
                ImGui::PushID(goffs + 1);
                ImGui::Text("             ");
                    ImGui::SameLine();
                ImGui::InputFloat3("", iso->array2, "%f", 0);
                ImGui::PopID();
                ImGui::PushID(goffs + 2);
                ImGui::Text("             ");
                    ImGui::SameLine();
                ImGui::InputFloat3("", iso->array3, "%f", 0);
                ImGui::PopID();
                ImGui::PushID(goffs + 3);
                ImGui::Text("             ");
                    ImGui::SameLine();
                ImGui::InputFloat3("", iso->array4, "%f", 0);
                ImGui::PopID();
                break;
            }
            case SMwMemberInfo::REAL:
            {
                Real* fval = (Real*)nod + (member->fieldOffset / 4);
                ImGui::PushID(fval);
                ImGui::InputFloat(member->pszName, fval, 0.01, 0.1, "%f", 0);
                ImGui::PopID();
                break;
            }
            case SMwMemberInfo::REALRANGE:
            {
                SMwMemberInfoRealRange* range_member = (SMwMemberInfoRealRange*)member;
                Real* nval = (Real*)nod + (member->fieldOffset / 4);
                ImGui::SliderFloat(member->pszName, (Real*)nval, range_member->min, range_member->max);
                break;
            }
            case SMwMemberInfo::REALARRAY:
            {
                CFastArray<Real>* buf = (CFastArray<Real>*)(nod_valk + (member->fieldOffset / 4));
                if(ImGui::TreeNode(buf, "%s (%i)", member->pszName, buf->numElems)) {
                    for(int j=0;j<buf->numElems;j++)
                    {
                        ImGui::PushID(j);
                        ImGui::Text("\t%i: ", j);
                        ImGui::SameLine();
                        ImGui::InputFloat("", &buf->pElems[j], 0.01, 0.1, "%f", 0);
                        ImGui::PopID();
                    }
                    ImGui::TreePop();
                }
                break;
            }
            case SMwMemberInfo::ID:
            {
                CFastString* str = (CFastString*)(nod_valk + (member->fieldOffset / 4));
                ImGui::InputText(member->pszName, str->m_Str, str->m_Size, ImGuiInputTextFlags_CallbackResize, ImGuiCFastStringResize, (void*)str);
                break;
            }
            case SMwMemberInfo::VEC2:
            {
                SVec2* vec = (SVec2*)(nod_valk + (member->fieldOffset / 4));
                ImGui::InputFloat2(member->pszName, vec->array, "%f", 0);
                break;
            }
            case SMwMemberInfo::VEC3:
            {
                SVec3* vec = (SVec3*)(nod_valk + (member->fieldOffset / 4));
                ImGui::InputFloat3(member->pszName, vec->array, "%f", 0);
                break;
            }
            case SMwMemberInfo::VEC4:
            {
                SVec4* vec = (SVec4*)(nod_valk + (member->fieldOffset / 4));
                ImGui::InputFloat4(member->pszName, vec->array, "%f", 0);
                break;
            }
            default:
                ImGui::Text("%s [%i]", member->pszName, member->type);
                break;
        }
        ImGui::PopID();
    }
}

// Custom

void DoCMwNod(CMwNod* nod, TMStuff::MwNodWindow* nodwindow)
{
    ImGui::InputInt("ReferenceCount", (int*)&nod->m_ReferenceCount, 1, 1, 0);
    ImGui::Text("SystemFid<CSystemFid> @ %08X", nod->m_SystemFid);
    ImGui::SameLine();
    TMStuff::DoNodCopyButton(nod->m_SystemFid, true);

    SFastBuffer<CMwNod*>* buf = nod->m_Dependants;
    if(buf) {
        ImGui::Text("Dependants (%i)", buf->numElems);
        for(int j=0;j<buf->numElems;j++) {
            CMwNod* new_nod = buf->pElems[j];
            ImGui::PushID(j);
            ImGui::Text("\t%i: %s @ %08X", j, new_nod->MwGetClassInfo()->m_ClassName, new_nod);
            ImGui::SameLine();
            TMStuff::DoNodCopyButton(new_nod, true);
            ImGui::PopID();
        }
    } else
        ImGui::Text("Dependants (NULL)");

    CMwId* id = nod->MwGetId();
    if(id) {
        CFastString* str = GbxTools::GetStringById(id);
        if(str)
            ImGui::Text("IdName = \"%s\"", str->m_Str);
        else
            ImGui::Text("IdName = \"Unassigned\"");
        ImGui::SameLine();
        if(ImGui::Button("Set")) {
            nodwindow->m_ParentNod = nod;
            nodwindow->m_IsSetIdDialog = true;
        }
    }
}

void DoCHmsItem(CMwNod* nod, TMStuff::MwNodWindow* nodwindow)
{

    DoClassAuto(nod, nod->MwGetClassInfo(), nodwindow);
    // additional data not in member infos

    ImGui::PushID((int)nod+69421); // nice

    CMwNod** new_nod = (CMwNod**)nod + (0x30 / 4);
    DoMemberClass(*new_nod, "ParentMobil", "CSceneMobil", nodwindow);
    ImGui::PopID();
}

void DoCTrackManiaPlayerInfo(CMwNod* nod, TMStuff::MwNodWindow* nodwindow)
{
    DoClassAuto(nod, nod->MwGetClassInfo(), nodwindow);
    // additional data not in member infos
    int* nod_valk = (int*)nod;
    ImGui::PushID((int)nod+69420); // nice
    CFastArray<Integer>* buf1 = (CFastArray<Integer>*)(nod_valk + (0xf0 / 4));
    ImGui::Text("CheckpointSplits (%i)", buf1->numElems);
    for(int j=0;j<buf1->numElems;j++)
    {
        ImGui::PushID(j);
        ImGui::Text("\t%i: ", j);
        ImGui::SameLine();
        ImGui::InputInt("", &buf1->pElems[j], 1, 1, 0);
        ImGui::PopID();
    }
    ImGui::PopID();
}

void DoCConstructionCollectorList(CMwNod* nod, TMStuff::MwNodWindow* nodwindow)
{
    struct SCollector {
        CMwId IdName;
        CMwId Collection;
        CMwId Author;
        Natural Count;
    };
    DoClassAuto(nod, nod->MwGetClassInfo(), nodwindow);
    // additional data not in member infos
    int* nod_valk = (int*)nod;
    ImGui::PushID((int)nod+69422); // nice
    SFastBuffer<SCollector>* buf = (SFastBuffer<SCollector>*)(nod_valk + (0x14 / 4));
    if(ImGui::TreeNode(buf, "%s<%s> (%i)", "CollectorList", "CConstructionCollector", buf->numElems)) {
        for(int j=0;j<buf->numElems;j++)
        {
            SCollector* pCollector = &buf->pElems[j];
            //SCollector pCollector = buf->pElems[j];
            ImGui::PushID(j);
            //printf("%i\n", pCollector.IdName);
            ImGui::Text("%i: %s/%s/%s", j, GbxTools::GetStringById(&pCollector->IdName)->m_Str, GbxTools::GetStringById(&pCollector->Collection)->m_Str, GbxTools::GetStringById(&pCollector->Author)->m_Str);
            ImGui::SameLine();
            ImGui::InputInt("Count", (int*)&pCollector->Count, 1, 10);
            //DoMemberClass(new_nod, idname, class_name, nodwindow);*/
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
    ImGui::PopID();
}

void DoClass(CMwNod* nod, CMwClassInfo* nod_class_info, TMStuff::MwNodWindow* nodwindow)
{
    ImGui::PushID(nod);
    if(nod_class_info->m_ParentClass != nullptr)        // Do parent classes first
        DoClass(nod, nod_class_info->m_ParentClass, nodwindow);
    ImGui::PopID();

    if(ImGui::CollapsingHeader(nod_class_info->m_ClassName, ImGuiTreeNodeFlags_DefaultOpen))
    {
        switch(nod_class_info->m_ClassId)
        {
            case 0x01001000: {
                DoCMwNod(nod, nodwindow);
                break;
            }
            case 0x06003000: {
                DoCHmsItem(nod, nodwindow);
                break;
            }
            case 0x24036000: {
                DoCTrackManiaPlayerInfo(nod, nodwindow);
                break;
            }
            case 0x2403c000: {
                DoCConstructionCollectorList(nod, nodwindow);
                break;
            }
            default: {
                DoClassAuto(nod, nod_class_info, nodwindow);
                break;
            }
        }
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 4.0f);
    }
}

void DoNod(CMwNod* nod, TMStuff::MwNodWindow* nodwindow)
{
    CMwClassInfo* nod_info = nod->MwGetClassInfo(); // this causes issues when quitting
    DoClass(nod, nod_info, nodwindow);
}

void DoNod(CMwNod* nod, TMStuff::MwNodWindowAddress* nodwindow)
{
    if(ImGui::Button("Save As")) {
        nodwindow->m_IsSaveDialog = true;
    }
    CMwClassInfo* nod_info = nod->MwGetClassInfo(); // this causes issues when quitting
    DoClass(nod, nod_info, nodwindow);
}

// Class methods


TMStuff::MwNodWindow::MwNodWindow()
{
    this->m_Nod = nullptr;
    this->m_Title = (char*)malloc(128 * sizeof(char));
    this->m_PathBuffer = (char*)malloc(256 * sizeof(char));
    memset(this->m_PathBuffer, 0, 256);
    this->m_Open = true;
    this->m_IsSaveDialog = false;
    this->m_IsLoadDialog = false;
    this->m_IsSetDialog = false;
    this->m_SetAddressBuffer = 0;
    sprintf_s(this->m_Title, 128 * sizeof(char), "Window ID:%08X", this);
}

TMStuff::MwNodWindow::MwNodWindow(CMwNod* nod)
{
    this->m_Nod = nod;
    this->m_Title = (char*)malloc(128 * sizeof(char));
    this->m_PathBuffer = (char*)malloc(256 * sizeof(char));
    memset(this->m_PathBuffer, 0, 256);
    this->m_Open = true;
    this->m_IsSaveDialog = false;
    this->m_IsLoadDialog = false;
    this->m_IsSetDialog = false;
    this->m_SetAddressBuffer = 0;

    sprintf_s(this->m_Title, 128 * sizeof(char), "Nod %08X (%s)", this->m_Nod, this->m_Nod->MwGetClassInfo()->m_ClassName);
}

TMStuff::MwNodWindow::MwNodWindow(CMwNod* nod, char* title)
{
    this->m_Nod = nod;
    this->m_Title = title;
    this->m_PathBuffer = (char*)malloc(256 * sizeof(char));
    memset(this->m_PathBuffer, 0, 256);
    this->m_Open = true;
    this->m_IsSaveDialog = false;
    this->m_IsLoadDialog = false;
    this->m_IsSetDialog = false;
    this->m_SetAddressBuffer = 0;
}

TMStuff::MwNodWindow::~MwNodWindow()
{
    free(this->m_Title);
    free(this->m_PathBuffer);
}

void TMStuff::MwNodWindow::DoSetIdWindow()
{
    if(this->m_IsSetIdDialog) {
        ImGui::PushID(this);
        ImGui::Begin("Set MwId?", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::InputText("IdName", this->m_PathBuffer, 256);
        if(ImGui::Button("Yes##SetId")) {
            if(this->m_TargetMember) {
                CFastString* new_str = (CFastString*)malloc(sizeof(CFastString));
                new_str->m_Str = this->m_PathBuffer;
                new_str->m_Size = 256;
                GbxTools::VirtualParam_Set_Fast(this->m_ParentNod, this->m_TargetMember, (void**)new_str);
                //free(new_str);
            } else {
                this->m_ParentNod->SetIdName(this->m_PathBuffer);
            }
            this->m_TargetMember = nullptr;
            this->m_IsSetIdDialog = false;
            this->m_ParentNod = nullptr;
        }
        ImGui::SameLine();
        if(ImGui::Button("No##SetId")) {
            this->m_IsSetIdDialog = false;
            this->m_TargetMember = nullptr;
            this->m_ParentNod = nullptr;
        }
        ImGui::End();
        ImGui::PopID();
    }
}

bool TMStuff::MwNodWindow::DoSetWindow()
{
    void (__thiscall* CFastArray_ReAllocate)(CFastArray<CMwNod*>* arr, Natural numElems) = (void (__thiscall* )(CFastArray<CMwNod*>* arr, Natural numElems))0x0065c900;

    if(this->m_IsSetDialog || this->m_IsAddDialog) {
        ImGui::PushID(this);
        ImGui::Begin("Param Nod?", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::InputInt("Address##Set", &this->m_SetAddressBuffer, 0, 0, ImGuiInputTextFlags_CharsHexadecimal);
        if(ImGui::Button("Yes##Set")) {
            CMwNod* target = (CMwNod*)this->m_SetAddressBuffer;
            if(this->m_IsSetDialog) {
                if(this->m_TargetMember->flags & SMwMemberInfo::eFlags::VIRTUAL_SET) {
                    // Is virtual (handles both ref and destroy)
                    if(this->m_TargetMember->type == SMwMemberInfo::eType::CLASSARRAY || this->m_TargetMember->type == SMwMemberInfo::eType::CLASSBUFFER) {
                        GbxTools::VirtualParam_Set_SuperFast(this->m_ParentNod, (void**)target, 2, this->m_TargetMemberIndex, this->m_TargetMember);
                    } else {
                        GbxTools::VirtualParam_Set_SuperFast(this->m_ParentNod, (void**)target, 1, this->m_TargetMember);
                    }

                } else {
                    // Is not virtual
                    int nod_offset = this->m_TargetMember->fieldOffset;
                    CMwNod** target_og = (CMwNod**)((int)this->m_ParentNod + nod_offset);
                    if(target != *target_og) {
                        GbxTools::IncrementRefCount(target);
                        if(*target_og != nullptr) {
                            GbxTools::MwDestroy(*target_og);
                        }
                        *target_og = target;
                    }
                }
            } else {
                // Add dialog
                if(this->m_TargetMember) {
                    if(this->m_TargetMember->flags & SMwMemberInfo::eFlags::VIRTUAL_ADD) {
                        // Is virtual
                        GbxTools::VirtualParam_Add_Fast(this->m_ParentNod, this->m_TargetMember, (void**)target);
                    } else {
                        printf("Add to CFastArray\n");
                        int nod_offset = this->m_TargetMember->fieldOffset;
                        CFastArray<CMwNod*>* arr = (CFastArray<CMwNod*>*)((int)this->m_ParentNod + nod_offset);

                        //CFastArray<CMwNod*>* arr = (CFastArray<CMwNod*>*)(nod_valk + (m_offset / 4));
                        Natural new_size = arr->numElems + 1;
                        printf("old size: %i\nnew size: %i\n",arr->numElems, new_size);
                        CFastArray_ReAllocate(arr, new_size);
                        arr->pElems[new_size-1] = target;
                    }
                }
            }
            this->m_IsSetDialog = false;
            this->m_IsAddDialog = false;
            this->m_TargetMember = nullptr;
            this->m_ParentNod = nullptr;
        }
        ImGui::SameLine();
        if(ImGui::Button("No##Set")) {
            this->m_IsSetDialog = false;
            this->m_IsAddDialog = false;
            this->m_TargetMember = nullptr;
            this->m_ParentNod = nullptr;
        }
        ImGui::End();
        ImGui::PopID();
    }
    return true;
}

bool TMStuff::MwNodWindow::Do(bool* p_open)
{
    if(!this->m_Nod)
        return false;
    ImGui::PushID(this);
    ImGui::Begin(this->m_Title, p_open);
    DoNod(this->m_Nod, this);
    ImGui::End();
    DoSetWindow();
    DoSetIdWindow();
    ImGui::PopID();
    return *p_open;
}

bool TMStuff::MwNodWindow::SetNod(CMwNod* nod)
{
    if(!GbxTools::MwCheckThis(nod, sizeof(nod)))
        return false;
    this->m_Nod = nod;
}

bool TMStuff::MwNodWindowAddress::SetNod(CMwNod* nod)
{
    if(!GbxTools::MwCheckThis(nod, sizeof(nod)))
        return false;
    this->m_Nod = nod;
    this->m_Lookup = true;
    return true;
}

void TMStuff::MwNodWindowAddress::SetupNod(CMwNod* nod)
{
}

bool TMStuff::MwNodWindowAddress::Do(bool* p_open)
{
    // delete window
    if(!this->m_Open)
        return false;
    // custom inputs
    ImGui::PushID(this);
    ImGui::SetNextWindowSize(ImVec2(480, 240), ImGuiCond_FirstUseEver);
    ImGui::Begin(this->m_Title, p_open);
    if(ImGui::InputInt("Nod Address", &this->m_AddressBuffer, 0, 0, ImGuiInputTextFlags_CharsHexadecimal))
        this->m_Lookup = false;
    if(ImGui::Button("Lookup"))
    {
        if(GbxTools::MwCheckThis((CMwNod*)this->m_AddressBuffer, sizeof((CMwNod*)this->m_AddressBuffer))) {
            this->m_Nod = (CMwNod*)this->m_AddressBuffer;
            this->m_Lookup = true;
            this->m_VTableBuffer = (void**)m_Nod;
        }
        //this->SetupNod(this->m_Nod);
    }
    ImGui::SameLine();
    if(ImGui::Button("Reset"))
    {
        this->m_Lookup = false;
        this->m_Nod = nullptr;
        this->m_VTableBuffer = nullptr;
    }
    // New Nod button
    ImGui::SameLine();
    if(ImGui::Button("New")) {
        this->m_IsNewDialog = true;
    }
    ImGui::SameLine();
    if(ImGui::Button("Load From")) {
        this->m_IsLoadDialog = true;
    }
    // DRAW NOD
    if(this->m_Lookup && this->m_Nod);
    {
        if(!GbxTools::MwCheckThis(this->m_Nod, sizeof(this->m_Nod))) {
            this->m_Lookup = false;
            this->m_Nod = nullptr;
        } else {
            ImGui::Separator();
            DoNod(this->m_Nod, this);
        }
    }
    // New nod
    if(this->m_IsNewDialog) {
        ImGui::PushID(this);
        ImGui::Begin("New Nod?", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        if(ImGui::Combo("Engine", &this->m_EngineIndex, TMStuff::m_EngineNames, TMStuff::m_NumEngines, TMStuff::m_NumEngines)) {
            this->m_ClassIndex = 0;
            this->m_ClassIdBuffer = TMStuff::m_Classes[m_EngineIndex][m_ClassIndex]->m_ClassId;
        }
        if(ImGui::Combo("Class", &this->m_ClassIndex, TMStuff::m_ClassNames[this->m_EngineIndex], TMStuff::m_NumClasses[this->m_EngineIndex], TMStuff::m_NumClasses[this->m_EngineIndex]))
            this->m_ClassIdBuffer = TMStuff::m_Classes[m_EngineIndex][m_ClassIndex]->m_ClassId;
        ImGui::InputInt("Class ID##New", &this->m_ClassIdBuffer, 0, 0, ImGuiInputTextFlags_CharsHexadecimal);
        if(ImGui::Button("Yes##New")) {
            CMwNod* new_nod = GbxTools::CreateByMwClassId(this->m_ClassIdBuffer);
            if(new_nod) {
                this->m_Nod = new_nod;
                this->m_Lookup = true;
                this->m_AddressBuffer = (int)new_nod;
                //this->SetupNod(this->m_Nod);
            }
            this->m_IsNewDialog = false;
        }
        ImGui::SameLine();
        if(ImGui::Button("No##New")) {
            this->m_IsNewDialog = false;
        }
        ImGui::End();
        ImGui::PopID();
    }
    // Load nod
    if(this->m_IsLoadDialog) {
        ImGui::PushID(this);
        ImGui::Begin("Load Nod?", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Combo(":##Load", &this->m_DriveIndex, TMStuff::m_DriveNames, 2, 2);
        ImGui::SameLine();
        ImGui::InputText("Path##Load", this->m_PathBuffer, 256, 0);
        if(ImGui::Button("Yes##Load")) {
            int res = 0;
            if(m_DriveIndex == 0)
                res = GbxTools::LoadNod(&this->m_Nod, this->m_PathBuffer);
            else
                res = GbxTools::LoadNod2(&this->m_Nod, this->m_PathBuffer);
            this->m_IsLoadDialog = false;
            if(res) {
                this->m_Lookup = true;
                this->m_AddressBuffer = (int)this->m_Nod;
                //this->SetupNod(this->m_Nod);
            }
            printf("Done loading\n");
        }
        ImGui::SameLine();
        if(ImGui::Button("No##Load")) {
            this->m_IsLoadDialog = false;
        }
        ImGui::End();
        ImGui::PopID();
    }
    // Save nod
    if(this->m_IsSaveDialog) {
        int flags = 0;
        ImGui::PushID(this);
        ImGui::Begin("Save Nod?", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Combo(":##Save", &this->m_DriveIndex, TMStuff::m_DriveNames, 2, 2);
        ImGui::SameLine();
        ImGui::InputText("Path##Save", this->m_PathBuffer, 256, 0);
        ImGui::Checkbox("Compress##Save", &this->m_Compress);
        ImGui::SameLine();
        ImGui::Checkbox("Text##Save", &this->m_Text);
        ImGui::SameLine();
        ImGui::Checkbox("R##Save", &this->m_R);
        ImGui::SameLine();
        ImGui::Checkbox("Embed##Save", &this->m_Embed);
        if(ImGui::Button("Yes##Save")) {
            if(this->m_Compress)
                flags |= GBX_COMPRESS;
            if(this->m_Text)
                flags |= GBX_ISTEXT;
            if(this->m_R)
                flags |= GBX_ISR;
            if(this->m_Embed)
                flags |= GBX_EMBEDDED;
            int res = 0;
            if(this->m_DriveIndex == 0)
                res = GbxTools::SaveNod(this->m_Nod, this->m_PathBuffer, flags);
            else
                res = GbxTools::SaveNod2(this->m_Nod, this->m_PathBuffer, flags);
            this->m_IsSaveDialog = false;
            printf("Done saving\n");
        }
        ImGui::SameLine();
        if(ImGui::Button("No##Save")) {
            this->m_IsSaveDialog = false;
        }
        ImGui::End();
        ImGui::PopID();
    }
    // Param nod
    DoSetWindow();
    DoSetIdWindow();
    // end
    ImGui::End();
    ImGui::PopID();
    return true;
}
