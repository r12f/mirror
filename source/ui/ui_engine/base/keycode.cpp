#include "stdafx.h"
#include "ui/ui_engine/base/keycode.h"

KeyCode
NativeKeyToKeyCode(WORD nNativeCode)
{
    return static_cast<KeyCode>(nNativeCode);
}

WORD
KeyCodeToNativeKey(KeyCode nKeyCode)
{
    return static_cast<WORD>(nKeyCode);
}

struct KeyNameMap {
    KeyCode m_nKeyCode;
    const TCHAR * m_pKeyName;
};

static const KeyNameMap s_pKeyNameMap[] = {
    { VKEY_BACK, _T("Back") },
    { VKEY_TAB, _T("Tab") },
    { VKEY_CLEAR, _T("Clear") },
    { VKEY_RETURN, _T("Return") },
    { VKEY_SHIFT, _T("Shift") },
    { VKEY_CONTROL, _T("Control") },
    { VKEY_MENU, _T("Menu") },
    { VKEY_PAUSE, _T("Pause") },
    { VKEY_CAPITAL, _T("Capital") },
    { VKEY_KANA, _T("Kana") },
    { VKEY_HANGUL, _T("Hangul") },
    { VKEY_JUNJA, _T("Junja") },
    { VKEY_FINAL, _T("Final") },
    { VKEY_HANJA, _T("Hanja") },
    { VKEY_KANJI, _T("Kanji") },
    { VKEY_ESCAPE, _T("Escape") },
    { VKEY_CONVERT, _T("Convert") },
    { VKEY_NONCONVERT, _T("Nonconvert") },
    { VKEY_ACCEPT, _T("Accept") },
    { VKEY_MODECHANGE, _T("ModeChange") },
    { VKEY_SPACE, _T("Space") },
    { VKEY_PRIOR, _T("Prior") },
    { VKEY_NEXT, _T("Next") },
    { VKEY_END, _T("End") },
    { VKEY_HOME, _T("Home") },
    { VKEY_LEFT, _T("Left") },
    { VKEY_UP, _T("Up") },
    { VKEY_RIGHT, _T("Right") },
    { VKEY_DOWN, _T("Down") },
    { VKEY_SELECT, _T("Select") },
    { VKEY_PRINT, _T("Print") },
    { VKEY_EXECUTE, _T("Execute") },
    { VKEY_SNAPSHOT, _T("Snapshot") },
    { VKEY_INSERT, _T("Insert") },
    { VKEY_DELETE, _T("Delete") },
    { VKEY_HELP, _T("Help") },
    { VKEY_0, _T("0") },
    { VKEY_1, _T("1") },
    { VKEY_2, _T("2") },
    { VKEY_3, _T("3") },
    { VKEY_4, _T("4") },
    { VKEY_5, _T("5") },
    { VKEY_6, _T("6") },
    { VKEY_7, _T("7") },
    { VKEY_8, _T("8") },
    { VKEY_9, _T("9") },
    { VKEY_A, _T("A") },
    { VKEY_B, _T("B") },
    { VKEY_C, _T("C") },
    { VKEY_D, _T("D") },
    { VKEY_E, _T("E") },
    { VKEY_F, _T("F") },
    { VKEY_G, _T("G") },
    { VKEY_H, _T("H") },
    { VKEY_I, _T("I") },
    { VKEY_J, _T("J") },
    { VKEY_K, _T("K") },
    { VKEY_L, _T("L") },
    { VKEY_M, _T("M") },
    { VKEY_N, _T("N") },
    { VKEY_O, _T("O") },
    { VKEY_P, _T("P") },
    { VKEY_Q, _T("Q") },
    { VKEY_R, _T("R") },
    { VKEY_S, _T("S") },
    { VKEY_T, _T("T") },
    { VKEY_U, _T("U") },
    { VKEY_V, _T("V") },
    { VKEY_W, _T("W") },
    { VKEY_X, _T("X") },
    { VKEY_Y, _T("Y") },
    { VKEY_Z, _T("Z") },
    { VKEY_LWIN, _T("LWin") },
    { VKEY_COMMAND, _T("Command") },
    { VKEY_RWIN, _T("RWin") },
    { VKEY_APPS, _T("Apps") },
    { VKEY_SLEEP, _T("Sleep") },
    { VKEY_NUMPAD0, _T("Numpad 0") },
    { VKEY_NUMPAD1, _T("Numpad 1") },
    { VKEY_NUMPAD2, _T("Numpad 2") },
    { VKEY_NUMPAD3, _T("Numpad 3") },
    { VKEY_NUMPAD4, _T("Numpad 4") },
    { VKEY_NUMPAD5, _T("Numpad 5") },
    { VKEY_NUMPAD6, _T("Numpad 6") },
    { VKEY_NUMPAD7, _T("Numpad 7") },
    { VKEY_NUMPAD8, _T("Numpad 8") },
    { VKEY_NUMPAD9, _T("Numpad 9") },
    { VKEY_MULTIPLY, _T("Numpad *") },
    { VKEY_ADD, _T("Numpad +") },
    { VKEY_SEPARATOR, _T("Separator") },
    { VKEY_SUBTRACT, _T("Numpad -") },
    { VKEY_DECIMAL, _T("Numpad .") },
    { VKEY_DIVIDE, _T("Numpad /") },
    { VKEY_F1, _T("F1") },
    { VKEY_F2, _T("F2") },
    { VKEY_F3, _T("F3") },
    { VKEY_F4, _T("F4") },
    { VKEY_F5, _T("F5") },
    { VKEY_F6, _T("F6") },
    { VKEY_F7, _T("F7") },
    { VKEY_F8, _T("F8") },
    { VKEY_F9, _T("F9") },
    { VKEY_F10, _T("F10") },
    { VKEY_F11, _T("F11") },
    { VKEY_F12, _T("F12") },
    { VKEY_F13, _T("F13") },
    { VKEY_F14, _T("F14") },
    { VKEY_F15, _T("F15") },
    { VKEY_F16, _T("F16") },
    { VKEY_F17, _T("F17") },
    { VKEY_F18, _T("F18") },
    { VKEY_F19, _T("F19") },
    { VKEY_F20, _T("F20") },
    { VKEY_F21, _T("F21") },
    { VKEY_F22, _T("F22") },
    { VKEY_F23, _T("F23") },
    { VKEY_F24, _T("F24") },
    { VKEY_NUMLOCK, _T("Numlock") },
    { VKEY_SCROLL, _T("Scroll") },
    { VKEY_LSHIFT, _T("LShift") },
    { VKEY_RSHIFT, _T("RShift") },
    { VKEY_LCONTROL, _T("LControl") },
    { VKEY_RCONTROL, _T("RControl") },
    { VKEY_LMENU, _T("LMenu") },
    { VKEY_RMENU, _T("RMenu") },
    { VKEY_BROWSER_BACK, _T("BrowserBack") },
    { VKEY_BROWSER_FORWARD, _T("BrowserForward") },
    { VKEY_BROWSER_REFRESH, _T("BrowserRefresh") },
    { VKEY_BROWSER_STOP, _T("BrowserStop") },
    { VKEY_BROWSER_SEARCH, _T("BrowserSearch") },
    { VKEY_BROWSER_FAVORITES, _T("BrowserFavorites") },
    { VKEY_BROWSER_HOME, _T("BrowserHome") },
    { VKEY_VOLUME_MUTE, _T("VolumeMute") },
    { VKEY_VOLUME_DOWN, _T("VolumeDown") },
    { VKEY_VOLUME_UP, _T("VolumeUp") },
    { VKEY_MEDIA_NEXT_TRACK, _T("MediaNextTrack") },
    { VKEY_MEDIA_PREV_TRACK, _T("MediaPrevTrack") },
    { VKEY_MEDIA_STOP, _T("MediaStop") },
    { VKEY_MEDIA_PLAY_PAUSE, _T("MediaPlayPause") },
    { VKEY_MEDIA_LAUNCH_MAIL, _T("MediaLaunchMail") },
    { VKEY_MEDIA_LAUNCH_MEDIA_SELECT, _T("MediaLaunchMediaSelect") },
    { VKEY_MEDIA_LAUNCH_APP1, _T("MediaLaunchApp1") },
    { VKEY_MEDIA_LAUNCH_APP2, _T("MediaLaunchApp2") },
    { VKEY_OEM_1, _T(";") },
    { VKEY_OEM_PLUS, _T("=") },
    { VKEY_OEM_COMMA, _T(",") },
    { VKEY_OEM_MINUS, _T("-") },
    { VKEY_OEM_PERIOD, _T(".") },
    { VKEY_OEM_2, _T("/") },
    { VKEY_OEM_3, _T("`") },
    { VKEY_OEM_4, _T("[") },
    { VKEY_OEM_5, _T("\\") },
    { VKEY_OEM_6, _T("]") },
    { VKEY_OEM_7, _T("'") },
    { VKEY_OEM_8, _T("OEM_8") },
    { VKEY_OEM_102, _T("OEM_102") },
    { VKEY_PROCESSKEY, _T("ProcessKey") },
    { VKEY_PACKET, _T("Packet") },
    { VKEY_ATTN, _T("Attn") },
    { VKEY_CRSEL, _T("Crsel") },
    { VKEY_EXSEL, _T("Exsel") },
    { VKEY_EREOF, _T("Ereof") },
    { VKEY_PLAY, _T("Play") },
    { VKEY_ZOOM, _T("Zoom") },
    { VKEY_NONAME, _T("Noname") },
    { VKEY_PA1, _T("Pa1") },
    { VKEY_OEM_CLEAR, _T("OEM_Clear") },
    { VKEY_UNKNOWN, _T("Unknown") },
};

static const int nNumKeyNames = sizeof(s_pKeyNameMap)/sizeof(KeyNameMap);

BOOL
KeyCodeToKeyName(KeyCode nCode, CString &strKeyName)
{
    const KeyNameMap *pMap = NULL;
    for(int i = 0; i < nNumKeyNames; ++i) {
        if(s_pKeyNameMap[i].m_nKeyCode == nCode) {
            pMap = &(s_pKeyNameMap[i]);
        }
    }

    if(NULL == pMap) {
        return FALSE;
    }

    strKeyName = pMap->m_pKeyName;
    
    return TRUE;
}

BOOL
ModifierToModifierName(UINT nModifier, CString &strModifier)
{
    CString strModifierBuffer;

    if(nModifier & MOD_CONTROL) {
        strModifierBuffer += _T("Ctrl+");
    }

    if(nModifier & MOD_ALT) {
        strModifierBuffer += _T("Alt+");
    }

    if(nModifier & MOD_SHIFT) {
        strModifierBuffer += _T("Shift+");
    }

    if(nModifier & MOD_WIN) {
        strModifierBuffer += _T("Win+");
    }

    if(strModifierBuffer.GetLength() == 0) {
        strModifier = _T("");
    } else {
        strModifier = strModifierBuffer.Left(strModifierBuffer.GetLength() - 1);
    }

    return TRUE;
}