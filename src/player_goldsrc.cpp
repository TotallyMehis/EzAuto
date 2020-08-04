#include "player_goldsrc.h"
#include "process.h"
#include "keys.h"
#include "offsetparser.h"


// Please note that these are different from Source's.
#define FL_INWATER          ( 1 << 4 )
#define FL_ONGROUND         ( 1 << 9 )


#define MOVETYPE_NONE       0
#define MOVETYPE_WALK       3
#define MOVETYPE_LADDER     5


CPlayer_GoldSrc::CPlayer_GoldSrc()
{
    m_offFlags = NULL;
    m_offmovetype = NULL;
    m_offBUNNYJUMP_MAX_SPEED_FACTOR = NULL;

    m_fFlags = 0;
    m_MoveType = MOVETYPE_WALK;
    m_flBunnyHopSpeedFactor = -1.0f;

    m_bHoldingForWater = false;
}

bool CPlayer_GoldSrc::ParseGameData( CSettings_Section* data )
{
    COffsetParser* parser = new COffsetParser();


    m_offFlags = parser->AddOffset( "flags", data->GetOptionValue( "Offset:flags" ) );

    
    m_offmovetype = parser->AddOffset( "movetype", data->GetOptionValue( "Offset:movetype" ) );


    m_offBUNNYJUMP_MAX_SPEED_FACTOR = parser->AddOffset( "BUNNYJUMP_MAX_SPEED_FACTOR", data->GetOptionValue( "Offset:BUNNYJUMP_MAX_SPEED_FACTOR" ) );

    delete parser;


    auto* pszSpeedFactor = data->GetOptionValue( "SetMaxSpeedFactor" );
    if ( pszSpeedFactor )
    {
        m_flBunnyHopSpeedFactor = (float)atof( pszSpeedFactor );
    }



    // movetype is not required.
    return ( m_offFlags != NULL );
}

bool CPlayer_GoldSrc::Init()
{
    if ( m_flBunnyHopSpeedFactor >= 0.0f )
    {
        bool res = g_Process.WriteMemory( (void*)( m_offBUNNYJUMP_MAX_SPEED_FACTOR ), (void*)( &m_flBunnyHopSpeedFactor ), sizeof( float ) );

        if ( !res )
        {
            CSystem::PrintWarning( "Failed to override BUNNYJUMP_MAX_SPEED_FACTOR! Error: %i", GetLastError() );
        }
    }


    return true;
}

bool CPlayer_GoldSrc::Update()
{
    return ReadMoveType();
}

bool CPlayer_GoldSrc::IsActive() const
{
    return IsAlive();
}

bool CPlayer_GoldSrc::IsAlive() const
{
    return ( m_MoveType != MOVETYPE_NONE );
}

bool CPlayer_GoldSrc::ReadFlags()
{
    return g_Process.ReadMemory( (void*)( m_offFlags ), &m_fFlags, sizeof( int ) );
}

bool CPlayer_GoldSrc::ReadMoveType()
{
    if ( m_offmovetype == NULL ) return true;

    return g_Process.ReadMemory( (void*)( m_offmovetype ), &m_MoveType, sizeof( int ) );
}

void CPlayer_GoldSrc::Jump()
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
    // We held for water, now release it.
    else if ( m_bHoldingForWater )
    {
        g_Keys.SendJumpUp();

        m_bHoldingForWater = false;

        SetSleep( true );
    }
}
