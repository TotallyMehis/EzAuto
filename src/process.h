#pragma once

#include <Windows.h>
#include <TlHelp32.h>


#include "sys.h"
#include "engine.h"
#include "settings.h"


class CProcess
{
public:
    CProcess();

    bool OpenProcess( const char* name, bool bWrite = false );
    bool OpenProcess( bool bWrite = false );

    bool SetBaseByModuleName( const char* name );


    HANDLE GetProcess() { return m_hProcess; };
    DWORD GetProcessId() { return m_dwProcessID; };

    HWND GetGameWindow() { return m_hWndGame; };

    Engine_t GetEngine() { return m_Engine; };


    static CSettings_Section* ListenToProcesses();



    bool FindProcessByWindowTitle( const char* name );

    DWORD FindModulePointerByName( const char* name );

private:
    bool FindProcessByName( const char* name, PROCESSENTRY32* process );

    HANDLE m_hProcess;
    DWORD m_dwProcessID;

    HWND m_hWndGame;

    Engine_t m_Engine;
};

extern CProcess g_Process;