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

    HANDLE GetProcess() const { return m_hProcess; };
    DWORD GetProcessId() const { return m_dwProcessID; };

    HWND GetGameWindow() const { return m_hWndGame; };

    Engine_t GetEngine() const { return m_Engine; };


    DWORD FindModulePointerByName( const char* name ) const;

    static const CSettings_Section* ListenToProcesses();

    bool ReadMemory( void* pSrc, void* pDest, size_t size );
    bool WriteMemory( void* pDest, void* pSrc, size_t size );

private:
    bool OpenProcess( const char* name, bool bWrite = false );
    bool OpenProcess( bool bWrite = false );

    bool SetBaseByModuleName( const char* name );


    bool FindProcessByWindowTitle( const char* name );

    bool FindProcessByName( const char* name, PROCESSENTRY32* process ) const;

    HANDLE m_hProcess;
    DWORD m_dwProcessID;

    HWND m_hWndGame;

    Engine_t m_Engine;
};

extern CProcess g_Process;
