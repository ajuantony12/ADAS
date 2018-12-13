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
    CBuffAdas(uint8_t* data, unsigned short len);
    /*destructor*/
    ~CBuffAdas();
    /*Append Data*/
    void Append(uint8_t data);
    /*Get Buffer Length*/
    unsigned short GetLength();
    uint8_t* GetData();
    /*Reset Buffer*/
    void Reset();
    unsigned int operator[](unsigned int index);
 private:
    uint8_t* m_Data;
    uint8_t m_Len;
    const uint8_t m_MaxLen;
};
#endif