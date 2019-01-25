#include "ADAS_Debug.h"
#include "Comm_ICC.h"
#include <Arduino.h>
#include "App_MotorCtrl.h"

CICCComms::CICCComms(CSerial& serPort)
  : m_serPort(serPort)
{
  //do nothing
}
CICCComms::~CICCComms()
{
  //do nothing
}

void CICCComms::Init(CMotorCtrl* mCtrl_o)
{
  m_mCtrl_o=mCtrl_o;
}

void CICCComms::Run(void)
{
  // Check if message is available
  if (m_serPort.Available())
  {
    // read ISR buffer and release ISR buffer
    m_serPort.GetData(m_rcvBuff, ICC_LEN);

    // Parse buffer
    ParseMsgContent(msg);

	
  switch(msg.cmd){
    case ICC_CMD_DRIVE_DIST:
        m_mCtrl_o->setDistance(msg.data);
        DPRINT("set distance to: ");
        DPRINTLN(msg.data);
      break;
    case ICC_CMD_ROT_ANGLE:
      m_mCtrl_o->startRotation(msg.data);
      DPRINT("set rotation to: ");
        DPRINTLN(msg.data);
      break;
    case ICC_CMD_PAUSE_DRIVE:
      m_mCtrl_o->pauseDrive();
      DPRINTLN("Paused driving! ");
      break;
    case ICC_CMD_CONT_DRIVE:
      m_mCtrl_o->contDrive();
      DPRINTLN("Continued driving! ");
      break;
    case ICC_CMD_CONT_DRIVE_IN:
      m_mCtrl_o->setDistance(32767);
      DPRINTLN("Infinite Driving ");
      break;
    case ICC_CMD_SOFT_STOP:
      m_mCtrl_o->setDistance(0);
      DPRINTLN("Soft stop!");
      break;
    case ICC_CMD_EMER_STOP:
      m_mCtrl_o->setDistance(0);
      DPRINTLN("Emergency stop! ");
      break;
    }
	
	

  }
}

// Function to parse received data
void CICCComms::ParseMsgContent(Message_t& msg)
{
  msg.cmd = static_cast<uint8_t>(m_rcvBuff[2]);
  msg.data = static_cast<signed int>((m_rcvBuff[3] << 8) + m_rcvBuff[4]);
}


// Function to send message to other controller
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

// Function to prepare Message_t to raw data
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

// Function to append tx telegram
void CICCComms::addTxMsg(uint8_t cmd, signed int data)
{
  Message_t txMsg;
  txMsg.cmd = cmd;
  txMsg.data = data;

  SendMsg(txMsg);
}
