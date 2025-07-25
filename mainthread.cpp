#define _GNU_SOURCE

#include "mainthread.h"
#include "GameBox/include/GbxTools.h"
#include "GameBox/include/DumpTools.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_dx9.h"
#include "imgui/backends/imgui_impl_win32.h"
#include <tchar.h>
#include "minhook/include/MinHook.h"
#include "TMStuff.h"
#include <fstream>


const unsigned long LOGSTRING		= 0x0097c0a4;  // CFastString *
#define WINDRACER_MAP 0x0045d960
void (__stdcall* WindRacerMap)(void) = (void (__stdcall* )(void))WINDRACER_MAP;


HWND window = nullptr;

HMODULE hModule;

DrawIndexedPrimitive DrawIndexedPrimitive_orig = 0;
EndScene EndScene_orig = 0;
Reset Reset_orig = 0;
Present Present_orig = 0;
hPostQuitMessage PostQuitMessage_orig = 0;
LoadFile oLoadFile = 0;
CreateEngines oCreateEngines = 0;


WNDPROC oWndProc;
DWORD* dVtable;

char* str_version = "TMStuff 1.2_a6\ngreffmaster 2024/2025\n";
char* str_build = "Build: " __TIME__ ", " __DATE__ "\n";
bool windracer_mapped = false;
bool show_info = false;
bool is_picking = false;
bool prev_is_mouse_down = false;
bool ask_dump_chunks = false;
CMwNod* pick_corpus_buffer = nullptr;
CMwNod* pick_tree_buffer = nullptr;

std::vector<TMStuff::MwNodWindow*> TMStuff::windowman;

int resave_succes = -1;
bool resave_compress = true;
bool resave_text = false;
bool resave_release = true;
bool resave_embed = false;

bool show_fid_explorer = false;
bool show_ids = false;
bool debug = false;
extern FILE* f; // console out
CFastString* logfststr = (CFastString*)LOGSTRING;
char* logstr = nullptr;
char* resave_list_path = nullptr;
bool capture_input = true;

int fillmode_wired = 1;
int fillmode_solid = 0;
CMwNod* pick_shader = nullptr;
CMwNod* pick_shader_previous = nullptr;

TMStuff::FidExplorerWindow* FidExplorer;

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if(!TMStuff::m_ImGuiReady && capture_input)
        return CallWindowProcA(oWndProc, hWnd, uMsg, wParam, lParam);

	ImGuiIO& ImIo = ImGui::GetIO();
    LRESULT ImWndProcResult = ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

    if(uMsg == WM_KEYDOWN) {
        bool isRepeat = (lParam & 0xFF000000);
        if(!isRepeat) { // ONLY ONCE
            switch(wParam) {
                /*case VK_F1: {
                    printf("Testing stack\n");
                    CMwNod* test = GbxTools::GetTrackManiaNod();
                    CMwStack* stacc = CMwStack::NewCMwStack();
                    CFastString* str = CFastString::NewCFastString("Editor.Cursor.CursorMobil.Item.Corpuss[0].Location");

                    //CFastString* str = CFastString::NewCFastString("VersionFeatures");
                    printf("\"%s\"\n", str->m_Str);

                    int res = CMwStack::Push(stacc, 0xFFFFFFFF, test, str);
                    printf("res: %i\n", res);
                    if(res == 0) {
                        printf("Success!\n");
                        printf("stacc size: %i, pos: %i\n", stacc->m_Size, stacc->iCurrentPos);
                        for(int i=0;i<stacc->m_Size;i++) {
                            int type = stacc->ppTypes[i];
                            printf("\t%i:\n", i+1);
                            printf("\t\ttype: %i\n", type);
                            switch(type) {
                                case 0: { //member
                                    SMwMemberInfo* member = stacc->ppMemberInfos[i];
                                    //printf("\t%s\n", member);
                                    printf("\t\tname: %s (%08X)\n", member->pszName, member->memberId);
                                    break;
                                }
                                case 1: { //index
                                    int index = (int)stacc->ppMemberInfos[i];
                                    printf("\t\tindex: %i \n", index);
                                }
                            }
                        }

                        int res1 = CMwNod::Param_Check(test, stacc);
                        if(res1 == 0) {
                            printf("Param_Check is good\n");
                            stacc->iCurrentPos = stacc->m_Size - 1;
                            GmIso4* val;
                            int res2 = CMwNod::Param_Get(test, stacc, (void**)&val);
                            if(res2)
                                printf("param get failed\n");
                            else
                                printf("value=%f\n", val->X);
                        }

                    }

                    stacc->Delete(1);
                    str->Delete(1);
                    break;
                }*/
                case VK_F3: {
                    TMStuff::m_Config->m_ShowUi = !TMStuff::m_Config->m_ShowUi;
                    break;
                }
                case VK_F9: {
                    // dump it
                    ask_dump_chunks = true;
                    //DumpTools::DumpChunkInfos("TM1.0_ChunkInfos.xml");
                    break;
                }
            }
        }
    }
    if(!TMStuff::m_Config->m_ShowUi)
        return CallWindowProcA(oWndProc, hWnd, uMsg, wParam, lParam);
    if(uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST) {
        // Test Key Input
        // To prevent typing while using ImGui interface
        if(ImWndProcResult) {
            return ImWndProcResult;
        }
        if(ImIo.WantCaptureKeyboard || !capture_input) {
            return 0;
        }
    } else if(uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST) {
        // Test Mouse Input
        // To prevent clicking while using ImGui interface
        if(ImWndProcResult) {
            return ImWndProcResult;
        }
        if(ImIo.WantCaptureMouse || !capture_input) {
            // TODO
            // Make the game ALWAYS capture mouse movement, to prevent the game from HIDING THE FUCKING CURSOR!
            return 0;
        }
    } else {
        if(ImWndProcResult) {
            return ImWndProcResult;
        }
    }

    return CallWindowProcA(oWndProc, hWnd, uMsg, wParam, lParam);
}

int hLoadFile(char* path, CMwNod** nod, unsigned long Cid, int EArchive, int flags)
{
    printf("Calling CSystemArchiveNod::LoadFile(\"%s\", 0x%08X)...\n", path, Cid);
    if(!windracer_mapped)
    {
        WindRacerMap();
        printf("WindRacer directories mapped!\n");
        windracer_mapped = true;
    }

    //int res = GbxTools::LoadNod(nod, path);
    int res = oLoadFile(path, nod, Cid, EArchive, flags);
    printf("CSystemArchiveNod::LoadFile returned %i\n", res);
    return res;
}

void CreateEngines_hook(CMwNod* nod, HINSTANCE inst)
{
    printf("CreateEngines called\n");
    oCreateEngines(nod, inst);

    CClassicLog::Setup(CClassicLog::GetClassicLog(), "LOG.txt", 1);
    TMStuff::Init();

    return;
}

DWORD WINAPI InitHooksPre()
{
	if (MH_CreateHook((LPVOID)0x00401240, (LPVOID)&CreateEngines_hook, reinterpret_cast<void**>(&oCreateEngines)) != MH_OK) { return 1; }
	if (MH_EnableHook((LPVOID)0x00401240) != MH_OK) { return 1; }

	if (MH_CreateHook((LPVOID)0x0061bb40, (LPVOID)&hLoadFile, reinterpret_cast<void**>(&oLoadFile)) != MH_OK) { return 1; }
	if (MH_EnableHook((LPVOID)0x0061bb40) != MH_OK) { return 1; }

    return 0;
}

DWORD WINAPI InitHooks(HWND win)
{
	while (GetModuleHandle("d3d9.dll") == 0)
	{
		Sleep(100);
	}

	LPDIRECT3D9 d3d = NULL;
	LPDIRECT3DDEVICE9 d3ddev = NULL;
    window = win;
	HWND tmpWnd = CreateWindowA("BUTTON", "Temp Window", WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, NULL, NULL, hModule, NULL);
	if (tmpWnd == NULL)
	{
		return 1;
	}

	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL)
	{
		DestroyWindow(tmpWnd);
		return 1;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = tmpWnd;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	HRESULT result = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, tmpWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
	if (result != D3D_OK)
	{
		d3d->Release();
		DestroyWindow(tmpWnd);
		return 1;
	}
    // hooks
	dVtable = (DWORD*)d3ddev;
	dVtable = (DWORD*)dVtable[0];

	EndScene_orig = (EndScene)dVtable[42];
	DrawIndexedPrimitive_orig = (DrawIndexedPrimitive)dVtable[82];
	Reset_orig = (Reset)dVtable[16];
	Present_orig = (Present)dVtable[17];
	PostQuitMessage_orig = PostQuitMessage;

	// PostQuitMessave
	if (MH_CreateHook((DWORD_PTR*)PostQuitMessage, (LPVOID)&PostQuitMessage_hook, reinterpret_cast<void**>(&PostQuitMessage_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)PostQuitMessage) != MH_OK) { return 1; }
	printf("PostQuitMessage=OK\n");
    // Reset
	if (MH_CreateHook((DWORD_PTR*)dVtable[16], (LPVOID)&Reset_hook, reinterpret_cast<void**>(&Reset_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[16]) != MH_OK) { return 1; }
    printf("d3d->Reset=OK\n");
	// Present
	if (MH_CreateHook((DWORD_PTR*)dVtable[17], (LPVOID)&Present_hook, reinterpret_cast<void**>(&Present_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[17]) != MH_OK) { return 1; }
	printf("d3d->Present=OK\n");
	// EndScene
	if (MH_CreateHook((DWORD_PTR*)dVtable[42], (LPVOID)&EndScene_hook, reinterpret_cast<void**>(&EndScene_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[42]) != MH_OK) { return 1; }
	printf("d3d->EndScene=OK\n");
	// DrawIndexedPrimitive
	if (MH_CreateHook((DWORD_PTR*)dVtable[82], (LPVOID)&DrawIndexedPrimitive_hook, reinterpret_cast<void**>(&DrawIndexedPrimitive_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[82]) != MH_OK) { return 1; }
    printf("d3d->DrawIndexedPrimitive=OK\n");

	printf("MH_EnableHooks=OK\n");

	oWndProc = (WNDPROC)SetWindowLongPtr(win, GWL_WNDPROC, (LONG_PTR)WndProc);

	d3ddev->Release();
	d3d->Release();
	DestroyWindow(tmpWnd);

	return 0;
}

DWORD WINAPI UninitHooks()
{
    MH_DisableHook((DWORD_PTR*)PostQuitMessage);
    MH_RemoveHook((DWORD_PTR*)PostQuitMessage);

    MH_DisableHook((DWORD_PTR*)dVtable[16]);
    MH_RemoveHook((DWORD_PTR*)dVtable[16]);

    MH_DisableHook((DWORD_PTR*)dVtable[17]);
    MH_RemoveHook((DWORD_PTR*)dVtable[17]);

    MH_DisableHook((DWORD_PTR*)dVtable[42]);
    MH_RemoveHook((DWORD_PTR*)dVtable[42]);

    MH_DisableHook((DWORD_PTR*)dVtable[82]);
    MH_RemoveHook((DWORD_PTR*)dVtable[82]);
    // LoadFile
    MH_DisableHook((DWORD_PTR*)0x00401240);
    MH_RemoveHook((DWORD_PTR*)0x00401240);
    // CreateEngines
    MH_DisableHook((DWORD_PTR*)0x0061bb40);
    MH_RemoveHook((DWORD_PTR*)0x0061bb4);

    return MH_Uninitialize();
}

bool menu;

TMStuff::MwNodWindow* nod_window1 = nullptr;
TMStuff::MwNodWindow* config_window = nullptr;

bool ResaveNods(int flags) {
    bool succ = true;
    std::ifstream infile(resave_list_path);
    if(!infile.good()) {
        printf("Failed to open \"%s\"\n", resave_list_path);
        return false;
    }
    std::string line;
    char* line_c_str = nullptr;
    while (std::getline(infile, line))
    {
        line_c_str = (char*)line.c_str();
        //printf("Loading \"%s\"\n", line_c_str);
        CMwNod* list_nod = nullptr;
        int res = GbxTools::LoadNod(&list_nod, line_c_str);
        if(res != 1 || list_nod == nullptr) {
            printf("Failed to read \"%s\"\n", line_c_str);
            succ = false;
            continue;
        }
        res = GbxTools::SaveNod(list_nod, line_c_str, flags );
        if(res != 1) {
            printf("Failed to save \"%s\"\n", line_c_str);
            succ = false;
        }
    }
    infile.close();
    return succ;
}

static bool init = true;

HRESULT APIENTRY Reset_hook(LPDIRECT3DDEVICE9 pD3D9, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT ResetReturn = Reset_orig(pD3D9, pPresentationParameters);
    ImGui_ImplDX9_CreateDeviceObjects();
	return ResetReturn;
}

HRESULT APIENTRY Present_hook(LPDIRECT3DDEVICE9 pD3D9, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
    if (init && pD3D9 && window)
    {
        init = false;
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplWin32_Init(window);
        ImGui_ImplDX9_Init(pD3D9);
        TMStuff::InitImGui();
        config_window = new TMStuff::MwNodWindow(TMStuff::m_Config, "Settings");
    }

    if(TMStuff::m_Ready)
    {
        // Update
        if (logstr != logfststr->m_Str) {
            logstr = logfststr->m_Str;
            printf("[Game] %s", logstr);
        }
        // Draw
        if(TMStuff::m_Config->m_ShowUi && pD3D9 && window)
        {

            ImGui_ImplDX9_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            ImGui::BeginMainMenuBar();

            if(ImGui::BeginMenu("TMStuff")) {
                if(ImGui::MenuItem("About", "", show_info)) {
                    show_info = !show_info;
                }
                if(ImGui::MenuItem("Settings", "", TMStuff::m_Config->m_ShowConfig)) {
                    TMStuff::m_Config->m_ShowConfig = !TMStuff::m_Config->m_ShowConfig;
                }
                ImGui::MenuItem("Debug", "", &debug);
                ImGui::EndMenu();
            }

            if(ImGui::MenuItem("Resave", "", TMStuff::m_Config->m_ShowResave)) {
                TMStuff::m_Config->m_ShowResave = !TMStuff::m_Config->m_ShowResave;
            }

            if(ImGui::BeginMenu("Windows")) {
                if(ImGui::MenuItem("TrackMania Nod", "", TMStuff::m_Config->m_ShowTrackManiaNod)) {
                    TMStuff::m_Config->m_ShowTrackManiaNod = !TMStuff::m_Config->m_ShowTrackManiaNod;
                }

                ImGui::MenuItem("Fid Explorer", "", &show_fid_explorer);

                if(ImGui::MenuItem("New Window", "", nullptr)) {
                    TMStuff::MwNodWindowAddress* new_window = new TMStuff::MwNodWindowAddress();
                    TMStuff::windowman.push_back(new_window);
                }

                if(ImGui::MenuItem("Close All", "", nullptr)) {
                    for(int i=0;i<TMStuff::windowman.size();i++) {
                        TMStuff::MwNodWindow* next_window = TMStuff::windowman[i];
                        if(next_window) {
                            delete next_window;
                        }
                    }
                    TMStuff::windowman.clear();
                }
                ImGui::EndMenu();
            }

            //ImGui::ShowDemoWindow();

            if(ImGui::MenuItem("Picker", "", TMStuff::m_Config->m_ShowPicker)) {
                TMStuff::m_Config->m_ShowPicker = !TMStuff::m_Config->m_ShowPicker;
            }

            ImGui::MenuItem("Capture Input", "", &capture_input);
            ImGui::EndMainMenuBar();

            if(!is_picking) {
                if(TMStuff::m_Config->m_ImGuiStyle->m_ShowEditor) {
                    ImGui::ShowStyleEditor(&TMStuff::m_Config->m_ImGuiStyle->m_Style);
                }

                if(debug)
                    ImGui::ShowDebugLogWindow(&debug);

                if(show_fid_explorer)
                    FidExplorer->Do(&show_fid_explorer);

                if(TMStuff::m_Config->m_ShowTrackManiaNod) // Main TM nod
                    nod_window1->Do((bool*)&TMStuff::m_Config->m_ShowTrackManiaNod);

                if(show_info) {
                    ImGui::Begin("About TMStuff", &show_info, ImGuiWindowFlags_AlwaysAutoResize);
                    ImGui::Text(str_version, "");
                    ImGui::Text(str_build, "");
                    ImGui::End();
                }

                if(TMStuff::m_Config->m_ShowConfig) {
                    if(!config_window->Do((bool*)&TMStuff::m_Config->m_ShowConfig)) {
                        TMStuff::m_Config->m_ImGuiStyle->m_ShowEditor = false;
                    }
                }

                if(TMStuff::m_Config->m_ShowResave) {
                    ImGui::Begin("Resave Nods", (bool*)&TMStuff::m_Config->m_ShowResave, ImGuiWindowFlags_AlwaysAutoResize);
                        ImGui::Text("Usage: provide path to a text file containing list of GBX paths you want to resave.\n"
                                    "The files are read from the \"GameData\" directory\n"
                                    "Example: Speed/Media/Solid/SpeedRoadBaseAir.Solid.Gbx\n"
                                    "         Vehicles/Mobil/Vehicle/TrackManiaVehicle/American.TrackManiaVehicle.Gbx\n");
                        ImGui::InputText("List file path", resave_list_path, 256);
                        ImGui::Checkbox("Compress##Save", &resave_compress);
                        ImGui::SameLine();
                        ImGui::Checkbox("Text##Save", &resave_text);
                        ImGui::SameLine();
                        ImGui::Checkbox("Release##Save", &resave_release);
                        ImGui::SameLine();
                        ImGui::Checkbox("Embed##Save", &resave_embed);

                        if(ImGui::Button("Resave")) {
                            int flags = 0;
                            if(resave_compress)
                                flags |= GBX_COMPRESS;
                            if(resave_text)
                                flags |= GBX_ISTEXT;
                            if(resave_release)
                                flags |= GBX_ISR;
                            if(resave_embed)
                                flags |= GBX_EMBEDDED;

                            if(ResaveNods(flags)) {
                                resave_succes = 1;
                            } else {
                                resave_succes = 0;
                            }
                        }
                        if(resave_succes >= 0) {
                            if(resave_succes)
                                ImGui::Text("Resaved successfully!");
                            else
                                ImGui::Text("Failed to resave one or few nods. Check log.");
                        }
                    ImGui::End();
                }

                if(show_ids) {
                    ImGui::Begin("Lookback Strings");
                    SFastBuffer<CFastString>* buf = GbxTools::GetLookbackStrings();
                    for(int j=0;j<buf->numElems;j++)
                    {
                        ImGui::PushID(j);
                        ImGui::Text("%i: ", j);
                        ImGui::SameLine();
                        CFastString* str = &buf->pElems[j];
                        ImGui::InputText("", str->m_Str, str->m_Size);
                        ImGui::PopID();
                    }
                    ImGui::End();
                }

                if(ask_dump_chunks) {
                    ImGui::Begin("Dump chunk infos", &ask_dump_chunks, ImGuiWindowFlags_AlwaysAutoResize);
                    ImGui::Text("Dumps chunk infos of all classes.\n"
                                "WARNING! THIS *WILL* CRASH THE GAME AFTER COMPLETION!\n"
                                "Are You Sure?");
                    if(ImGui::Button("No"))
                        ask_dump_chunks = false;
                    ImGui::SameLine();
                    if(ImGui::Button("Yes"))
                        DumpTools::DumpChunkInfos("TM1.0_ChunkInfos.xml");
                    ImGui::End();
                }



                // Do nod windows
                for(int i=0;i<TMStuff::windowman.size();i++)
                {
                    TMStuff::MwNodWindow* next_window = TMStuff::windowman[i];
                    if(next_window)
                        if(!next_window->Do(&next_window->m_Open)) {
                            TMStuff::windowman.erase(std::next(TMStuff::windowman.begin(), i));
                            delete next_window;
                        }
                }
            }

            if(TMStuff::m_Config->m_ShowPicker) {
                ImGui::Begin("Hms Picker", (bool*)&TMStuff::m_Config->m_ShowPicker, ImGuiWindowFlags_AlwaysAutoResize);

                CMwNod* TM = GbxTools::GetTrackManiaNod();
                CMwNod* Scene = *(CMwNod**)((int)TM + 0x000000b0);
                CMwNod* Viewport = *(CMwNod**)((int)TM + 0x00000044);
                CMwNod* Picker = *(CMwNod**)((int)Viewport + 0x0000017c);
                CMwNod* Corpus = *(CMwNod**)((int)Picker + 0x000000b0);
                CMwNod* Tree = *(CMwNod**)((int)Picker + 0x000000b4);

                if(!Scene) {
                    ImGui::Text("No Scene3d detected. Load a map to use this tool.");
                } else {
                    if(is_picking) {
                        if(ImGui::Button("Cancel picking")) {
                            is_picking = false;
                            // Make all cameras PickEnable = false
                            SFastBuffer<CMwNod*>* Cameras = nullptr;
                            GbxTools::VirtualParam_Get_Fast(Viewport, 0x0601002a, (void**)&Cameras);
                            if(Cameras) {
                                for(int i=0; i<Cameras->numElems; i++) {
                                    CMwNod* Camera = Cameras->pElems[i];
                                    int* tog_val = (int*)((int)Camera + 0x00000110);
                                    *tog_val = 0;
                                }
                            }
                        }
                        ImGui::Text("Corpus @ %08X", Corpus);
                        ImGui::Text("Tree @ %08X", Tree);
                    } else {
                        if(ImGui::Button("Begin picking")) {
                            is_picking = true;
                            // Make all cameras PickEnable = true
                            SFastBuffer<CMwNod*>* Cameras = nullptr;
                            GbxTools::VirtualParam_Get_Fast(Viewport, 0x0601002a, (void**)&Cameras);
                            if(Cameras) {
                                for(int i=0; i<Cameras->numElems; i++) {
                                    CMwNod* Camera = Cameras->pElems[i];
                                    int* tog_val = (int*)((int)Camera + 0x00000110);
                                    *tog_val = 1;
                                }
                            }
                            // Make all models pickable
                            SFastBuffer<CMwNod*>* Mobils = (SFastBuffer<CMwNod*>*)((int)Scene + 0x00000014);
                            for(int i=0; i<Mobils->numElems; i++) {
                                CMwNod* Mobil = Mobils->pElems[i];
                                CMwNod* Item = *(CMwNod**)((int)Mobil + 0x0000003c);
                                if(Item) {
                                    CMwNod* Solid = *(CMwNod**)((int)Item + 0x00000054);
                                    if(Solid) {
                                        CMwNod* Tree = *(CMwNod**)((int)Solid + 0x00000068);
                                        if(Tree) {
                                            GbxTools::SetTreePickableRecursive(Tree);
                                        }
                                    }
                                }
                            }
                        }
                        ImGui::PushID("PickerCorpus");
                        ImGui::Text("Corpus @ %08X", pick_corpus_buffer);
                        ImGui::SameLine();
                        TMStuff::DoNodCopyButton(pick_corpus_buffer, false);
                        ImGui::PopID();
                        ImGui::PushID("PickerTree");
                        ImGui::Text("Tree @ %08X", pick_tree_buffer);
                        ImGui::SameLine();
                        TMStuff::DoNodCopyButton(pick_tree_buffer, false);
                        ImGui::PopID();
                    }
                }
                ImGui::End();
            }

            ImGui::GetIO().MouseDrawCursor = menu;

            // CHMSPICKER

            if(is_picking) {
                CMwNod* TM = GbxTools::GetTrackManiaNod();
                CMwNod* Viewport = *(CMwNod**)((int)TM + 0x00000044);
                CMwNod* Picker = *(CMwNod**)((int)Viewport + 0x0000017c);
                CMwNod* Corpus = *(CMwNod**)((int)Picker + 0x000000b0);
                CMwNod* Tree = *(CMwNod**)((int)Picker + 0x000000b4);

                if(Corpus != nullptr) {
                    pick_corpus_buffer = Corpus;
                }

                if(Tree != nullptr) {
                    if(pick_tree_buffer != Tree && pick_tree_buffer != nullptr) {
                        CMwNod** Shader = (CMwNod**)((int)pick_tree_buffer + 0x00000098);

                        if(pick_shader_previous) {
                            // reset wire
                            if(GbxTools::MwCheckThis(*Shader, sizeof(CMwNod*))) {
                                *Shader = pick_shader_previous;
                                //GbxTools::VirtualParam_Set_Fast(Shader, 0x09002002, (void**)&fillmode_solid);
                            }
                        }
                        // set new wire
                        Shader = (CMwNod**)((int)Tree + 0x00000098);
                        if(GbxTools::MwCheckThis(*Shader, sizeof(CMwNod*))) {
                            pick_shader_previous = *Shader;
                            *Shader = pick_shader;
                        }
                    }

                    pick_tree_buffer = Tree;
                } else if(pick_tree_buffer != nullptr) {
                    // reset wire

                    if(pick_shader_previous) {
                        // reset wire
                        CMwNod** Shader = (CMwNod**)((int)pick_tree_buffer + 0x00000098);
                        if(GbxTools::MwCheckThis(*Shader, sizeof(CMwNod*))) {
                            *Shader = pick_shader_previous;
                            //GbxTools::VirtualParam_Set_Fast(Shader, 0x09002002, (void**)&fillmode_solid);
                        }
                    }
                }

                if(!prev_is_mouse_down && ImGui::IsMouseDown(0)) { // Pick CHms
                    is_picking = false;

                    if(pick_shader_previous) {
                        // reset wire
                        CMwNod** Shader = (CMwNod**)((int)pick_tree_buffer + 0x00000098);
                        if(GbxTools::MwCheckThis(*Shader, sizeof(CMwNod*))) {
                            *Shader = pick_shader_previous;
                            //GbxTools::VirtualParam_Set_Fast(Shader, 0x09002002, (void**)&fillmode_solid);
                        }
                    }

                    // Make all cameras PickEnable = false
                    SFastBuffer<CMwNod*>* Cameras = nullptr;
                    GbxTools::VirtualParam_Get_Fast(Viewport, 0x0601002a, (void**)&Cameras);
                    if(Cameras) {
                        for(int i=0; i<Cameras->numElems; i++) {
                            CMwNod* Camera = Cameras->pElems[i];
                            int* tog_val = (int*)((int)Camera + 0x00000110);
                            *tog_val = 0;
                        }
                    }
                }
            }
            prev_is_mouse_down = ImGui::IsMouseDown(0);
            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        }
    }
    return Present_orig(pD3D9, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT APIENTRY EndScene_hook(LPDIRECT3DDEVICE9 pD3D9)
{
    return EndScene_orig(pD3D9);
}

HRESULT APIENTRY DrawIndexedPrimitive_hook(LPDIRECT3DDEVICE9 pD3D9, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
    return DrawIndexedPrimitive_orig(pD3D9, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

char* cwd = 0;

void APIENTRY PostQuitMessage_hook(int iExitCode)
{
    if(TMStuff::m_Ready) // check cuz PostQuitMessage is called multiple times
    {
        printf("PostQuitMessage detected, shutting down...\n");
        printf("Freeing windows...\n");
        delete nod_window1;
        delete FidExplorer;
        for(int i=0;i<TMStuff::windowman.size();i++) {
            TMStuff::MwNodWindow* next_window = TMStuff::windowman[i];
            if(next_window) {
                delete next_window;
            }
        }
        TMStuff::windowman.clear();

        if(pick_shader)
            GbxTools::MwDestroy(pick_shader);

        TMStuff::Terminate();

        printf("Shutting down ImGui...\n");
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        free(cwd);
        free(resave_list_path);
        printf("Clean up complete. See ya next time!");

    }
    return PostQuitMessage_orig(iExitCode);
}

bool HoldTillGameInit()
{
    while(true)
    {
        LPDIRECT3D9 d3d = GbxTools::GetD3D9();
        LPDIRECT3DDEVICE9 dev = GbxTools::GetD3DDevice9();
        if(d3d != nullptr && dev != nullptr)
            return true;
    }
}

DWORD WINAPI MainThread(LPVOID param) {
    printf("Waiting for the game to create engines...\n");
    while(true)
    {
        if(TMStuff::m_Ready)
        {
            printf("TMStuff ready\n");
            break;
        }

    }

    printf("Waiting for the game to finish starting...\n");
    HoldTillGameInit();
    printf("DONE\n");

    cwd = (char*)malloc(256);
    GetCurrentDirectoryA(256, cwd);
    resave_list_path = (char*)malloc(256 * sizeof(char));
    memset(resave_list_path, 0, 256);

    printf("%s\n", cwd);
    printf("Game started!\n");
    printf(str_version);
    printf(str_build);


    CMwNodMain* main = GbxTools::GetMainNod();
    HWND hwnd = main->m_HWNDWindow;

    printf("InitHooks...\n");
    InitHooks(hwnd);
    printf("DONE\n");

    WindRacerMap();

    nod_window1 = new TMStuff::MwNodWindow(GbxTools::GetTrackManiaNod());
    FidExplorer = new TMStuff::FidExplorerWindow(GbxTools::GetGameDataDrive());

    pick_shader = GbxTools::CreateByMwClassId(0x09026000);
    if(pick_shader)
        GbxTools::VirtualParam_Set_Fast(pick_shader, 0x09002002, (void**)&fillmode_wired);
    return 0;
}
