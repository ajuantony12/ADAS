#include "CBuffer.h"

//! Cbuffer constructor.
CBuffAdas::CBuffAdas(uint8_t* data, const uint16_t len)
  : m_Data(data),
    m_Len(0),
    m_MaxLen(len)
{
  //do nothing
}
//! Cbuffer destructor.
CBuffAdas::~CBuffAdas()
{
  m_Data = 0;
  m_Len = 0;
}
//! Append data 
void CBuffAdas::Append(const uint8_t data)
{
  if (m_Len < m_MaxLen)
  {
    m_Data[m_Len++] = data;
  }
}
//! get lenght
uint16_t CBuffAdas::GetLength()
{
  return m_Len;
}
//! get buffer pointer
uint8_t* CBuffAdas::GetData()
{
  return m_Data;
}
//! reset buffer
void CBuffAdas::Reset()
{
  m_Len = 0;
}
//! operator overloading []
uint32_t CBuffAdas::operator[](const uint32_t index)
{
  uint32_t index4 = index * 4;
  return (static_cast<uint32_t>(m_Data[index4]) &
          (static_cast<uint32_t>(m_Data[index4 + 1]) << 8) &
          (static_cast<uint32_t>(m_Data[index4 + 2]) << 16) &
          (static_cast<uint32_t>(m_Data[index4 + 3]) << 24));
}
