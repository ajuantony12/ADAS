/**
* @file Comm_ICC.cpp
* @author Christoph Jurczyk
* @date January 30, 2019
* @brief Communication file for Inter-Controller Communication (ICC)
*
*/

#include "ADAS_Debug.h"
#include "Comm_ICC.h"
#include <Arduino.h>
#include "App_Navigation.h"

//! Constructor of CICCComms
/*!
	\param serPort Handover of serial interface object
*/
CICCComms::CICCComms(CSerial& serPort)
  : m_serPort(serPort), m_nav_o(NULL)
{
  //do nothing
}
//! Destructor of CICCComms
CICCComms::~CICCComms()
{
  //do nothing
}

//! Initialization function of CICCComms
/*!
	\param nav_o Handover of navigation unit object
*/
void CICCComms::Init(CNavigation* nav_o)
{
  m_nav_o = nav_o;
}

//! Run function of CICCComms which is periodically called by CNavigation
/*!
	Function reads serial buffer, parses and processes message. Received commands
	will call the dedicated functions.
*/
void CICCComms::Run(void)
{
  // Check if message is available
  if (m_serPort.Available())
  {
    // read ISR buffer and release ISR buffer
    m_serPort.GetData(m_rcvBuff, ICC_LEN);

    // Parse buffer
    ParseMsgContent(msg);

    DPRINT("msg.cmd=0x");
    DPRINTLN(msg.cmd, HEX);
    DPRINT("msg.data=");
    DPRINTLN(msg.data, DEC);

    switch (msg.cmd)
    {
      case ICC_CMD_FB_DIST:
        DPRINTLN("Distance done!");
        m_nav_o->distanceDone = true;
        break;

      case ICC_CMD_FB_ROT:
        DPRINTLN("Rotation done!");
        m_nav_o->rotationDone = true;
        break;
    }
  }
}

//! Function to parse received data into ::Message_t
/*!
	\param msg Message which shall be parsed
*/
void CICCComms::ParseMsgContent(Message_t& msg)
{
  msg.cmd = static_cast<uint8_t>(m_rcvBuff[2]);
  msg.data = static_cast<signed int>((m_rcvBuff[3] << 8) + m_rcvBuff[4]);
}


//! Function to send message to other controller
/*!
	\param msg Message which shall be sent
*/
void CICCComms::SendMsg(Message_t& msg)
{
  // Prepare buffer
  CBuffAdas buffer(m_sndBuff, ICC_SND_BUFF_SIZE);
  buffer.Reset();
  CreatePacket(buffer, msg);
  // Send  message
  m_serPort.Send(m_sndBuff, buffer.GetLength());
  m_serPort.ReleaseBuffer();
}

//! Function to prepare Message_t to raw data for serial transmission
/*!
	\param buff Pointer to transmission buffer
	\param msg Message which shall be sent
*/
void CICCComms::CreatePacket(CBuffAdas& buff, Message_t& msg)
{
  // Add STXx
  buff.Append(ICC_STX1);
  buff.Append(ICC_STX2);
  // Add command
  buff.Append(msg.cmd);
  // Add data
  buff.Append(static_cast<uint8_t>(msg.data >> 8));
  buff.Append(static_cast<uint8_t>(msg.data & 0x00FF));
  // Add TTX
  buff.Append(ICC_TTX);
}

//! Function to append message which shall be sent
/*!
	This function can be called outside of the class to send message to the other controller.
	\param cmd Command of message
	\param data Data of message
*/
void CICCComms::addTxMsg(uint8_t cmd, signed int data)
{
  Message_t txMsg;
  txMsg.cmd = cmd;
  txMsg.data = data;

  SendMsg(txMsg);
}

