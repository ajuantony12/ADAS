/*H**********************************************************************
* FILENAME :        CBuffer.h             
*
* DESCRIPTION : Buffer class for data handling
*************************************************************************/
#include "ADAS_Types.h"

#ifndef C_BUFF_ADAS_TYPE
#define C_BUFF_ADAS_TYPE
class CBuffAdas
{
 public:
    /*COnstructor*/
    CBuffAdas(uint8_t* data, uint16_t len);
    /*destructor*/
    ~CBuffAdas();
    /*Append Data*/
    void Append(uint8_t data);
    /*Get Buffer Length*/
    uint16_t GetLength();
    uint8_t* GetData();
    /*Reset Buffer*/
    void Reset();
    uint32_t operator[](uint32_t index);
 private:
    uint8_t* m_Data;
    uint8_t m_Len;
    const uint8_t m_MaxLen;
};
#endif
