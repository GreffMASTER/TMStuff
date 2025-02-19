#ifndef __MWNODMAIN_H__
#define __MWNODMAIN_H__

#include "GbxTypes.h"

class CMwNodMain : public CMwNod
{
    public:
        CMwNod* m_Config;
        CMwNod* m_Window;
        HWND m_HWNDWindow;
        CFastString m_PathGameDir;
        CFastString m_GameGbxName;
        CGameProcess* m_Game;
        CMwNod* m_Window2;
        CVisionViewportDx9* m_Viewport;
        Natural u1;
};

#endif // __MWNODMAIN_H__
