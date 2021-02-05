#include <iostream>
#include <cstdarg>


#include "sys.h"
#include "process.h"



#define VA_BUFFER_SIZE      256

#define HANDLE_VARIADIC(a)  va_list args; \
                            va_start( args, format ); \
                            vsprintf_s( a, sizeof( a ), format, args ); \
                            va_end( args ); \


CSystem g_System;

CSystem::CSystem()
{
    m_szCmds = nullptr;
    m_nCmdArgs = 0;
}

bool CSystem::IsFocused()
{
    return ( GetForegroundWindow() == g_Process.GetGameWindow() );
}

// Haven't gotten this to work, more testing needed.
bool CSystem::IsProcessRunning()
{
    DWORD code;
    if ( !GetExitCodeProcess( g_Process.GetProcess(), &code ) )
    {
        return false;
    }

    return ( code == STILL_ACTIVE );
    //return ( WaitForSingleObject( g_Process.GetProcess(), 0 ) == WAIT_TIMEOUT );
}

void CSystem::Print( const char* format, ... )
{
    char msg[VA_BUFFER_SIZE];

    HANDLE_VARIADIC( msg )

    std::cout << msg;
}

void CSystem::PrintVerbose( const char* format, ... )
{
#ifndef _DEBUG
    if ( !g_System.HasCmdLine( "-verbose" ) ) return;
#endif

    char msg[VA_BUFFER_SIZE];

    HANDLE_VARIADIC( msg )


    ChangeColor( CCLR_VERBOSE );

    std::cout << msg;

    ChangeColor( CCLR_DEFAULT );
}

void CSystem::PrintDev( const char* format, ... )
{
#ifndef _DEBUG
    if ( !g_System.HasCmdLine( "-dev" ) ) return;
#endif

    char msg[VA_BUFFER_SIZE];

    HANDLE_VARIADIC( msg )


    ChangeColor( CCLR_DEV );

    std::cout << msg;

    ChangeColor( CCLR_DEFAULT );
}

void CSystem::PrintWarning( const char* format, ... )
{
    char msg[VA_BUFFER_SIZE];

    HANDLE_VARIADIC( msg )


    ChangeColor( CCLR_WARNING );

    std::cout << msg;

    ChangeColor( CCLR_DEFAULT );
}

int CSystem::ExitWarning( int errorcode, const char* format, ... )
{
    char msg[VA_BUFFER_SIZE];

    HANDLE_VARIADIC( msg )


    ChangeColor( CCLR_WARNING );

    std::cout << msg;

    ChangeColor( CCLR_DEFAULT );


    std::cin.get();

    return errorcode;
}

void CSystem::ChangeColor( unsigned short clr )
{
    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), clr );
}

void CSystem::SetTitle( const char* title )
{
    SetConsoleTitle( title );
}

void CSystem::SetCmdLine( char* cmds[], int cmdnum )
{
    m_szCmds = cmds;

    m_nCmdArgs = cmdnum;
}

bool CSystem::HasCmdLine( const char* name ) const
{
    return ( GetCmdPos( name ) != -1 );
}

int CSystem::GetCmdPos( const char* name ) const
{
    for ( int i = 0; i < m_nCmdArgs; i++ )
    {
        if ( strcmp( m_szCmds[i], name ) == 0 )
        {
            return i;
        }
    }

    return -1;
}

bool CSystem::GetCmdValue( const char* name, char* value, size_t len ) const
{
    if ( !value || !len ) return false;


    int pos = GetCmdPos( name );

    if ( pos == -1 ) return false;

    if ( ++pos >= m_nCmdArgs ) return false;


    strcpy_s( value, len, m_szCmds[pos] );

    return true;
}
