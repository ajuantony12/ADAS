/**
  @file Cbuffer.h
  @author Aju Antony Jose
  @date January 30, 2019
  @brief general purpose buffer

*/

#include "ADAS_Types.h"

#ifndef C_BUFF_ADAS_TYPE
#define C_BUFF_ADAS_TYPE
class CBuffAdas
{
  public:
    //! Cbuffer constructor.
    /*!
      \brief  Instantiation and initialization
      \param[in] data buffer memory pointer
      \param[in] len length of buffer memory
    */
    CBuffAdas(uint8_t* data, const uint16_t len);
    //! Cbuffer destructor.
    ~CBuffAdas();
    //! Append data 
    /*!
      \param[in] data byte to be appended
    */
    void Append(const uint8_t data);
    //! get lenght
    /*!
      \return length of used buffer
    */
    uint16_t GetLength();
    //! get buffer pointer
    /*!
      \return pointer to buffer
    */
    uint8_t* GetData();
    //! reset buffer
    void Reset();
    //! operator overloading []
    /*!
      \param index index
      \return uint32_t
    */
    uint32_t operator[](const uint32_t index);  
  private:
    uint8_t* m_Data;                /*!< buffer pointer*/
    uint8_t m_Len;                  /*!< buffer length*/
    const uint8_t m_MaxLen;         /*!< max buffer length */
};
#endif
