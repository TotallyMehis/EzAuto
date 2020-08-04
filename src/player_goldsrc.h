#pragma once

#include "player_base.h"

#include "process.h"


class CPlayer_GoldSrc : public CPlayer_Base
{
public:
    CPlayer_GoldSrc();


    virtual bool Update() override;

    virtual void Jump() override;

    virtual bool IsActive() const override;

    virtual bool ParseGameData( const CSettings_Section* data ) override;
    virtual bool Init() override;

private:
    bool IsAlive() const;

    bool ReadFlags();
    bool ReadMoveType();

    offset_t m_offFlags;
    offset_t m_offmovetype;
    offset_t m_offBUNNYJUMP_MAX_SPEED_FACTOR;

    int m_fFlags;
    int m_MoveType;
    float m_flBunnyHopSpeedFactor;

    bool m_bHoldingForWater;
};
