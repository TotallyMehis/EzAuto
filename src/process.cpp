#include "settings.h"
#include "process.h"
#include "engine.h"
#include "core.h"


CProcess g_Process;


CProcess::CProcess()
{
    m_hProcess = INVALID_HANDLE_VALUE;
    m_dwProcessID = 0;
    m_hWndGame = NULL;

    m_Engine = ENGINE_INVALID;
}

bool CProcess::FindProcessByName( const char* name, PROCESSENTRY32* process ) const
{
    // Create snapshot that holds all processes.
    HANDLE hSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if ( hSnap == INVALID_HANDLE_VALUE ) return false;


    PROCESSENTRY32 entry = { 0 };
    // Required.
    entry.dwSize = sizeof( PROCESSENTRY32 );

    if ( !Process32First( hSnap, &entry ) )
    {
        CloseHandle( hSnap );
        return false;
    }

    
    do
    {
        if ( _stricmp( entry.szExeFile, name ) == 0 )
        {
            // Found our match!

            // Copy result...
            memcpy( (void*)process, (void*)&entry, sizeof( PROCESSENTRY32 ) );


            CloseHandle( hSnap );

            return true;
        }
    }
    while ( Process32Next( hSnap, &entry ) );


    CloseHandle( hSnap );

    return false;
}

DWORD CProcess::FindModulePointerByName( const char* name ) const
{
    // Create snapshot that holds all process' modules.
    HANDLE hSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, m_dwProcessID );
    if ( hSnap == INVALID_HANDLE_VALUE ) return NULL;


    MODULEENTRY32 entry = { 0 };
    // Required.
    entry.dwSize = sizeof( MODULEENTRY32 );


    if ( !Module32First( hSnap, &entry ) )
    {
        CloseHandle( hSnap );
        return NULL;
    }


    do
    {
        if ( _stricmp( entry.szModule, name ) == 0 )
        {
            // Found our match!
            CloseHandle( hSnap );


            CSystem::PrintVerbose( "Found %s @ 0x%x\n", name, entry.modBaseAddr );


            return (DWORD)entry.modBaseAddr;
        }
    }
    while ( Module32Next( hSnap, &entry ) );


    CloseHandle( hSnap );

    return NULL;
}

bool CProcess::OpenProcess( const char* name, bool bWrite )
{
    PROCESSENTRY32 entry;

    if ( !FindProcessByName( name, &entry ) )
    {
        return false;
    }


    m_dwProcessID = entry.th32ProcessID;

    return OpenProcess( bWrite );
}

bool CProcess::OpenProcess( bool bWrite )
{
    DWORD flags = PROCESS_VM_READ | PROCESS_QUERY_INFORMATION;

    if ( bWrite )
    {
        flags |= PROCESS_VM_WRITE | PROCESS_VM_OPERATION;

        CSystem::PrintWarning( "Opening process for writing!\n" );
    }
    
    m_hProcess = ::OpenProcess( flags, false, m_dwProcessID );


    return m_hProcess != NULL;
}

bool CProcess::ReadMemory( void* pSrc, void* pDest, size_t out_size )
{
    return ReadProcessMemory(
        GetProcess(),
        pSrc,
        pDest,
        out_size,
        nullptr ) != 0;
}

bool CProcess::WriteMemory( void* pDest, void* pSrc, size_t size )
{
    DWORD oldprotect;
    VirtualProtectEx( GetProcess(), pDest, size, PAGE_EXECUTE_READWRITE, &oldprotect );

    auto res = WriteProcessMemory(
        GetProcess(),
        pDest,
        pSrc,
        size,
        nullptr );

    if ( res == 0 )
    {
        CSystem::PrintDev( "Couldn't write to process memory! Error: %i\n", GetLastError() );
    }

    VirtualProtectEx( GetProcess(), pDest, size, oldprotect, &oldprotect );

    return res != 0;
}

/*#define DEFAULT_LOOKUP_GAME_GOLDSRC     0x100404A


void CProcess::LookupGame( DWORD offset )
{
    if ( m_Engine == ENGINE_GOLDSRC )
    {
        offset += m_dwBase;
    }

    ReadProcessMemory(
        m_hProcess,
        (PBYTE*)( m_dwBase + offset ),
        &m_szGame,
        sizeof( m_szGame ),
        NULL );

    m_szGame[sizeof( m_szGame ) - 1] = 0;
}*/

bool CProcess::FindProcessByWindowTitle( const char* name )
{
    m_hWndGame = FindWindow( nullptr, name );

    if ( m_hWndGame != NULL )
    {
        GetWindowThreadProcessId( m_hWndGame, &m_dwProcessID );

        return ( m_dwProcessID != NULL );
    }

    return false;
}

const CSettings_Section* CProcess::ListenToProcesses()
{
    CSystem::ChangeColor( CCLR_CYAN );
    CSystem::Print( "Listening to games...\n" );
    CSystem::ChangeColor( CCLR_DEFAULT );


    // Loop through our game data...
    size_t i = 0;
    size_t maxlen = g_pGameData->data.size();



    if ( !maxlen ) return nullptr;


    while ( true )
    {
        const char* title = g_pGameData->data[i]->GetOptionValue( "WindowTitle" );

        if ( title && g_Process.FindProcessByWindowTitle( title ) )
        {
            // We've found a window, let's see if our settings were setup correctly.
            const char* engine = g_pGameData->data[i]->GetOptionValue( "Engine" );


            Engine_t eng = ENGINE_INVALID;

            if ( engine && (eng = Engine::EngineNameToType( engine )) != ENGINE_INVALID )
            {
                // Open for write if on GoldSrc and wants max speed factor changed.
                if ( g_Process.OpenProcess( eng == ENGINE_GOLDSRC && g_Core.CS16_MaxSpeedFactor() >= 0.0f ) )
                {
                    g_Process.m_Engine = eng;

                    return g_pGameData->data[i];
                }
            }
            else
            {
                CSystem::PrintWarning( "Game '%s' has invalid engine called '%s'!", g_pGameData->data[i]->GetSectionName(), engine );

                g_pGameData->RemoveSection( i );
                maxlen = g_pGameData->data.size();

                if ( !maxlen ) return nullptr;
            }
        }

        // Go back to start and sleep a bit.
        if ( ++i >= maxlen ) i = 0;

        Sleep( 50 );
    }

    return nullptr;
}
