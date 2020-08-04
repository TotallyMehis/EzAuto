#pragma once

#include "player_base.h"


#define GAMEDATA_FILE       "games.ini"
#define SETTINGS_FILE       "settings.ini"

namespace Core
{
    int ListenToProcess();

    void ReadSettings();

    void JumpLoop( CPlayer_Base* );
}
