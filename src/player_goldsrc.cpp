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


    m_fFlags = 0;
    m_MoveType = MOVETYPE_WALK;

    m_bHoldingForWater = false;
}

bool CPlayer_GoldSrc::ParseGameData( CSettings_Section* data )
{
    COffsetParser* parser = new COffsetParser();


    m_offFlags = parser->AddOffset( "flags", data->GetOptionValue( "Offset:flags" ) );

    
    m_offmovetype = parser->AddOffset( "movetype", data->GetOptionValue( "Offset:movetype" ) );


    delete parser;

    // movetype is not required.
    return ( m_offFlags != NULL );
}

bool CPlayer_GoldSrc::Update()
{
    return ReadMoveType();
}

bool CPlayer_GoldSrc::IsActive()
{
    return IsAlive();
}

bool CPlayer_GoldSrc::IsAlive()
{
    return ( m_MoveType != MOVETYPE_NONE );
}

bool CPlayer_GoldSrc::ReadFlags()
{
    return ReadProcessMemory(
        g_Process.GetProcess(),
        (void*)( m_offFlags ),
        &m_fFlags,
        sizeof( int ),
        nullptr ) != 0;
}

bool CPlayer_GoldSrc::ReadMoveType()
{
    if ( m_offmovetype == NULL ) return true;


    return ReadProcessMemory(
        g_Process.GetProcess(),
        (void*)( m_offmovetype ),
        &m_MoveType,
        sizeof( int ),
        nullptr ) != 0;
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