#include "ADAS_Debug.h"
#include "Comm_PLS.h"
#include <Arduino.h>



#define SICK_STX                    0x02
#define SICK_DEST                   0x00
#define SICK_DESTR                  0x80
#define SICK_RCV                    0x02
#define SICK_NACK                   0x92
#define SICK_STATUS                 0x31

#define SICK_PKT_HDR_LEN            0x04
#define SICK_CRC_HDR_LEN            0x02
#define SICK_D_FIELD_POS            0x02
#define SICK_ADDR_POS               0x01
#define SICK_MSG_B1_ST_POS          0x08

#define CRC16_GEN_POL 0x8005
#define MKSHORT(a,b) ((uint16_t) (a) | ((uint16_t)(b) << 8))

#ifdef ONE_TIME_CFG
static const uint8_t BM_TGM_BAUD500[] = {0x05, 0x02, 0x00, 0x20, 0x48};
static const uint8_t BPERMDEF_TGM[] = {0x04, 0x01, 0x00, 0x66};
static const uint8_t RSPTIMEDEF_TGM[] = {0x05, 0x02, 0x00, 0x63, 0x02};
static const uint8_t RSPTIMECONF_TGM[] = {0x05, 0x02, 0x00, 0x64, 0x02};
static const uint8_t BSFKFG_TGM[] = {0x0D, 0x0A, 0x00, 0x40, 0x00, 0x00, 0x00, 0x01, 0x00, 0x5f, 0x1E, 0x1E, 0x32};
static const uint8_t BSFCONF_TGM[] = {0x0D, 0x0A, 0x00, 0x41, 0x00, 0x00, 0x00, 0x01, 0x00, 0x5f, 0x1E, 0x1E, 0x32};
static const uint8_t WANL_TGM[] = {0x06, 0x03, 0x00, 0x44, 0x02, 0x00};
static const uint8_t WANLCFRM_TGM[] = {0x06, 0x03, 0x00, 0x47, 0x02, 0x00};
#else
static const uint8_t BM_TGM_SETUP[] = {0x0D, 0x0A, 0x00, 0x20, 0x00, 'S', 'I', 'C', 'K', '_', 'P', 'L', 'S'};
//static const uint8_t KSFKFG_TGM[] = {0x1B, 0x18, 0x00, 0x43, 0x0A, 0x18, 0x80, 0x19, 0x80, 0x1E, 0x80, 0x29, 0x80, 0x4E, 0x80, 0x4A, 0x80, 0x4E, 0x80, 0x5C, 0x80, 0x43, 0x80, 0x39, 0x80, 0x36, 0x80};
//static const uint8_t KSFKFG_TGM[] = {0x1B, 0x18, 0x00, 0x43, 0x0A, 0x35, 0x80, 0x38, 0x80, 0x41, 0x80, 0x5A, 0x80, 0x4D, 0x80, 0x49, 0x80, 0x4D, 0x80, 0x5A, 0x80, 0x41, 0x80, 0x38, 0x80, 0x35, 0x80};
static const uint8_t INIT_TGM[] = {0x04, 0x01, 0x00, 0x10};
static const uint8_t SSANF_TGM[] = {0x04, 0x01, 0x00, 0x31};
static const uint8_t ERRANF_TGM[] = {0x04, 0x01, 0x00, 0x32};
//ops change
//static const uint8_t BM_TGM_AllVal[] ={0x05, 0x02, 0x00, 0x20, 0x25};
//static const uint8_t BM_TGM_CONT[] ={0x05, 0x02, 0x00, 0x20, 0x24};
static const uint8_t BM_TGM_AllSeg[] = {0x05, 0x02, 0x00, 0x20, 0x21};
//static const uint8_t BM_TGM_Vert[] ={0x05, 0x02, 0x00, 0x20, 0x23};
// Get Frame
static const uint8_t GetFrameMsg[] = {0x06, 0x03, 0x00, 0x30, 0x00, 0x01};
static const uint8_t GetFrameMsgVrt[] = {0x06, 0x03, 0x00, 0x30, 0x00, 0x02};
#endif

CPLSComms::CPLSComms(CSerial& serPort)
  : m_serPort(serPort),
    m_status(Status_e::MsgSuccess)
{
  //do nothing
}
CPLSComms::~CPLSComms()
{
  //do nothing
}

CPLSComms::Status_e CPLSComms::Init(void)
{
  bool msgStatus = false;
  Message_t msg;
  uint16_t len;
  m_status = Status_e::MsgSuccess;
  CBuffAdas buffer(m_sndBuff, PLS_SND_BUFF_SIZE);
  /*Reset Laser*/
  buffer.Reset();
  CreatePacket(buffer, INIT_TGM);
  m_serPort.Send(m_sndBuff, buffer.GetLength());
  m_serPort.ReleaseBuffer();
  DPRINTLN("INIT message send");
  while (!m_serPort.Available());
  len = m_serPort.GetDataLen();
  if (0 < len)
  {
    m_serPort.GetData(m_rcvBuff, len);
    for (int i = 0; i < len; i++)
    {
        DPRINTLN(m_rcvBuff[i], HEX);
    }
    DPRINTLN("Recieved reset init packet");
    msgStatus = SearchMsg(msg, 0x91, len);
#ifdef ADAS_DEBUG
    if (msgStatus)
    {
      DPRINTLN("Reset Initiated\n\r");
    }
#endif
  }
  if ((Status_e::MsgSuccess == m_status) && (msgStatus))
  {
    m_status = Status_e::CommsError;
  while (!m_serPort.Available());
  len = m_serPort.GetDataLen();
    if (0 < len)
    {
      m_serPort.GetData(m_rcvBuff, len);
      msgStatus = SearchMsg(msg, 0x90, len);
      if (msgStatus)
      {
        m_status = Status_e::MsgSuccess;
        DPRINTLN("Powered up ");
      }
    }
  }
  if ((Status_e::MsgSuccess == m_status) && (msgStatus))
  {
    m_status = Status_e::CommsError;
    buffer.Reset();
    CreatePacket(buffer, BM_TGM_SETUP);
    m_serPort.Send(m_sndBuff, buffer.GetLength());
    m_serPort.ReleaseBuffer();
  while (!m_serPort.Available());
  len = m_serPort.GetDataLen();
    if (0 < len)
    {
      m_serPort.GetData(m_rcvBuff, len);
      msgStatus = SearchMsg(msg, 0xA0, len);
      if (msgStatus)
      {
        DPRINTLN("Mode changed to set up \n\r");
        m_status = (0 == msg.data[0]) ? Status_e::MsgSuccess : Status_e::CommsError;
      }
    }
  }
#if 0
  if ((Status_e::MsgSuccess == m_status) && (msgStatus))
  {
    m_status = Status_e::CommsError;
    buffer.Reset();
    CreatePacket(buffer, KSFKFG_TGM);
    m_serPort.Send(m_sndBuff, buffer.GetLength());
    if (MsgSuccess == RecievePkt(len))
    {
      msgStatus = SearchMsg(msg, 0xC3, len);
      if (msgStatus)
      {
        DPRINTLN("WF Configured\n\r");
        m_status = (1 == msg.data[0]) ? Status_e::MsgSuccess : Status_e::CommsError;
      }
    }
  }
#endif
  if ((Status_e::MsgSuccess == m_status) && (msgStatus))
  {
    m_status = Status_e::CommsError;
    buffer.Reset();
    CreatePacket(buffer, BM_TGM_AllSeg);
    m_serPort.Send(m_sndBuff, buffer.GetLength());
    m_serPort.ReleaseBuffer();
  while (!m_serPort.Available());
  len = m_serPort.GetDataLen();
    if (0 < len)
    {
      m_serPort.GetData(m_rcvBuff, len);
      msgStatus = SearchMsg(msg, 0xA0, len);
      if (msgStatus)
      {
        DPRINTLN("Mode changes\n\r");
        m_status = (0 == msg.data[0]) ? Status_e::MsgSuccess : Status_e::CommsError;
      }
    }
  }
  if ((Status_e::MsgSuccess == m_status) && (msgStatus))
  {
    m_status = Status_e::CommsError;
    buffer.Reset();
    CreatePacket(buffer, SSANF_TGM);
    m_serPort.Send(m_sndBuff, buffer.GetLength());
    m_serPort.ReleaseBuffer();
  while (!m_serPort.Available());
  len = m_serPort.GetDataLen();
    if (0 < len)
    {
      m_serPort.GetData(m_rcvBuff, len);
      msgStatus = SearchMsg(msg, 0xB1, len);
      if (msgStatus)
      {
        DPRINTLN("status response recieved\n\r");
        m_status = (0 == msg.data[SICK_MSG_B1_ST_POS]) ? Status_e::MsgSuccess : Status_e::CommsError;
      }
    }
  }
#ifdef ADAS_DEBUG
  switch (m_status)
  {
    case Status_e::MsgSuccess:
      DPRINTLN("success");
      break;
    case Status_e::InvalidReq:
      DPRINTLN("invalid");
      break;
    case Status_e::CommsError:
      DPRINTLN("comms error");
      break;
    case Status_e::CRCCheckFail:
      DPRINTLN("crc fail");
      break;
    case Status_e::Msg_NCK:
      DPRINTLN("NAK ");
      break;
    case Status_e::PartialMsg:
      DPRINTLN("Partial message");
      break;
  }
#endif
  DPRINTLN("end of PLS INIT");
  return m_status;
}

void CPLSComms::RequestMeasurements(bool onlyVert)
{
  CBuffAdas buffer(m_sndBuff, PLS_SND_BUFF_SIZE);
  buffer.Reset();
  if (onlyVert){
    CreatePacket(buffer, GetFrameMsgVrt);
  }else{
    CreatePacket(buffer, GetFrameMsg);
  }

  m_serPort.Send(m_sndBuff, buffer.GetLength());
  m_serPort.ReleaseBuffer();
}

bool CPLSComms::GetAsyncData(Message_t& msg, uint16_t& len)
{
   bool msgStatus = false;
  if (m_serPort.Available())
  {
      len = m_serPort.GetDataLen();
      if (0 < len)
      {
        m_serPort.GetData(m_rcvBuff, len);
        msgStatus = SearchMsg(msg, 0xB0, len);
        if (!msgStatus)
        {
          DPRINTLN("no measruement data recieved\n\r");
          m_status = CommsError;
        }
      }
      m_serPort.ReleaseBuffer();
  }
  return msgStatus;
}

CPLSComms::Status_e CPLSComms::GetStatus(void)
{
  return m_status;
}
uint16_t CPLSComms::CalcCrC(uint8_t* data, uint16_t len)
{
  uint16_t uCrc16;
  unsigned char abData[2];
  uCrc16 = 0;
  abData[0] = 0;
  while (len-- )
  {
    abData[1] = abData[0];
    abData[0] = *data++;
    if (uCrc16 & 0x8000)
    {
      uCrc16 = (uCrc16 & 0x7fff) << 1;
      uCrc16 ^= CRC16_GEN_POL;
    }
    else
    {
      uCrc16 <<= 1;
    }
    uCrc16 ^= MKSHORT (abData[0] , abData[1]);
  }
  return (uCrc16);
}

bool CPLSComms::SearchMsg(Message_t& msg, uint8_t ID, uint16_t len)
{
  bool retVal = false;
  bool NackRecv = false;
  uint16_t msgLen = len;
  while (true)
  {
    DPRINTLN("In search");
    m_status = ParseMsgContent(msg, 0, msgLen);
#ifdef ADAS_DEBUG
    DPRINT("Message ID = ");
    DPRINT(msg.messageID, HEX);
    DPRINT("\n\r");
    DPRINT("Message len = ");
    DPRINT(msg.len, HEX);
    DPRINT("\n\r");
    DPRINT("Message data = ");
    for (int i = 0; i < msg.len; i++)
    {
      DPRINT(msg.data[i], HEX);
      DPRINT(" ");
    }
    DPRINT("\n\r");
#endif
    if (Status_e::Msg_NCK == m_status) {
      NackRecv = true;
    }
    else if (Status_e::MsgSuccess == m_status)
    {
      if (ID == msg.messageID)
      {
        retVal = true;
        break;
      }
      else if (SICK_NACK == msg.messageID)
      {
        retVal = false;
        m_status = Status_e::Msg_NCK;
        break;

      }
      else {
        //do nothing
      }
    }
    else {
      if (Status_e::PartialMsg == m_status)
      {
        retVal = ID == msg.messageID;
      }
      else if (NackRecv)
      {
        m_status = Status_e::Msg_NCK;
      }
      else {
        //do nothing
      }
      break;
    }
    if (msgLen > (msg.len + msg.start + 6))
    {
      msgLen -= (msg.len + msg.start + 6);
      memmove(m_rcvBuff, &m_rcvBuff[msg.len], msgLen);
      DPRINT("REMAINING DATA = ");
      DPRINT(msgLen, HEX);
      DPRINT("\n\r");
    }
    else {
      break;
    }
  }
  return retVal;
}

CPLSComms::Status_e CPLSComms::ParseMsgContent(Message_t& msg, uint16_t start, uint16_t len)
{
  uint16_t crc;
  bool  flag = false;
  Status_e retVal = Status_e::CommsError;
  msg.messageID = 0;
  msg.len = 0;
  msg.start = 0;
  msg.data = NULL;
  uint16_t j = len;
  for (uint16_t i = 0 ; i < len; i++)
  {
    DPRINT(i, HEX);
    DPRINT(" | ");
    DPRINT(j, HEX);
    DPRINT("  ||  ");
    DPRINT(m_rcvBuff[i], HEX);
    DPRINT("  ||  ");
    if (SICK_NAK == m_rcvBuff[i])
    {
      DPRINTLN("NAK Recieved");
      retVal = Status_e::Msg_NCK;
      msg.start = i;
      msg.len++;
      memmove(m_rcvBuff, &m_rcvBuff[i], (len - i));
      msg.data = &m_rcvBuff[0];
      break;
    }
    if ((SICK_ACK == m_rcvBuff[i]) || (SICK_STX == m_rcvBuff[i]))
    {
      if (SICK_ACK == m_rcvBuff[i]) {
        i++; j--;
        DPRINTLN("ACK Recieved");
        if (SICK_PKT_HDR_LEN >= j ) {
          DPRINTLN("Not enough Length to proceed");
          break;
        }
      }
      if ((SICK_STX == m_rcvBuff[i]) &&
          (SICK_DESTR == m_rcvBuff[i + SICK_ADDR_POS]))
      {
        DPRINTLN("Packet recognized");
        msg.len =  (m_rcvBuff[i + SICK_D_FIELD_POS + 1] << 8) | m_rcvBuff[i + SICK_D_FIELD_POS] ;
        if (((msg.len + SICK_PKT_HDR_LEN + 2) <= (len - i)) && (0 < msg.len))
        {
          crc = CalcCrC(&m_rcvBuff[i], (msg.len + SICK_PKT_HDR_LEN ));
          if ((m_rcvBuff[i + SICK_PKT_HDR_LEN + msg.len ] == (crc & 0xFF)) &&
              (m_rcvBuff[i + SICK_PKT_HDR_LEN + msg.len + 1] == (crc >> 8)))
          {
            DPRINTLN("Packte recieved");
            retVal = Status_e::MsgSuccess;
          }
          else {
            DPRINTLN("CRC Failure");
            DPRINT(m_rcvBuff[i + msg.len ], HEX);
            DPRINT("  = ");
            DPRINT((crc & 0xFF), HEX);
            DPRINT("\n\r");
            DPRINT(m_rcvBuff[i + msg.len + 1], HEX);
            DPRINT(" = ");
            DPRINT((crc >> 8), HEX);
            DPRINT("\n\r");
            retVal = Status_e::CRCCheckFail;
          }
          msg.len--;
          msg.messageID = m_rcvBuff[i + SICK_PKT_HDR_LEN];
          msg.start = i + SICK_PKT_HDR_LEN;
          memmove(m_rcvBuff, &m_rcvBuff[i + SICK_PKT_HDR_LEN + 1], len - i - SICK_PKT_HDR_LEN - 1);
          msg.data = (msg.len) ? &m_rcvBuff[0] : NULL;
          break;
        }
        else if ((0 < msg.len))
        {
          DPRINTLN("Partial packet Recieved");
          retVal = Status_e::PartialMsg;
          msg.len = len - i - SICK_PKT_HDR_LEN;
          if (0 < msg.len) {
            msg.messageID = m_rcvBuff[i + SICK_PKT_HDR_LEN];
          }
          break;
        }
        else {
          //do nothing
        }
      }
      else {
        if (flag) {
          i--;
          j++;
        }
      }
    }
    j--;
  }
  return retVal;
}

void  CPLSComms::CreatePacket(CBuffAdas& buff, uint8_t* Data)
{
  uint16_t crc;
  buff.Append(SICK_STX);
  DPRINT(SICK_STX, HEX);
  DPRINT(" ");
  buff.Append(SICK_DEST);
  DPRINT(SICK_DEST, HEX);
  DPRINT(" ");
  for (int i = 1; i < Data[0]; i++)
  {
    buff.Append(Data[i]);
    DPRINT(Data[i], HEX);
    DPRINT(" ");
  }
  crc = CalcCrC(buff.GetData(), buff.GetLength());
  buff.Append(static_cast<uint8_t>(crc & 0x00FF));
  DPRINT(static_cast<uint8_t>(crc & 0x00FF), HEX);
  DPRINT(" ");
  buff.Append(static_cast<uint8_t>((crc >> 8) & 0x00FF));
  DPRINT(static_cast<uint8_t>((crc >> 8) & 0x00FF), HEX);
  DPRINT("\n\r");
}
bool CPLSComms::DataAvailable()
{
    return m_serPort.Available();
}
