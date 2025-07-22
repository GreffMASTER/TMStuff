#ifndef TMSTUFFIMGUISTYLE_H_INCLUDED
#define TMSTUFFIMGUISTYLE_H_INCLUDED

#include "GameBox/include/GbxTools.h"

namespace TMStuff
{
    class CTMStuffImGuiStyle : public CMwNod
    {
        public:
            static CMwClassInfo m_MwClassInfo_TMStuffImGuiStyle;

            static void Init();
            static CTMStuffImGuiStyle * MwNewCTMStuffImGuiStyle();

            // """virtual""" methods
            static CTMStuffImGuiStyle* __thiscall vDelete(CTMStuffImGuiStyle* nod, byte purge);
            static CMwClassInfo * vMwGetClassInfo() {return &m_MwClassInfo_TMStuffImGuiStyle;};
            static int vGetMwClassId(void) {return 0x48002000;};
            static int __stdcall vMwIsKindOf(int classId) {
                if(classId==0x48002000) {
                    return 1;
                }
                return (classId == 0x01001000);
            };
            static CMwId* __thiscall vMwGetId(CTMStuffImGuiStyle* nod) {return &nod->m_IdName;};
            static int __stdcall vGetUidParamFromIndex(int index) {
                if(index == 0) {
                    return 0x01001000;
                }
                return index - 1 | 0x48002000;
            };
            static char __stdcall vGetParamCount(void) {
                return 4;
            };
            static void __thiscall vChunk(TMStuff::CTMStuffImGuiStyle* nod, CClassicArchive* pA, int chunkId);
            static int __stdcall vGetChunkInfo(int chunkId);
            static int __stdcall vGetUidChunkFromIndex(int index) {
                if(index == 0) {
                    return 0x01001000;
                }
                return index - 1 | 0x48002000;
            };
            static char __stdcall vGetChunkCount(void) {
                return 6;
            };

            static void __fastcall Capture(CTMStuffImGuiStyle* nod);
            static void __fastcall Set(CTMStuffImGuiStyle* nod);
            static void __fastcall Edit(CTMStuffImGuiStyle* nod);

            // members

            CMwId m_IdName;
            ImGuiStyle m_Style;
            bool m_ShowEditor;
    };
}

#endif // TMSTUFFIMGUISTYLE_H_INCLUDED
