#include "offsetparser.h"
#include "offsetparser_int.h"
#include "process.h"


COffset::COffset( COffsetParser* owner, const char* name, const char* offset )
{
    m_Owner = owner;
    m_off = NULL;
    m_pszName = nullptr;

    SetName( name );


    Parse( offset );
}

COffset::COffset( COffsetParser* owner, const char* name, offset_t offset )
{
    m_Owner = owner;
    m_off = offset;
    m_pszName = nullptr;

    SetName( name );
}

void COffset::SetName( const char* name )
{
    delete[] m_pszName;


    if ( name && *name )
    {
        const size_t len = strlen( name ) + 1;

        m_pszName = new char[len];

        strcpy_s( m_pszName, len, name );
    }
    else
    {
        m_pszName = nullptr;
    }
}

COffset::~COffset()
{
    delete[] m_pszName;
}

void COffset::Parse( const char* offset )
{
    char temp[92];

    strcpy_s( temp, sizeof( temp ), offset );



    char* c = temp;

    while ( c )
    {
        char* n = strchr( c, '+' );

        if ( n )
        {
            *n = 0;
        }


        offset_t off;
        if ( m_Owner->FindOffset( c, &off ) != -1 )
        {
            m_off += off;
        }
        else if ( strstr( c, "0x" ) == c ) // It's just hex.
        {
            m_off += strtol( c, nullptr, 16 );
        }
        else
        {
            // It's a dll?
            if ( strstr( c, ".dll" ) )
            {
                off = (offset_t)g_Process.FindModulePointerByName( c );

                // Add the dll into a list of offsets.
                if ( off != NULL )
                {
                    m_off += m_Owner->AddOffset( c, off );
                }
            }
        }


        if ( n )
        {
            c = n + 1;
        }
        else
        {
            break;
        }
    }
}
