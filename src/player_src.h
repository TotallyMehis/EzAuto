#pragma once

#include "player_base.h"
#include "process.h"


class CPlayer_Source : public CPlayer_Base
{
public:
    CPlayer_Source();


    virtual bool Update() override;

    virtual void Jump() override;

    virtual bool IsActive() const override;

    virtual bool ParseGameData( const CSettings_Section* data ) override;
    virtual bool Init() override;

private:
    bool IsAlive() const;

    // Read the location of the local player data.
    bool ReadLocalPlayerPointer();
    bool ReadFlags();
    bool ReadMoveType();

    offset_t m_offLocalPlayer;

    offset_t m_offLocalPlayerPointer;

    offset_t m_offFlags;
    offset_t m_offMoveType;


    int m_fFlags;
    int m_MoveType;

    bool m_bHoldingForWater;
};
