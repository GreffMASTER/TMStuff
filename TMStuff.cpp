#include "TMStuff.h"

TMStuff::CTMStuffConfig* TMStuff::m_Config = nullptr;
bool TMStuff::m_Ready = false;
bool TMStuff::m_ImGuiReady = false;
char** TMStuff::m_EngineNames = nullptr;
int TMStuff::m_NumEngines = 0;
char*** TMStuff::m_ClassNames = nullptr;
int* TMStuff::m_NumClasses = nullptr;
CMwClassInfo*** TMStuff::m_Classes = nullptr;
char** TMStuff::m_DriveNames = nullptr;
//std::vector<TMStuff::MwNodWindow*> TMStuff::windowman;

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
	TMStuff::m_DriveNames[1] = "<null>";

    TMStuff::m_Ready = true;
}

void TMStuff::InitImGui() // is called AFTER Init()
{
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

	TMStuff::m_ImGuiReady = true;
}

void TMStuff::Terminate()
{
    printf("Stopping TMStuff...\nSaving settings..\n");
    GbxTools::SaveNod(TMStuff::m_Config, "Config.TMStuff.Gbx", GBX_ISR | GBX_EMBEDDED);
    printf("Saved\n");
    GbxTools::MwDestroy(TMStuff::m_Config);
    printf("Config freed\n");
    TMStuff::m_Config = nullptr;
    TMStuff::m_Ready = false;
    TMStuff::m_ImGuiReady = false;
    free(TMStuff::m_EngineNames);
    for(int i=0;i<TMStuff::m_NumEngines;i++) {
        free(TMStuff::m_ClassNames);
        free(TMStuff::m_Classes);
	}
	free(TMStuff::m_NumClasses);
	free(TMStuff::m_DriveNames);
	printf("TMStuff Terminated\n");
}

void TMStuff::NewNodAddressWindow(CMwNod* nod)
{
    TMStuff::MwNodWindowAddress* new_window = new TMStuff::MwNodWindowAddress();
    new_window->SetNod(nod);
    TMStuff::windowman.push_back(new_window);
}

static int ImGuiCFastStringResize(ImGuiInputTextCallbackData* data)
{
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        CFastString* my_str = (CFastString*)data->UserData;
        my_str->m_Size = data->BufTextLen + 1;
        realloc(my_str->m_Str, my_str->m_Size);
        //my_str->m_Str[my_str->m_Size] = '\0';
        //IM_ASSERT(my_str->begin() == data->Buf);
        //my_str->resize(data->BufSize); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
        //data->Buf = my_str->begin();
    }
    return 0;
}

void TMStuff::CopyToClipboard(char* str, int str_size)
{
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, str_size);
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
    if(ImGui::Button("Nod")) {
        TMStuff::NewNodAddressWindow(nod);
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
        if(ImGui::Button("Nod")) {
            TMStuff::NewNodAddressWindow(*nod);
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
    if(ImGui::Button("Nod")) {
        TMStuff::NewNodAddressWindow(nod);
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
    if(flags & mask & SMwMemberInfo::eFlags::GET)
        return "G";
    if(flags & mask & SMwMemberInfo::eFlags::SET)
        return "S";
    if(flags & mask & SMwMemberInfo::eFlags::ADD)
        return "A";
    if(flags & mask & SMwMemberInfo::eFlags::SUB)
        return "S";
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

////////////////////////////////////////////////////////////////////////

void DoClassAuto(CMwNod* nod, CMwClassInfo* nod_class_info, TMStuff::MwNodWindow* nodwindow)
{
    int* nod_valk = (int*)nod;
    for(int i=0;i<nod_class_info->m_MemberCount;i++)
    {
        SMwMemberInfo* member = nod_class_info->m_MemberInfos[i];
        SMwMemberInfo::eType member_type = member->type;
        switch(member->memberId) // member id blacklist for broken virtual members (THANKS NANDO!)
        {
            case 0x07001011: // when style is empty, it tries to dereference a nullptr, fantastic (temporary, this nod is kinda important)
            case 0x07001012: // when trying to access it, it forcfully tries to get a value from an object that might be a nullptr, fantastic
            case 0x0904f010: // when trying to access it, it forcfully tries to get a value from an object that might be a nullptr... again, fantastic
            case 0x09047006: //
            case 0x09047007: //
            case 0x09047008: // w h a t
            case 0x0a10200c: // SandYach stuff
                continue;
            // type overrides
            case 0x0a031000: // CSceneToyVehicleMaterial.MaterialId
            case 0x24001006: // CTrackMania.VersionFeatures
            {
                member_type = SMwMemberInfo::NATURAL;
                break;
            }
        }
        ImGui::Text("(%s%s%s%s %s%s%s%s)",
            GetFlagStr(member->flags, SMwMemberInfo::eFlags::GET),
            GetFlagStr(member->flags, SMwMemberInfo::eFlags::SET),
            GetFlagStr(member->flags, SMwMemberInfo::eFlags::ADD),
            GetFlagStr(member->flags, SMwMemberInfo::eFlags::SUB),
            GetFlagStr(member->flags, SMwMemberInfo::eFlags::VIRTUAL_GET),
            GetFlagStr(member->flags, SMwMemberInfo::eFlags::VIRTUAL_SET),
            GetFlagStr(member->flags, SMwMemberInfo::eFlags::VIRTUAL_ADD),
            GetFlagStr(member->flags, SMwMemberInfo::eFlags::VIRTUAL_SUB)
        );
        ImGui::SameLine();

        /////////////////
        // doin
        /////////////////

        //printf("%08X\n", member->memberId); // debug
        //printf("Type: %i\n", member_type);

        if(member->type == SMwMemberInfo::ACTION)
        {
            ImGui::PushID((int)member+(int)nod);
            SMwActionInfo* action_member = (SMwActionInfo*)member;
            if(action_member->pFun != nullptr) {
                if(ImGui::Button(member->pszName))
                    action_member->pFun(nod);
            } else {
                ImGui::Text("(V)");
                ImGui::SameLine();
                if(ImGui::Button(member->pszName)) {
                    CMwStack* stacc = (CMwStack*)malloc(sizeof(CMwStack));
                    stacc->m_Size = 1;
                    stacc->ppMemberInfos = (SMwMemberInfo**)malloc(sizeof(SMwMemberInfo*) * stacc->m_Size);
                    stacc->ppMemberInfos[0] = member;
                    stacc->iCurrentPos = 0;

                    nod->VirtualParam_Set(stacc, nullptr);

                    free(stacc->ppMemberInfos);
                    free(stacc);
                }
            }
            ImGui::PopID();
            continue;
        }

        if(member->fieldOffset == -1) // virtual memebers
        {
            //printf("Virtual member\n");
            ImGui::PushID((int)member+(int)nod);
            // prepare the stacc
            CMwStack* stacc = (CMwStack*)malloc(sizeof(CMwStack));
            stacc->m_Size = 1;
            stacc->ppMemberInfos = (SMwMemberInfo**)malloc(sizeof(SMwMemberInfo*) * stacc->m_Size);
            stacc->ppMemberInfos[0] = member;
            stacc->iCurrentPos = 0;

            switch(member_type)
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
                            nodwindow->m_TargetMemberIndex = buf->numElems - 1;
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
                            nodwindow->m_TargetMemberIndex = buf->numElems - 1;
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
                    //printf("ID\n");
                    CFastString* str = nullptr;
                    nod->VirtualParam_Get(stacc, (void**)&str);
                    //printf("VirtualParam_Get OK\n");
                    ImGui::Text("%s = \"%s\"", member->pszName, str->m_Str);
                    ImGui::SameLine();
                    ImGui::PushID(str);
                    if(ImGui::Button("Set")) {
                        nodwindow->m_ParentNod = nod;
                        nodwindow->m_TargetMember = (SMwClassMemberInfo*)member;
                        nodwindow->m_IsSetIdDialog = true;
                        nodwindow->m_IdBuffer.clear();
                    }
                    ImGui::PopID();
                    //printf("EO ID\n");
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
                    int res = nod->VirtualParam_Get(stacc, (void**)&vec);
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
            //printf("EO %08X\n", member->memberId); // debug
            continue;
        }
        //////////////////////////
        // Non-virtual
        //////////////////////////
        //printf("Non-virtual member\n");
        ImGui::PushID((int)member+(int)nod);
        switch(member_type)
        {
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
                //if(member->flags & SMwMemberInfo::eFlags::ADD || member->flags & SMwMemberInfo::eFlags::VIRTUAL_ADD) {
                    if(ImGui::Button("Add"))
                    {
                        nodwindow->m_ParentNod = nod;
                        nodwindow->m_TargetMember = (SMwClassMemberInfo*)member;
                        nodwindow->m_IsAddDialog = true;
                        nodwindow->m_TargetMemberIndex = buf->numElems - 1;
                    }
                //}
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
                        nodwindow->m_TargetMemberIndex = buf->numElems - 1;
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
    //printf("EO DoClassAuto\n");
}


/////////////////////////////////
// Custom
/////////////////////////////////


void DoCMwNod(CMwNod* nod, TMStuff::MwNodWindow* nodwindow, CMwClassInfo* curclassinfo)
{
    DoClassAuto(nod, curclassinfo, nodwindow);

    ImGui::InputInt("ReferenceCount", (int*)&nod->m_ReferenceCount, 1, 1, 0);
    DoMemberClass(nod->m_SystemFid, "Fid", "CSystemFid", nodwindow);
    if(nod->m_SystemFid) {
        ImGui::SameLine();
        if(ImGui::Button("Delete")) {
            nod->m_SystemFid->Delete(1);
            nod->m_SystemFid = NULL;
        }
    }
    SFastBuffer<CMwNod*>* buf = nod->m_Dependants;
    if(buf) {
        if(ImGui::TreeNode(buf, "%s<%s> (%i)", "Dependants", "CMwNod", buf->numElems)) {
            for(int j=0;j<buf->numElems;j++) {
                CMwNod* new_nod = buf->pElems[j];
                ImGui::PushID(j);
                ImGui::Text("\t%i: %s @ %08X", j, new_nod->MwGetClassInfo()->m_ClassName, new_nod);
                ImGui::SameLine();
                TMStuff::DoNodCopyButton(new_nod, true);
                ImGui::PopID();
            }
            ImGui::TreePop();
        }
    } else {
        ImGui::Text("Dependants (NULL)");
    }
}

void DoCFuncKeysReal(CMwNod* nod, TMStuff::MwNodWindow* nodwindow){
    DoClassAuto(nod, nod->MwGetClassInfo(), nodwindow);
    // custom display
    CFastArray<Real>* arr = (CFastArray<Real>*)(int(nod) + 0x24);
    ImGui::PlotLines("Graph", arr->pElems, arr->numElems);
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

void DoCTrackManiaControlPlayerInput(CMwNod* nod, TMStuff::MwNodWindow* nodwindow)
{
    // additional data not in member infos
    ImGui::PushID((int)nod+69423); // nice
    SFastBuffer<CMwNod*>* buf = (SFastBuffer<CMwNod*>*)((int)nod + 0x4c);

    if(ImGui::TreeNode(buf, "%s<%s> (%i)", "PocLinks", "CHmsPocLink", buf->numElems)) {
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
            DoMemberClass(new_nod, idname, class_name, nodwindow);
            ImGui::PopID();
        }
        ImGui::TreePop();
    }

    CMwNod** new_nod = (CMwNod**)((int)nod + 0x24);
    DoMemberClass(*new_nod, "FastCall", "CMwCmdFastCall", nodwindow);

    Natural* nval = (Natural*)((int)nod + 0x48);
    ImGui::InputInt("SomeValue", (int*)nval, 1, 1, 0);

    ImGui::PopID();
}

void DoCSceneToyVehicleTrackMania(CMwNod* nod, TMStuff::MwNodWindow* nodwindow)
{
    DoClassAuto(nod, nod->MwGetClassInfo(), nodwindow);
    // additional data not in member infos
    ImGui::PushID((int)nod+69424); // nice
    int* horn_count =  (int*)((int)nod + 0x204);
    ImGui::InputInt("HornCount", horn_count, 1, 1, 0);
    ImGui::PopID();
}

void DoCHmsZone(CMwNod* nod, TMStuff::MwNodWindow* nodwindow)
{
    DoClassAuto(nod, nod->MwGetClassInfo(), nodwindow);
    // additional data not in member infos
    ImGui::PushID((int)nod+69426);

    DoMemberClass(*(CMwNod**)((int)nod + 0x14), "Sector", "CSceneSector", nodwindow);

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
                DoCMwNod(nod, nodwindow, nod_class_info);
                break;
            }
            case 0x03060000: { //CWindRacer
                DoClassAuto(nod, nod_class_info, nodwindow);
                ImGui::PushID((int)nod+69425);
                int* stuff = (int*)((int)nod + 0x1b8);
                ImGui::InputInt("Multiplayer", stuff, 1, 1, 0);
                ImGui::PopID();
                break;
            }
            case 0x0501a000: {
                DoCFuncKeysReal(nod, nodwindow);
                break;
            }
            case 0x06003000: {
                DoCHmsItem(nod, nodwindow);
                break;
            }
            //case 0x06004000: {
            //    DoCHmsZone(nod, nodwindow);
            //    break;
            //}
            case 0x0a103000: {
                DoCSceneToyVehicleTrackMania(nod, nodwindow);
                break;
            }
            case 0x24032000: {
                DoCTrackManiaControlPlayerInput(nod, nodwindow);
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
    CMwClassInfo* nod_info = nod->MwGetClassInfo();
    DoClass(nod, nod_info, nodwindow);
}

void DoNod(CMwNod* nod, TMStuff::MwNodWindowAddress* nodwindow)
{
    if(ImGui::Button("Save As")) {
        nodwindow->m_IsSaveDialog = true;
    }
    CMwClassInfo* nod_info = nod->MwGetClassInfo();
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
        ImGui::InputText("IdName", &this->m_IdBuffer);
        if(ImGui::Button("Yes##SetId")) {
            if(this->m_TargetMember) {
                if(this->m_TargetMember->memberId == 0x01001000)
                {
                    this->m_ParentNod->SetIdName((char*)this->m_IdBuffer.c_str());
                } else {
                    CFastString* new_str = (CFastString*)malloc(sizeof(CFastString));

                    new_str->m_Str = (char*)malloc(this->m_IdBuffer.length());
                    memcpy(new_str->m_Str, this->m_IdBuffer.c_str(), this->m_IdBuffer.length());
                    new_str->m_Size = this->m_IdBuffer.length();

                    GbxTools::VirtualParam_Set_Fast(this->m_ParentNod, this->m_TargetMember, (void**)m_IdBuffer.c_str());
                }

                //free(new_str);
            } else {
                this->m_ParentNod->SetIdName((char*)this->m_IdBuffer.c_str());
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
        if(this->m_TargetMember->fieldOffset != 0xFFFFFFFF && this->m_TargetMember->flags & 0b11110000) { // has a valid offset and virtual flags, make optional
            ImGui::SameLine();
            ImGui::Checkbox("Use Virtual Function", &this->m_UseVirtualFunction);
        }
        if(ImGui::Button("Yes##Set")) {
            CMwNod* target = (CMwNod*)this->m_SetAddressBuffer;
            if(this->m_IsSetDialog) {
                // Set dialog
                if(this->m_TargetMember->flags & SMwMemberInfo::eFlags::VIRTUAL_SET && this->m_UseVirtualFunction) {
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
                if(this->m_TargetMemberIndex < 0)
                    this->m_TargetMemberIndex = 0;
                if(this->m_TargetMember) {
                    if((this->m_TargetMember->flags & SMwMemberInfo::eFlags::VIRTUAL_ADD) && this->m_UseVirtualFunction) {
                        // Is virtual
                        GbxTools::VirtualParam_Add_SuperFast(this->m_ParentNod, (void**)target, 1, this->m_TargetMember);

                    } else {
                        if(this->m_TargetMember->type == SMwMemberInfo::CLASSARRAY) {
                            printf("Add to CFastArray\n");
                            int nod_offset = this->m_TargetMember->fieldOffset;
                            CFastArray<CMwNod*>* arr = (CFastArray<CMwNod*>*)((int)this->m_ParentNod + nod_offset);

                            //CFastArray<CMwNod*>* arr = (CFastArray<CMwNod*>*)(nod_valk + (m_offset / 4));
                            Natural new_size = arr->numElems + 1;
                            printf("old size: %i\nnew size: %i\n",arr->numElems, new_size);
                            CFastArray_ReAllocate(arr, new_size);
                            arr->pElems[new_size-1] = target;
                        } else if(this->m_TargetMember->type == SMwMemberInfo::CLASSBUFFER) {
                            //int nod_offset = this->m_TargetMember->fieldOffset;
                            //SFastBuffer<CMwNod*>* buff = (SFastBuffer<CMwNod*>*)((int)this->m_ParentNod + nod_offset);

                            //Natural new_size = buff->numElems + 1;
                            //if(buff->numElems >= buff->cap)
                            //    buff->cap = buff->cap + 1;


                        }
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
    this->m_AddressBuffer = (int)nod;
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
    if(this->m_Lookup && this->m_Nod) {
        if(this->m_Nod->m_SystemFid) {
            ImGui::SameLine();
            if(ImGui::Button("ReSave")) {
                GbxTools::ReSaveNod(this->m_Nod);
            }
        }
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

TMStuff::FidExplorerWindow::FidExplorerWindow(CSystemFidsDrive* drive)
{
    this->m_Drive = drive;
    this->m_Title = "Fid Explorer - GameData";
}

TMStuff::FidExplorerWindow::~FidExplorerWindow()
{}

bool DoSystemFids(CSystemFids* fids)
{
    for(int i=0;i<fids->m_Trees.numElems;i++)
    {
        CSystemFids* next_fids = fids->m_Trees.pElems[i];
        if(next_fids)
        {
            if(next_fids->MwIsKindOf(0x0b00b000))
            {
                if(ImGui::TreeNode(next_fids, "%s", CSystemFidsFolder::GetFolderName((CSystemFidsFolder*)next_fids)->m_Str))
                {
                    DoSystemFids(next_fids);
                    ImGui::TreePop();
                }
            }

        }
    }

    for(int i=0;i<fids->m_Leaves.numElems;i++)
    {
        char* prefix = "|-";
        if(i==fids->m_Leaves.numElems-1)
            prefix = "|_";
        CSystemFid* next_fid = fids->m_Leaves.pElems[i];
        if(next_fid)
        {
            ImGui::PushID(next_fid);
            if(next_fid->MwIsKindOf(0x0b00a000))
            {
                CSystemFidFile* next_fid_file = (CSystemFidFile*)next_fid;
                if(next_fid_file->m_Nod)
                {

                    if(ImGui::Button("Nod"))
                    {
                        TMStuff::NewNodAddressWindow(next_fid_file->m_Nod);
                    }
                    ImGui::SameLine();
                }
                else
                {
                    if(ImGui::Button("Preload"))
                    {
                        CMwNod* dummy = NULL;
                        GbxTools::LoadFromFid(&dummy, next_fid_file, 7);
                    }
                    ImGui::SameLine();
                }

                ImGui::Text("%s %s", prefix, next_fid_file->m_FileName.m_Str);
            }
            else
            {
                ImGui::Text("%s %i", prefix, i);
            }
            ImGui::PopID();
        }
    }
    return true;
}

bool TMStuff::FidExplorerWindow::Do(bool* p_open)
{
    // delete window
    if(!this->m_Open)
        return false;
    ImGui::PushID(this);
    ImGui::SetNextWindowSize(ImVec2(512, 512), ImGuiCond_FirstUseEver);
    ImGui::Begin(this->m_Title, p_open);

    if(ImGui::TreeNode(this->m_Drive, "GameData"))
    {
        DoSystemFids(this->m_Drive);
        ImGui::TreePop();
    }

    ImGui::End();
    ImGui::PopID();
    return true;
}
