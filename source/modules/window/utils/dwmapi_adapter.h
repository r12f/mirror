#pragma once

#include <dwmapi.h>

HRESULT AdaDwmIsCompositionEnabled(BOOL* pfEnabled);
HRESULT AdaDwmEnableComposition(UINT uCompositionAction);
HRESULT AdaDwmGetWindowAttribute(HWND hwnd, DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute);
HRESULT AdaDwmSetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute );
HRESULT AdaDwmExtendFrameIntoClientArea(HWND hWnd, const MARGINS* pMarInset);
HRESULT AdaDwmGetTransportAttributes(BOOL *pfIsRemoting, BOOL *pfIsConnected, DWORD *pDwGeneration);
HRESULT AdaDwmGetColorizationColor(DWORD *pcrColorization, BOOL *pfOpaqueBlend);
HRESULT AdaDwmInvalidateIconicBitmaps(HWND hwnd);
HRESULT AdaDwmGetCompositionTimingInfo(HWND hwnd, DWM_TIMING_INFO *pTimingInfo);