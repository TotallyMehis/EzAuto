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
    m_offLocalPlayerPointer = NULL;
    m_offLocalPlayer = NULL;

    m_offFlags = NULL;
    m_offMoveType = NULL;


    m_fFlags = 0;
    m_MoveType = MOVETYPE_WALK;

    m_bHoldingForWater = false;
}

bool CPlayer_Source::ParseGameData( const CSettings_Section* data )
{
    COffsetParser* parser = new COffsetParser();

    auto* pszLocalPlayerPointer = data->GetOptionValue( "Offset:LocalPlayerPointer" );
    // Fallback to old value
    if ( !pszLocalPlayerPointer )
        pszLocalPlayerPointer = data->GetOptionValue( "Offset:PlayerBase" );
    
    m_offLocalPlayerPointer = parser->AddOffset( "LocalPlayerPointer", pszLocalPlayerPointer );


    m_offFlags = parser->AddOffset( "m_fFlags", data->GetOptionValue( "Offset:m_fFlags" ) );


    m_offMoveType = parser->AddOffset( "m_MoveType", data->GetOptionValue( "Offset:m_MoveType" ) );


    delete parser;

    // m_lifeState is not required.
    return ( m_offLocalPlayerPointer != NULL && m_offFlags != NULL );
}

bool CPlayer_Source::Init()
{
    return true;
}

bool CPlayer_Source::Update()
{
    return ( ReadLocalPlayerPointer() && ReadMoveType() );
}

bool CPlayer_Source::IsActive() const
{
    return IsAlive();
}

bool CPlayer_Source::IsAlive() const
{
    return ( m_MoveType == MOVETYPE_WALK || m_MoveType == MOVETYPE_LADDER );
}

bool CPlayer_Source::ReadLocalPlayerPointer()
{
    return g_Process.ReadMemory( (void*)( m_offLocalPlayerPointer ), &m_offLocalPlayer, sizeof( m_offLocalPlayer ) );
}

bool CPlayer_Source::ReadMoveType()
{
    // Just return true if we have no m_MoveType offset.
    if ( m_offMoveType == NULL ) return true;

    return g_Process.ReadMemory( (void*)( m_offLocalPlayer + m_offMoveType ), &m_MoveType, sizeof( int ) );
}

bool CPlayer_Source::ReadFlags()
{
    return g_Process.ReadMemory( (void*)( m_offLocalPlayer + m_offFlags ), &m_fFlags, sizeof( int ) );
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
