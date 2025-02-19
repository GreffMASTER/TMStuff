#ifndef TMSTUFF_H_INCLUDED
#define TMSTUFF_H_INCLUDED

#include "GameBox/include/GbxTools.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "TMStuffConfig.h"
#include "TMStuffImGuiStyle.h"

namespace TMStuff
{
    class CTMStuffConfig;

    extern CTMStuffConfig* m_Config;
    extern bool m_Ready;
    extern char** m_EngineNames;
    extern int m_NumEngines;
    extern char*** m_ClassNames;
    extern int* m_NumClasses;
    extern CMwClassInfo*** m_Classes;
    extern char** m_DriveNames;

    void Init();
    void InitImGui();
    void Terminate();

    class MwNodWindow;

    void CopyToClipboard(char* str, int str_size);
    void DoNodCopyButton(CMwNod* nod, SMwClassMemberInfo* memberinfo, MwNodWindow* nodwindow, CMwNod* parent_nod);
    void DoNodCopyButton(CMwNod** nod, SMwClassMemberInfo* memberinfo, MwNodWindow* nodwindow, CMwNod* parent_nod);
    void DoNodCopyButton(CMwNod* nod, bool killable);

    class MwNodWindow
    {
    public:
        // methods
        MwNodWindow();
        MwNodWindow(CMwNod* nod);
        MwNodWindow(CMwNod* nod, char* title);
        virtual ~MwNodWindow();
        virtual bool Do(bool* p_open);
        virtual void Unreferenced() {printf("Node unreferenced!\n");};
        virtual bool SetNod(CMwNod* nod);
        bool DoSetWindow();
        void DoSetIdWindow();
        // members
        CMwNod* m_Nod;              // our nod we want to display

        SMwClassMemberInfo* m_TargetMember;
        CMwNod* m_ParentNod;
        char* m_Title;              // window title
        char* m_PathBuffer;
        int m_SetAddressBuffer;
        int m_EngineIndex = 0;
        int m_ClassIndex = 0;
        int m_DriveIndex = 0;
        int m_TargetMemberIndex = 0;
        bool m_Open;
        bool m_IsSaveDialog = false;
        bool m_IsLoadDialog = false;
        bool m_IsNewDialog = false;
        bool m_IsSetDialog = false;
        bool m_IsAddDialog = false;
        bool m_IsSetIdDialog = false;
        bool m_Compress = true;
        bool m_Text = false;
        bool m_R = false;
        bool m_Embed = false;
    };

    class MwNodWindowAddress : public MwNodWindow
    {
    public:
        // methods
        virtual bool Do(bool* p_open);
        virtual bool SetNod(CMwNod* nod);
        void SetupNod(CMwNod* nod);
        // members
        bool m_Lookup;
        int m_AddressBuffer;
        int m_ClassIdBuffer;
        void** m_VTableBuffer;
    };
}

#endif // TMSTUFF_H_INCLUDED
