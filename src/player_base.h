#pragma once

#include "settings.h"


typedef unsigned long offset_t;


class CPlayer_Base
{
public:
    virtual bool Update() { return true; }

    virtual bool IsActive() { return true; }

    virtual void Jump() {}

    virtual bool ParseGameData( CSettings_Section* data ) { return false; }

    virtual bool Init() { return false; }


    bool GetSleep() { return m_bSleep; }
    void SetSleep( bool value ) { m_bSleep = value; }

protected:
    bool m_bSleep = false;
};