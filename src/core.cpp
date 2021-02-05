#include "keys.h"
#include "core.h"
#include "sys.h"
#include "process.h"

#include "player_goldsrc.h"
#include "player_src.h"


CCore g_Core;


CCore::CCore()
{
    m_flBunnyHopSpeedFactor = -1.0f;
}

void CCore::Init()
{
    ReadSettings();
}

void CCore::ReadSettings()
{
    char temp[32];

    // Read commandline settings...
    if ( g_System.GetCmdValue( "-pausekey", temp, sizeof( temp ) ) ) g_Keys.SetPauseKey( temp );
    if ( g_System.GetCmdValue( "-holdkey", temp, sizeof( temp ) ) ) g_Keys.SetHoldKey( temp );
    if ( g_System.GetCmdValue( "-jumpkey", temp, sizeof( temp ) ) ) g_Keys.SetJumpKey( temp );


    // Read our file.
    CSettings* settings = CSettings::OpenFile( SETTINGS_FILE );

    if ( settings && !g_Keys.GetPauseKey().IsValidVirtual() && settings->FindOption( "PauseKey", temp, sizeof( temp ) ) )
    {
        g_Keys.SetPauseKey( temp );
    }
    else
    {
        g_Keys.SetPauseKey( "f11" );
    }

    if ( settings && !g_Keys.GetHoldKey().IsValidVirtual() && settings->FindOption( "HoldKey", temp, sizeof( temp ) ) )
    {
        g_Keys.SetHoldKey( temp );
    }
    else
    {
        g_Keys.SetHoldKey( "space" );
    }

    if ( settings && !g_Keys.GetJumpKey().IsValidScancode() && settings->FindOption( "JumpKey", temp, sizeof( temp ) ) )
    {
        g_Keys.SetJumpKey( temp );
    }
    else
    {
        g_Keys.SetJumpKey( "v" );
    }

    if ( settings && settings->FindOption( "SetMaxSpeedFactor", temp, sizeof( temp ) ) )
    {
        m_flBunnyHopSpeedFactor = (float)atof( temp );
    }

    delete settings;
}

int CCore::ListenToProcess()
{
    // Get the game section from games.ini
    const CSettings_Section* data = CProcess::ListenToProcesses();

    if ( !data )
    {
        return CSystem::ExitWarning( 1, "Something went wrong when listening to game processes! Exiting...\n" );
    }


    char name[64];
    if ( !data->GetSectionName( name, sizeof( name ) ) || !strlen( name ) )
    {
        strcpy_s( name, sizeof( name ), "N/A" );
    }


    CSystem::ChangeColor( CCLR_GREEN );
    CSystem::Print( "Found game %s!\n", name );
    CSystem::ChangeColor( CCLR_DEFAULT );


    CPlayer_Base* pBase = nullptr;



    switch ( g_Process.GetEngine() )
    {
        case ENGINE_SOURCE :
        {
            pBase = new CPlayer_Source();
            break;
        }
        case ENGINE_GOLDSRC :
        {
            pBase = new CPlayer_GoldSrc();
            break;
        }
    }

    if ( !pBase )
    {
        return CSystem::ExitWarning( 1, "Unknown engine was found. Exiting...\n" );;
    }

    if ( !pBase->ParseGameData( data ) )
    {
        delete pBase;

        return CSystem::ExitWarning( 1, "Couldn't parse game data! Exiting...\n" );
    }

    if ( !pBase->Init() )
    {
        delete pBase;

        return CSystem::ExitWarning( 1, "Failed to init player data! Exiting...\n" );
    }


    CCore::JumpLoop( pBase );


    return 0;
}

void CCore::JumpLoop( CPlayer_Base* pPlayer )
{
    if ( !pPlayer ) return;


    bool bPaused = false;


    while ( true )
    {
        if ( !CSystem::IsProcessRunning() )
        {
            break;
        }

        if ( g_Keys.PressedPause() )
        {
            bPaused = !bPaused;

            CSystem::ChangeColor( CCLR_MAGENTA );
            CSystem::Print( "Bunnyhopping is now %s!\n", bPaused ? "paused" : "continued" );
            CSystem::ChangeColor( CCLR_DEFAULT );

            // Always sleep after toggling pause.
            Sleep( 100 );
        }


        if ( bPaused || !CSystem::IsFocused() )
        {
            Sleep( 100 );
            continue;
        }


        if ( !pPlayer->Update() )
        {
            continue;
        }

        if ( !pPlayer->IsActive() )
        {
            continue;
        }


        pPlayer->Jump();


        // We have to sleep when sending an input.
        // Otherwise our game would freeze from trying to read messages.
        if ( pPlayer->GetSleep() )
        {
            pPlayer->SetSleep( false );

            Sleep( 1 );
        }
    }
}
