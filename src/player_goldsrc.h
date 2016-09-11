#pragma once

#include "player_base.h"

#include "process.h"


class CPlayer_GoldSrc : public CPlayer_Base
{
public:
    CPlayer_GoldSrc();


    virtual bool Update();

    virtual void Jump();

    virtual bool IsActive();

    virtual bool ParseGameData( CSettings_Section* data );

private:
    bool IsAlive();

    bool ReadFlags();
    bool ReadMoveType();

    offset_t m_offFlags;
    offset_t m_offmovetype;

    int m_fFlags;
    int m_MoveType;

    bool m_bHoldingForWater;
};