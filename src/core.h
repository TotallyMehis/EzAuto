#pragma once

#include "player_base.h"


#define GAMEDATA_FILE       "games.ini"
#define SETTINGS_FILE       "settings.ini"

class CCore
{
public:
    CCore();
    virtual ~CCore() {}

    void Init();
    int ListenToProcess();

    float CS16_MaxSpeedFactor() const { return m_flBunnyHopSpeedFactor; }

private:
    void ReadSettings();

    void JumpLoop( CPlayer_Base* pPlayer );


    float m_flBunnyHopSpeedFactor;
};

extern CCore g_Core;

