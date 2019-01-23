#include "CBuffer.h"


CBuffAdas::CBuffAdas(uint8_t* data, uint16_t len)
                    :m_Data(data),
                    m_Len(0),
                    m_MaxLen(len)
{
    //do nothing
}
CBuffAdas::~CBuffAdas()
{
    m_Data = 0;
    m_Len = 0;
}
void CBuffAdas::Append(uint8_t data)
{
    if (m_Len < m_MaxLen)
    {
        m_Data[m_Len++] = data;
    }
}
uint16_t CBuffAdas::GetLength()
{
    return m_Len;
}
uint8_t* CBuffAdas::GetData()
{
    return m_Data;
}
/*Reset Buffer*/
void CBuffAdas::Reset()
{
    m_Len = 0;
}
uint32_t CBuffAdas::operator[](uint32_t index)
{

    uint32_t index4 = index * 4;
    return (static_cast<uint32_t>(m_Data[index4])& 
                (static_cast<uint32_t>(m_Data[index4+1]) << 8) & 
                (static_cast<uint32_t>(m_Data[index4+2]) <<16) & 
                (static_cast<uint32_t>(m_Data[index4+3]) <<24));

}
