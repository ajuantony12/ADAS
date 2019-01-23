#include "ADAS_Cfg.h"
#include "HAL_Serial_IF.h"
#include <Arduino.h>

CSerial::CSerial(PortID_e i_ID, uint16_t i_bufLen):
  m_ID(i_ID),
  m_rxBufLen(i_bufLen),
  m_rxMsgLen(0),
  m_rxBufferPointer(0),
  m_rxBufferFull(false),
  m_rxBufferRdy(false)
{
  m_rxBuffer = new uint8_t[i_bufLen];
}
CSerial::~CSerial() {
  m_rxMsgLen = 0U;
  m_rxBufferPointer = 0U;
  m_rxBufferFull = false;
  m_rxBufferRdy = false;
  delete[] m_rxBuffer;
}

void CSerial::Init(void) {
  if (S1 == m_ID) {

    // Set baudrate
    UBRR1L = SERIAL1_BAUD_PRESCALE;
    UBRR1H = (SERIAL1_BAUD_PRESCALE >> 8);

    // Ayncrhonous USART
    bitWrite(UCSR1C, UMSEL10, 0);
    bitWrite(UCSR1C, UMSEL11, 0);

    // Enable even parity
    bitWrite(UCSR1C, UPM10, 0);
    bitWrite(UCSR1C, UPM11, 1);

    // 8-bit mode
    bitWrite(UCSR1C, UCSZ12, 0);
    bitWrite(UCSR1C, UCSZ11, 1);
    bitWrite(UCSR1C, UCSZ10, 1);


    // Enable Receiver and Transmitter
    bitWrite(UCSR1B, RXEN1, 1);
    bitWrite(UCSR1B, TXEN1, 1);

    // Enable receive interrupt
    bitWrite(UCSR1B, RXCIE1, 1);
  } else {
    // Set baudrate
    UBRR1L = SERIAL2_BAUD_PRESCALE;
    UBRR1H = (SERIAL2_BAUD_PRESCALE >> 8);

    // Ayncrhonous USART
    bitWrite(UCSR2C, UMSEL20, 0);
    bitWrite(UCSR2C, UMSEL21, 0);

    // Enable even parity
    bitWrite(UCSR2C, UPM20, 0);
    bitWrite(UCSR2C, UPM21, 1);

    // 8-bit mode
    bitWrite(UCSR2C, UCSZ22, 0);
    bitWrite(UCSR2C, UCSZ21, 1);
    bitWrite(UCSR2C, UCSZ20, 1);


    // Enable Receiver and Transmitter
    bitWrite(UCSR2B, RXEN2, 1);
    bitWrite(UCSR2B, TXEN2, 1);

    // Enable receive interrupt
    bitWrite(UCSR2B, RXCIE2, 1);
  }
  sei();
}

bool CSerial::Send(char Buff[], uint8_t len) {
  uint8_t index;
  bool retVal = false;
  // wait for free tx buffer
  if ((NULL != Buff) && (0 < len ))
  {
    if (S1 == m_ID) {

      for (index = 0U; index < len; index++)
      {
        while ( !( UCSR1A & (1 << UDRE1)));
        // send data
        UDR1 = Buff[index];
      }
    } else {
      for (index = 0U; index < len; index++)
      {
        while ( !( UCSR2A & (1 << UDRE2)));
        // send data
        UDR2 = Buff[index];
      }
    }
    retVal = true;
  }
  return retVal;
}

bool CSerial::Available(void)
{
  return m_rxBufferRdy;
}
uint16_t CSerial::GetDataLen(void)
{
  return (m_rxMsgLen + 6);
}
bool CSerial::GetData(uint8_t* data, uint16_t len)
{
  bool retVal = false;
  if (len <= (m_rxMsgLen + 6))
  {
    memcpy(data, m_rxBuffer, len);
    ReleaseBuffer();
    retVal = true;
  }
  return retVal;
}

void CSerial::ReleaseBuffer(void)
{
  m_rxBufferPointer = 0U;
  m_rxBufferRdy = false;
  m_rxBufferFull = false;
}

void CSerial::SerialISRcommPLS(void)
{
  if ((!m_rxBufferFull) && (!m_rxBufferRdy))
  {
    // Read incoming data

    // Write rx data to buffer
    m_rxBuffer[m_rxBufferPointer] = (m_ID == S1) ? UDR1 : UDR2;
    m_rxBufferPointer++;


    if (m_rxBufferPointer == 2)
    {
      // Check for start byte and PLS address
      if ((m_rxBuffer[0] != SICK_STX) || (m_rxBuffer[1] != SICK_DESTR))
      {
        // STX & ADR not in first two bytes => overwrite received data
        m_rxBuffer[0] = m_rxBuffer[1];
        m_rxBufferPointer = 1;
      }
    } else if (m_rxBufferPointer == 4)
    {
      // Check length of telegram
      m_rxMsgLen = (m_rxBuffer[3] << 8) + m_rxBuffer[2];
    } else if (m_rxBufferPointer == m_rxMsgLen + 6) // + 2 for CRC
    {
      // Received full telegram
      m_rxBufferRdy = true;
    } else if (m_rxBufferPointer == m_rxBufLen)
    {
      // Set flag if buffer is full
      m_rxBufferFull = true;
    }
  } else {
    // flush data
    if (m_ID == S1)
    {
      (void) UDR1;
    } else {
      (void) UDR2;
    }

  }
}

// ISR function handler for inter-controller communication
void CSerial::SerialISRcommICC(void)
{
  // Check for free buffer
  if (!m_rxBufferRdy)
  {
    // Read incoming data

    // Write rx data to buffer
    m_rxBuffer[m_rxBufferPointer] = (m_ID == S1) ? UDR1 : UDR2;
    m_rxBufferPointer++;

    // wait for 2 bytes to detect STXs
    if (m_rxBufferPointer == 2)
    {
      // Check for start bytes
      if ((m_rxBuffer[0] != ICC_STX1) || (m_rxBuffer[1] != ICC_STX1))
      {
        // STXs not found => overwrite received data
        m_rxBufferPointer = 0;
      } else if (m_rxBufferPointer == ICC_LEN)
      {
        // Received full telegram
        // Check for TTX
        if (m_rxBuffer[5] != ICC_TTX)
        {
          // TTX not received => discard telegram
          m_rxBufferPointer = 0;
        } else {
          // TTX received => ready to progress
          m_rxBufferRdy = true;
        }
      }
    }
  } else {
    // flush data
    if (m_ID == S1)
    {
      (void) UDR1;
    } else {
      (void) UDR2;
    }
  }
}

