#pragma once

// Everything to do with printing and cmd line. Mostly static methods.


#define CCLR_ALL            FOREGROUND_INTENSITY

#define CCLR_BLUE           FOREGROUND_BLUE | CCLR_ALL
#define CCLR_GREEN          FOREGROUND_GREEN | CCLR_ALL
#define CCLR_CYAN           FOREGROUND_BLUE | FOREGROUND_GREEN | CCLR_ALL
#define CCLR_RED            FOREGROUND_RED | CCLR_ALL
#define CCLR_MAGENTA        FOREGROUND_RED | FOREGROUND_BLUE | CCLR_ALL
#define CCLR_WHITE          FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE

#define CCLR_DEFAULT        CCLR_WHITE
#define CCLR_VERBOSE        CCLR_MAGENTA
#define CCLR_DEV            CCLR_BLUE
#define CCLR_WARNING        CCLR_RED

class CSystem
{
public:
    CSystem();


    static bool IsFocused();

    static bool IsProcessRunning();

    static void Print( const char* format, ... );
    static void PrintVerbose( const char* format, ... );
    static void PrintDev( const char* format, ... );
    static void PrintWarning( const char* format, ... );

    static int ExitWarning( int errorcode, const char* format, ... );

    static  void ChangeColor( unsigned short clr );

    static void SetTitle( const char* title );


    void SetCmdLine( char* cmds[], int cmdnum );

    bool HasCmdLine( const char* name );

    bool GetCmdValue( const char* name, char* value, size_t len );

private:
    int GetCmdPos( const char* name );


    char** m_szCmds;

    int m_nCmdArgs;
};

extern CSystem g_System;