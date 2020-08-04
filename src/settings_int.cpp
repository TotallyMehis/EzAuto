#include "sys.h"
#include "settings_int.h"


CSettings_Section::CSettings_Section( const char* name )
{
    m_pszSectionName = nullptr;


    SetSectionName( name );


    CSystem::PrintDev( "Adding section '%s'\n", ( !name || !*name ) ? "N/A" : name );
}

CSettings_Section::~CSettings_Section()
{
    delete[] m_pszSectionName;

    for ( size_t i = 0; i < m_Options.size(); i++ )
    {
        delete m_Options[i];
    }
}

void CSettings_Section::InsertOption( const char* name, const char* value )
{
    // Both must be valid...
    if ( !name || !value ) return;

    if ( !*name || !*value ) return;


    // Check if that option already exists.
    if ( GetOptionIndex( name ) != -1 )
    {
        CSystem::PrintWarning( "Option '%s' already exists! (value: %s)\n", name, value );

        return;
    }


    CSettings_Section_Option* option = new CSettings_Section_Option( name, value );

    m_Options.push_back( option );
}

bool CSettings_Section::GetOptionValue( const char* name, char* value, size_t len )
{
    int index = GetOptionIndex( name );
    if ( index != -1 )
    {
        if ( value && len )
        {
            strcpy_s( value, len, m_Options[index]->GetValue() );
        }
        
        return true;
    }

    if ( value && len )
    {
        *value = 0;
    }

    return false;
}

const char* CSettings_Section::GetOptionValue( const char* name )
{
    int index = GetOptionIndex( name );
    if ( index != -1 )
    {
        return m_Options[index]->GetValue();
    }

    return nullptr;
}

int CSettings_Section::GetOptionIndex( const char* name )
{
    for ( size_t i = 0; i < m_Options.size(); i++ )
    {
        if ( _stricmp( name, m_Options[i]->GetName() ) == 0 )
        {
            return i;
        }
    }


    return -1;
}

bool CSettings_Section::GetSectionName( char* name, size_t len )
{
    if ( name && len )
    {
        if ( m_pszSectionName )
        {
            strcpy_s( name, len, m_pszSectionName );

            return true;
        }

        *name = 0;
    }

    return false;
}

void CSettings_Section::SetSectionName( const char* name )
{
    delete[] m_pszSectionName;


    if ( name && *name )
    {
        const size_t len = strlen( name ) + 1;

        m_pszSectionName = new char[len];
        strcpy_s( m_pszSectionName, len, name );
    }
    else
    {
        m_pszSectionName = nullptr;
    }
}

CSettings_Section_Option::CSettings_Section_Option( const char* name, const char* value )
{
    const size_t namelen = strlen( name ) + 1;
    if ( namelen > 1 )
    {
        m_pszName = new char[namelen];
        strcpy_s( m_pszName, namelen, name );
    }
    else
    {
        m_pszName = nullptr;
    }
    

    const size_t valuelen = strlen( value ) + 1;
    if ( valuelen > 1 )
    {
        m_pszValue = new char[valuelen];
        strcpy_s( m_pszValue, valuelen, value );
    }
    else
    {
        m_pszValue = nullptr;
    }


    CSystem::PrintDev( "Inserting option '%s' with value '%s'\n", m_pszName, m_pszValue );
}

CSettings_Section_Option::~CSettings_Section_Option()
{
    delete[] m_pszName;
    delete[] m_pszValue;
}
