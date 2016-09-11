#pragma once

#include <vector>

class CSettings_Section_Option
{
public:
    CSettings_Section_Option( const char* name, const char* value );
    ~CSettings_Section_Option();

    const char* GetName() { return m_pszName; };
    const char* GetValue() { return m_pszValue; };

private:
    char* m_pszName;
    char* m_pszValue;
};

class CSettings_Section
{
public:
    CSettings_Section( const char* name );
    ~CSettings_Section();


    void InsertOption( const char* name, const char* value );


    // Will null value if option is not found.
    bool GetOptionValue( const char* name, char* value, size_t len = 0 );

    const char* GetOptionValue( const char* name );


    int GetOptionIndex( const char* name );



    bool GetSectionName( char* name, size_t len );
    const char* GetSectionName() { return m_pszSectionName; }

    void SetSectionName( const char* name );

    bool HasValidName() { return m_pszSectionName != nullptr; }


private:
    char* m_pszSectionName;


    std::vector<CSettings_Section_Option*> m_Options;
};