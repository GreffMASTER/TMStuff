#pragma once
#ifndef __MAINTHREAD_H__
#define __MAINTHREAD_H__

#include <windows.h>
#include <iostream>
#include <d3d9.h>
#include "GameBox/include/GbxTypes.h"

#pragma comment (lib, "d3d9.lib" )
#pragma comment (lib, "d3dx9.lib" )

typedef void (APIENTRY* hPostQuitMessage)(int);
void APIENTRY PostQuitMessage_hook(int);

typedef HRESULT(APIENTRY* DrawIndexedPrimitive)(LPDIRECT3DDEVICE9, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
HRESULT APIENTRY DrawIndexedPrimitive_hook(LPDIRECT3DDEVICE9, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);

typedef HRESULT(APIENTRY* EndScene) (LPDIRECT3DDEVICE9);
HRESULT APIENTRY EndScene_hook(LPDIRECT3DDEVICE9);

typedef HRESULT(APIENTRY* Reset)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
HRESULT APIENTRY Reset_hook(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);

typedef HRESULT(APIENTRY* Present)(LPDIRECT3DDEVICE9, CONST RECT*, CONST RECT*,HWND,CONST RGNDATA*);
HRESULT APIENTRY Present_hook(LPDIRECT3DDEVICE9, CONST RECT*,CONST RECT*,HWND,CONST RGNDATA*);

typedef int(__cdecl* LoadFile)(char*, CMwNod**, unsigned long, int, int);
int __cdecl hLoadFile(char*, CMwNod**, unsigned long, int, int);

typedef void(__thiscall* CreateEngines)(CMwNod*, HINSTANCE);
void __thiscall CreateEngines_hook(CMwNod*, HINSTANCE);

typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

DWORD WINAPI InitHooksPre();

DWORD WINAPI UninitHooks();

DWORD WINAPI MainThread(LPVOID param);

#endif // __MAINTHREAD_H__
