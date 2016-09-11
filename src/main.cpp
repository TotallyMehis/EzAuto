#include "sys.h"
#include "settings.h"
#include "core.h"

#include <iostream>



int main( int argc, char* argv[] )
{
    g_System.SetCmdLine( argv, argc );


    CSystem::SetTitle( "EzAuto" );


    g_pGameData = CSettings::OpenFile( GAMEDATA_FILE );


    if ( !g_pGameData || !g_pGameData->data.size() )
    {
        return CSystem::ExitWarning( 1, "Couldn't read game data from file '%s'! Exiting...\n", GAMEDATA_FILE );
    }


    Core::ReadSettings();


    return Core::ListenToProcess();
}