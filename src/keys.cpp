#include "keys.h"

#include <Windows.h>

#define NDEBUG 1
#include <assert.h>


#include "sys.h"
#include "keylist.h"


CKeyboard g_Keys;


#define KEYDOWN         0x8000


CKey::CKey()
{
    Reset();
}

void CKey::Reset()
{
    m_Virtual = INVALID_KEY;
    m_Scancode = INVALID_KEY;
    m_InputType = 0;
}

void CKey::SetKey( key_t virtualkey, inputtype_t inputtype, bool bAllowBadScancode )
{
    m_Virtual = virtualkey;


    // We have to get the scancode so the game is able to recognize the key.
    // However, this isn't required for keys that aren't pressed by the program.
    m_Scancode = MapVirtualKey( virtualkey, MAPVK_VK_TO_VSC );

    m_InputType = inputtype;


    CSystem::PrintDev( "Virtual key: 0x%x | Scancode: 0x%x | Input type: %i\n", m_Virtual, m_Scancode, m_InputType );


    if ( m_Scancode == 0 && !bAllowBadScancode )
    {
        CSystem::PrintWarning( "Couldn't find scancode for virtual key 0x%x! Please choose another button.\n", m_Virtual );

        Reset();
    }
}



CKeyboard::CKeyboard()
{
    const int virtlen = sizeof g_KeysVirtual / sizeof( g_KeysVirtual[0] );
    const int inputlen = sizeof g_KeyTypes / sizeof( g_KeyTypes[0] );
    const int namelen = sizeof g_KeysName / sizeof( g_KeysName[0] );

    assert( virtlen == inputlen && virtlen == namelen );
}

bool CKeyboard::GetKey( const char* keyname, CKey* key, bool bAllowBadScancode )
{
    // See keylist.h for the list of keys.
    const int len = sizeof g_KeysVirtual / sizeof( g_KeysVirtual[0] );

    for ( int i = 0; i < len; i++ )
    {
        if ( _stricmp( keyname, g_KeysName[i] ) == 0 )
        {
            key->SetKey( g_KeysVirtual[i], g_KeyTypes[i], bAllowBadScancode );

            return true;
        }
    }


    CSystem::PrintWarning( "Cannot find key '%s'!\n", keyname );


    key->Reset();

    return false;
}


bool CKeyboard::SetJumpKey( const char* name )
{
    if ( !GetKey( name, &m_keyJump, false ) )
    {
        return false;
    }

    CSystem::Print( "Your jump key is %s! Bind this key to +jump. (bind %s +jump)\n", name, name );

    return true;
}

bool CKeyboard::SetHoldKey( const char* name )
{
    if ( !GetKey( name, &m_keyHold ) )
    {
        return false;
    }

    CSystem::Print( "Your hold key is %s! Unbind this key. (unbind %s)\n", name, name );

    return true;
}

bool CKeyboard::SetPauseKey( const char* name )
{
    if ( !GetKey( name, &m_keyPause ) )
    {
        return false;
    }

    CSystem::Print( "Your pause key is %s!\n", name );

    return true;
}

void CKeyboard::SendJump()
{
    SendJumpHold();

    SendJumpUp();
}

void CKeyboard::SendJumpHold()
{
    if ( !m_keyJump.IsValidScancode() ) return;


    INPUT input;
	
    input.type = m_keyJump.GetInputType();
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.wVk = 0;
    input.ki.wScan = m_keyJump.GetScancode();
    

    input.ki.dwFlags = KEYEVENTF_SCANCODE;
    SendInput( 1, &input, sizeof( INPUT ) );
}

void CKeyboard::SendJumpUp()
{
    if ( !m_keyJump.IsValidScancode() ) return;


    INPUT input;
	
    input.type = m_keyJump.GetInputType();
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.wVk = 0;
    input.ki.wScan = m_keyJump.GetScancode();


    input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
    SendInput( 1, &input, sizeof( INPUT ) );
}

bool CKeyboard::IsHoldingJump()
{
    if ( !m_keyHold.IsValidVirtual() ) return false;


    return ( GetKeyState( m_keyHold.GetVirtual() ) & KEYDOWN ) ? true : false;
}

bool CKeyboard::PressedPause()
{
    if ( !m_keyPause.IsValidVirtual() ) return false;


    return GetAsyncKeyState( m_keyPause.GetVirtual() ) != 0;
}