#pragma once


class COffsetParser;

class COffset
{
public:
    COffset( COffsetParser* owner, const char* name, const char* offset );
    COffset( COffsetParser* owner, const char* name, offset_t offset );
    ~COffset();

    void Parse( const char* offset );

    const char* GetName() { return m_pszName; }
    offset_t GetOffset() { return m_off; }

private:
    void SetName( const char* name );

    offset_t m_off;

    char* m_pszName;


    COffsetParser* m_Owner;
};
