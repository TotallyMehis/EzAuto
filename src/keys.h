#pragma once


typedef int key_t;

typedef unsigned char inputtype_t;

#define INVALID_KEY     0


class CKey
{
public:
    CKey();


    bool IsValidVirtual() { return ( m_Virtual != INVALID_KEY ); }
    bool IsValidScancode() { return ( m_Scancode != INVALID_KEY ); }

    void Reset();

    void SetKey( key_t virtualkey, inputtype_t inputtype, bool bAllowBadScancode = true );

    key_t GetVirtual() { return m_Virtual; }
    key_t GetScancode() { return m_Scancode; }
    inputtype_t GetInputType() { return m_InputType; }

private:
    key_t m_Virtual;

    key_t m_Scancode;

    inputtype_t m_InputType;
};


class CKeyboard
{
public:
    CKeyboard();


    static bool GetKey( const char* keyname, CKey* key, bool bAllowBadScancode = true );


    CKey GetJumpKey() { return m_keyJump; };
    bool SetJumpKey( const char* name );

    CKey GetHoldKey() { return m_keyHold; };
    bool SetHoldKey( const char* name );


    CKey GetPauseKey() { return m_keyPause; };
    bool SetPauseKey( const char* name );

    bool PressedPause();


    void SendJump();
    void SendJumpHold();
    void SendJumpUp();

    bool IsHoldingJump();

private:
    CKey m_keyJump;
    CKey m_keyHold;
    CKey m_keyPause;
};

extern CKeyboard g_Keys;