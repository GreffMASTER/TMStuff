#define _GNU_SOURCE

#include "mainthread.h"
#include "GameBox/include/GbxTools.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_dx9.h"
#include "imgui/backends/imgui_impl_win32.h"
#include <tchar.h>
#include "minhook/include/MinHook.h"
#include "TMStuff.h"
#include <vector>
#include <fstream>

const unsigned long LOGSTRING		= 0x0097c0a4;  // CFastString *

HWND window = nullptr;

HMODULE hModule;

DrawIndexedPrimitive DrawIndexedPrimitive_orig = 0;
EndScene EndScene_orig = 0;
Reset Reset_orig = 0;
Present Present_orig = 0;
hPostQuitMessage PostQuitMessage_orig = 0;
WNDPROC oWndProc;
DWORD* dVtable;

char* str_version = "TMStuff 1.2_a3\ngreffmaster 2024/2025\n";
char* str_build = "Build: " __TIME__ ", " __DATE__ "\n";
bool show_info = false;
bool is_picking = false;
bool prev_is_mouse_down = false;
CMwNod* pick_corpus_buffer = nullptr;
CMwNod* pick_tree_buffer = nullptr;
int resave_succes = -1;
bool show_ids = false;
bool debug = false;
std::vector<TMStuff::MwNodWindow*> windowman;
FILE* f; // console out
CFastString* logfststr = (CFastString*)LOGSTRING;
char* logstr = nullptr;
char* resave_list_path = nullptr;
bool capture_input = true;

int fillmode_wired = 1;
int fillmode_solid = 0;
CMwNod* pick_shader = nullptr;
CMwNod* pick_shader_previous = nullptr;

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if(!TMStuff::m_ImGuiReady && capture_input)
        return CallWindowProcA(oWndProc, hWnd, uMsg, wParam, lParam);

	ImGuiIO& ImIo = ImGui::GetIO();
    LRESULT ImWndProcResult = ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

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
            return 0;
        }
    } else {
        if(ImWndProcResult) {
            return ImWndProcResult;
        }
    }

    return CallWindowProcA(oWndProc, hWnd, uMsg, wParam, lParam);
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
		return 0;
	}

	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL)
	{
		DestroyWindow(tmpWnd);
		return 0;
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
		return 0;
	}
    // hooks
	dVtable = (DWORD*)d3ddev;
	dVtable = (DWORD*)dVtable[0];

	EndScene_orig = (EndScene)dVtable[42];
	DrawIndexedPrimitive_orig = (DrawIndexedPrimitive)dVtable[82];
	Reset_orig = (Reset)dVtable[16];
	Present_orig = (Present)dVtable[17];
	PostQuitMessage_orig = PostQuitMessage;

	if (MH_Initialize() != MH_OK) { return 1; }
	// PostQuitMessave
	if (MH_CreateHook((DWORD_PTR*)PostQuitMessage, (LPVOID)&PostQuitMessage_hook, reinterpret_cast<void**>(&PostQuitMessage_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)PostQuitMessage) != MH_OK) { return 1; }
    // Reset
	if (MH_CreateHook((DWORD_PTR*)dVtable[16], (LPVOID)&Reset_hook, reinterpret_cast<void**>(&Reset_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[16]) != MH_OK) { return 1; }
	// Present
	if (MH_CreateHook((DWORD_PTR*)dVtable[17], (LPVOID)&Present_hook, reinterpret_cast<void**>(&Present_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[17]) != MH_OK) { return 1; }
	// EndScene
	if (MH_CreateHook((DWORD_PTR*)dVtable[42], (LPVOID)&EndScene_hook, reinterpret_cast<void**>(&EndScene_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[42]) != MH_OK) { return 1; }
	// DrawIndexedPrimitive
	if (MH_CreateHook((DWORD_PTR*)dVtable[82], (LPVOID)&DrawIndexedPrimitive_hook, reinterpret_cast<void**>(&DrawIndexedPrimitive_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[82]) != MH_OK) { return 1; }

	oWndProc = (WNDPROC)SetWindowLongPtr(win, GWL_WNDPROC, (LONG_PTR)WndProc);

	d3ddev->Release();
	d3d->Release();
	DestroyWindow(tmpWnd);

	return 1;
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

    return MH_Uninitialize();
}

bool menu;

TMStuff::MwNodWindow* nod_window1 = nullptr;
TMStuff::MwNodWindow* config_window = nullptr;

bool ResaveNods() {
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
        res = GbxTools::SaveNod(list_nod, line_c_str, GBX_COMPRESS | GBX_ISR );
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
            GbxTools::PushLog(logfststr->m_Str, logfststr->m_Size);
        }
        if (GetAsyncKeyState(VK_F3) & 0x80000) {
            TMStuff::m_Config->m_ShowUi = !TMStuff::m_Config->m_ShowUi;
            Sleep(100);
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

                if(ImGui::MenuItem("New Window", "", nullptr)) {
                    TMStuff::MwNodWindowAddress* new_window = new TMStuff::MwNodWindowAddress();
                    windowman.push_back(new_window);
                }

                if(ImGui::MenuItem("Close All", "", nullptr)) {
                    for(int i=0;i<windowman.size();i++) {
                        TMStuff::MwNodWindow* next_window = windowman[i];
                        if(next_window) {
                            delete next_window;
                        }
                    }
                    windowman.clear();
                }
                ImGui::EndMenu();
            }

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
                        if(ImGui::Button("Resave")) {
                            if(ResaveNods()) {
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

                // Do nod windows
                for(int i=0;i<windowman.size();i++)
                {
                    TMStuff::MwNodWindow* next_window = windowman[i];
                    if(next_window)
                        if(!next_window->Do(&next_window->m_Open)) {
                            windowman.erase(std::next(windowman.begin(), i));
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
        for(int i=0;i<windowman.size();i++) {
            TMStuff::MwNodWindow* next_window = windowman[i];
            if(next_window) {
                delete next_window;
            }
        }
        windowman.clear();

        if(pick_shader)
            GbxTools::MwDestroy(pick_shader);

        TMStuff::Terminate();

        printf("Shutting down ImGui...\n");
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        free(cwd);
        free(resave_list_path);
        GbxTools::DeInitLog();
        printf("Clean up complete. See ya next time!");
        fclose(f);
    }
    return PostQuitMessage_orig(iExitCode);
}

bool HoldTillGameInit()
{
    printf("Waiting for the game to finish starting...\n");
    while(true)
    {
        LPDIRECT3D9 d3d = GbxTools::GetD3D9();
        LPDIRECT3DDEVICE9 dev = GbxTools::GetD3DDevice9();
        if(d3d != nullptr && dev != nullptr)
            return true;
    }
}

DWORD WINAPI MainThread(LPVOID param) {
    freopen_s(&f, "CONOUT$", "w", stdout);
    GbxTools::InitLog("log.txt");
    HoldTillGameInit();
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

    InitHooks(hwnd);

    TMStuff::Init();

    nod_window1 = new TMStuff::MwNodWindow(GbxTools::GetTrackManiaNod());

    pick_shader = GbxTools::CreateByMwClassId(0x09026000);
    if(pick_shader)
        GbxTools::VirtualParam_Set_Fast(pick_shader, 0x09002002, (void**)&fillmode_wired);
    return 0;
}
