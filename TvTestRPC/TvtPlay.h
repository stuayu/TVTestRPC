#pragma once

#include "stdafx.h"

// https://github.com/xtne6f/TvtPlay/blob/work/src/_getinfo_test.cpp

#define WM_TVTP_GET_POSITION (WM_APP + 52)

inline BOOL CALLBACK FindTvtPlayFrameEnumProc(const HWND hwnd, const LPARAM lParam)
{
    TCHAR className[32];
    if (GetClassName(hwnd, className, _countof(className)) && !lstrcmp(className, L"TvtPlay Frame"))
    {
        *reinterpret_cast<HWND*>(lParam) = hwnd;
        return false;
    }
    
    return true;
}

inline HWND FindTvtPlayFrame()
{
    HWND hwnd = nullptr;
    EnumThreadWindows(GetCurrentThreadId(), FindTvtPlayFrameEnumProc, reinterpret_cast<LPARAM>(&hwnd));
    
    return hwnd;
}

inline int GetTvtPlayPositionSec(const HWND hwnd)
{
    return static_cast<int>(SendMessage(hwnd, WM_TVTP_GET_POSITION, 0, 0)) / 1000;
}
