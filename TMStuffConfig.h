#ifndef TMSTUFFCONFIG_H_INCLUDED
#define TMSTUFFCONFIG_H_INCLUDED

#include "GameBox/include/GbxTools.h"
#include "TMStuffImGuiStyle.h"

class TMStuff::CTMStuffImGuiStyle;

namespace TMStuff
{
    class CTMStuffConfig : public CMwNod
    {
        public:
            static CMwClassInfo m_MwClassInfo_TMStuffConfig;

            static void Init();
            static CTMStuffConfig * MwNewCTMStuffConfig();

            static CTMStuffConfig* __thiscall vDelete(CTMStuffConfig* nod, byte purge);
            static CMwClassInfo * vMwGetClassInfo() {return &m_MwClassInfo_TMStuffConfig;};
            static int vGetMwClassId(void) {return 0x48001000;};
            static int __stdcall vMwIsKindOf(int classId) {
                if(classId==0x48001000) {
                    return 1;
                }
                return (classId == 0x01001000);
            };
            static int __stdcall vGetUidParamFromIndex(int index) {
                if(index == 0) {
                    return 0x01001000;
                }
                return index - 1 | 0x48001000;
            };
            static char __stdcall vGetParamCount(void) {
                return 6; // param count + 1
            };
            static void __thiscall vChunk(TMStuff::CTMStuffConfig* nod, CClassicArchive* pA, int chunkId);
            static int __stdcall vGetChunkInfo(int chunkId);
            static int __stdcall vGetUidChunkFromIndex(int index) {
                if(index == 0) {
                    return 0x01001000;
                }
                return index - 1 | 0x48001000;
            };
            static char __stdcall vGetChunkCount(void) {
                return 3; // chunk count + 1
            };
            static void __thiscall vOnNodLoaded(TMStuff::CTMStuffConfig* nod);

            // members
            CTMStuffImGuiStyle* m_ImGuiStyle;
            Bool m_ShowUi;
            Bool m_ShowTrackManiaNod;
            Bool m_ShowResave;
            Bool m_ShowPicker;
            Bool m_ShowConfig;
    };
}

#endif // TMSTUFFCONFIG_H_INCLUDED
