#include "ADAS_Debug.h"
#include "Comm_ICC.h"
#include <Arduino.h>

CICCComms::CICCComms(CSerial& serPort)
  : m_serPort(serPort)
{
  //do nothing
}
CICCComms::~CICCComms()
{
  //do nothing
}

void CICCComms::Init(void)
{
  //do nothing
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

    // Do somethin with msg data
	:
	/* Example_
	if(msg.cmd == 0x04)
	{
		// new rotation
		 mcu.startRotation(msg.data);
	}
	*/
	
	

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

