#include "player_src.h"
#include "keys.h"
#include "offsetparser.h"


#define FL_ONGROUND             ( 1 << 0 )
// According to SM API, it should be 1 << 9...
#define	FL_INWATER              ( 1 << 10 )


#define MOVETYPE_WALK           2
#define MOVETYPE_LADDER         9
#define MOVETYPE_OBSERVER       10


CPlayer_Source::CPlayer_Source()
{
    m_offPlayerBase = NULL;
    m_offLocalPlayer = NULL;

    m_offFlags = NULL;
    m_offMoveType = NULL;


    m_fFlags = 0;
    m_MoveType = MOVETYPE_WALK;

    m_bHoldingForWater = false;
}

bool CPlayer_Source::ParseGameData( CSettings_Section* data )
{
    COffsetParser* parser = new COffsetParser();


    m_offPlayerBase = parser->AddOffset( "PlayerBase", data->GetOptionValue( "Offset:PlayerBase" ) );


    m_offFlags = parser->AddOffset( "m_fFlags", data->GetOptionValue( "Offset:m_fFlags" ) );


    m_offMoveType = parser->AddOffset( "m_MoveType", data->GetOptionValue( "Offset:m_MoveType" ) );


    delete parser;

    // m_lifeState is not required.
    return ( m_offPlayerBase != NULL && m_offFlags != NULL );
}

bool CPlayer_Source::Update()
{
    return ( ReadLocalPlayer() && ReadMoveType() );
}

bool CPlayer_Source::IsActive()
{
    return IsAlive();
}

bool CPlayer_Source::IsAlive()
{
    return ( m_MoveType == MOVETYPE_WALK || m_MoveType == MOVETYPE_LADDER );
}

bool CPlayer_Source::ReadLocalPlayer()
{
    return ReadProcessMemory(
        g_Process.GetProcess(),
        (void*)( m_offPlayerBase ),
        &m_offLocalPlayer,
        sizeof( offset_t ),
        nullptr ) != 0;
}

bool CPlayer_Source::ReadMoveType()
{
    // Just return true if we have no m_MoveType offset.
    if ( m_offMoveType == NULL ) return true;


    return ReadProcessMemory(
        g_Process.GetProcess(),
        (void*)( m_offLocalPlayer + m_offMoveType ),
        &m_MoveType,
        sizeof( int ),
        nullptr ) != 0;
}

bool CPlayer_Source::ReadFlags()
{
    return ReadProcessMemory(
        g_Process.GetProcess(),
        (void*)( m_offLocalPlayer + m_offFlags ),
        &m_fFlags,
        sizeof( int ),
        nullptr ) != 0;
}

void CPlayer_Source::Jump()
{
    if ( g_Keys.IsHoldingJump() )
    {
        ReadFlags();

        if ( m_fFlags & FL_INWATER )
        {
            // We weren't in water last frame...
            if ( !m_bHoldingForWater )
            {
                g_Keys.SendJumpUp();

                g_Keys.SendJumpHold();

                m_bHoldingForWater = true;

                SetSleep( true );
            }
        }
        else
        {
            // Not in water anymore, reset jump.
            if ( m_bHoldingForWater )
            {
                g_Keys.SendJumpUp();

                m_bHoldingForWater = false;

                SetSleep( true );
            }

            if ( m_fFlags & FL_ONGROUND || m_MoveType == MOVETYPE_LADDER )
            {
                g_Keys.SendJump();

                SetSleep( true );
            }
        }
    }
    // We held 
    else if ( m_bHoldingForWater )
    {
        g_Keys.SendJumpUp();

        m_bHoldingForWater = false;

        SetSleep( true );
    }
}
