#pragma once

#include "player_base.h"
#include "process.h"


class CPlayer_Source : public CPlayer_Base
{
public:
    CPlayer_Source();


    virtual bool Update();

    virtual void Jump();

    virtual bool IsActive();

    virtual bool ParseGameData( CSettings_Section* data );
    virtual bool Init();

private:
    bool IsAlive();

    bool ReadLocalPlayer();
    bool ReadFlags();
    bool ReadMoveType();

    offset_t m_offLocalPlayer;

    offset_t m_offPlayerBase;

    offset_t m_offFlags;
    offset_t m_offMoveType;


    int m_fFlags;
    int m_MoveType;

    bool m_bHoldingForWater;
};