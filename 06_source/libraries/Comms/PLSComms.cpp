#include "PLSComms.h"
#include "Arduino.h"



#define SICK_STX                    0x02
#define SICK_DEST                   0x00
#define SICK_DESTR                  0x80
#define SICK_RCV                    0x02
#define SICK_ACK                    0x06
#define SICK_NAK                    0x15
#define SICK_NACK                   0x92
#define SICK_STATUS                 0x31

#define CRC16_GEN_POL 0x8005
#define MKSHORT(a,b) ((unsigned short) (a) | ((unsigned short)(b) << 8))

#ifdef ONE_TIME_CFG
static const uint8_t BM_TGM_SETUP[] ={0x0D 0x0A, 0x00, 0x20, 0x00, 'S', 'I', 'C', 'K', '_', 'L', 'S', 'I'};
static const uint8_t BM_TGM_BAUD500[] ={0x05, 0x04, 0x00, 0x20,0x48};
static const uint8_t BPERMDEF_TGM[] ={0x04, 0x04, 0x00, 0x66};
static const uint8_t RSPTIMEDEF_TGM[] ={0x05, 0x04, 0x00, 0x63, 0x02};
static const uint8_t RSPTIMECONF_TGM[] ={0x05, 0x04, 0x00, 0x64, 0x02};
static const uint8_t BSFKFG_TGM[] ={0x0D, 0x0A, 0x00, 0x40, 0x00, 0x00, 0x00, 0x01, 0x00, 0x5f, 0x1E, 0x1E, 0x32};
static const uint8_t BSFCONF_TGM[] ={0x0D, 0x0A, 0x00, 0x41, 0x00, 0x00, 0x00, 0x01, 0x00, 0x5f, 0x1E, 0x1E, 0x32};
static const uint8_t WANL_TGM[] = {0x06, 0x04, 0x00, 0x44, 0x02, 0x00};
static const uint8_t WANLCFRM_TGM[] = {0x06, 0x04, 0x00, 0x47, 0x02, 0x00};
static const uint8_t KSFKFG_TGM[] = {0x0E, 0x0B, 0x00, 0x43, 0x09, 0x32, 0x47, 0x70, 0x67, 0x64, 0x67, 0x70, 0x47, 0x32};
#else
static const uint8_t INIT_TGM[] ={0x04, 0x03, 0x00, 0x10};
static const uint8_t SSANF_TGM[] ={0x04, 0x03, 0x00, 0x31};
static const uint8_t ERRANF_TGM[] ={0x04, 0x03, 0x00, 0x32};
//ops change
static const uint8_t BM_TGM_AllVal[] ={0x05, 0x04, 0x00, 0x20, 0x25};
static const uint8_t BM_TGM_AllSeg[] ={0x05, 0x04, 0x00, 0x20, 0x21};
static const uint8_t BM_TGM_Vert[] ={0x05, 0x04, 0x00, 0x20, 0x23};
// Get Frame
static const uint8_t GetFrameMsg[] ={0x05, 0x04, 0x00, 0x40, 0x00};
#endif



CPLSComms::CPLSComms(CSerial& serPort)
    :m_serPort(serPort),
    m_status(Status_e::MsgSuccess),
    m_asyncDataFLag(false)
{
  //do nothing
}
CPLSComms::~CPLSComms()
{
  //do nothing
}

void CPLSComms::Init(void)
{
    char buff[40] = {0};
    m_status= Status_e::CommsError;
    CBuffAdas buffer(m_sndBuff, SND_BUFF_SIZE);
    /*Reset Laser*/
    CreatePacket(buffer, INIT_TGM);
    m_serPort.Send(m_sndBuff, buffer.GetLength());
    buffer.Reset();
    Serial.write("INIT message send \n\r");
/*      if (m_serPort.Read(m_rcvBuff, 1))
    {
        sprintf(buff,"recieved %x\n\r", m_rcvBuff[0]);
        Serial.write(buff);
        if (0x91 == m_rcvBuff[0])
        {
            Serial.write("waiting for 91\n\r");
            while (!m_serPort.Available());
            (void)m_serPort.Read(m_rcvBuff, 1);
            snprintf(buff, "recieved %x\n\r", m_rcvBuff[0]);
            Serial.write(buff);
            if (0x91 == m_rcvBuff[0])
            {
                m_status = Status_e::MsgSuccess;
            }
        }
    } 
    Serial.write("\n\r"); */
#ifdef ONE_TIME_CFG
    //try;
#endif
/*     if (Status_e::MsgSuccess == m_status)
    {
        CreatePacket(buffer, SSANF_TGM);
        m_serPort.Send(m_sndBuff, buffer.GetLength());
        buffer.Reset();

        CreatePacket(buffer, BM_TGM_AllVal);
        m_serPort.Send(m_sndBuff, buffer.GetLength());
        buffer.Reset();
    } */

}

bool CPLSComms::GetMeasurements(uint8_t* buff[], uint8_t& len)
{
    CBuffAdas buffer(m_sndBuff, SND_BUFF_SIZE);
    CreatePacket(buffer, GetFrameMsg);
    m_serPort.Send(m_sndBuff, buffer.GetLength());
    
    return true;
}
bool CPLSComms::IsPFBreached(unsigned int& distToObj)
{
    return true;
}

void CPLSComms::AsyncMessageUpdate(uint8_t* buff[], uint8_t len)
{
    
}

CPLSComms::Status_e CPLSComms::GetStatus(void)
{
    return Status_e::MsgSuccess;
}
unsigned short CPLSComms::CalcCrC(uint8_t* data, unsigned short len)
{
    unsigned short uCrc16;
    unsigned char abData[2];
    uCrc16 = 0;
    abData[0] = 0;
    while (len-- )
    {
        abData[1] = abData[0];
        abData[0] = *data++;
        if(uCrc16 & 0x8000)
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
    return(uCrc16);
}
CPLSComms::Status_e CPLSComms::ParseMsgContent(Message_t& msg)
{
    unsigned short crc;
    int i = 0;
    Status_e retVal = Status_e::CommsError;
    uint8_t data = m_rcvBuff[i];
    if (SICK_NACK !=data)
    {
       if (SICK_ACK == data)
       {
           data = m_rcvBuff[++i];
       }
       if ((SICK_STX == data)&&
            (SICK_DESTR == m_rcvBuff[i+1]))
       {
            msg.len =  (m_rcvBuff[i+3] << 8) | m_rcvBuff[i+2] ;
            crc = CalcCrC(&m_rcvBuff[i], msg.len );
            if ((m_rcvBuff[i+msg.len ] == (crc & 0xFF))&&
                (m_rcvBuff[i+msg.len +1] == (crc >> 8)))
            {
                memmove(m_rcvBuff, &m_rcvBuff[i+4], msg.len);
                msg.len--;
                msg.messageID= m_rcvBuff[0];
                msg.data = (msg.len)?&m_rcvBuff[1]:NULL;
            }
            else
            {
                retVal = Status_e::CRCCheckFail;
            }
       }
    }
    return retVal;
}

CPLSComms::Status_e CPLSComms::RecievePkt   (void)
{
    Status_e retVal = Status_e::CommsError;
    bool flag = false;
    uint8_t count = 0;
    unsigned short len = 0;
    while (!flag)
    {
        if (m_serPort.Read(m_rcvBuff, 1))
        {
            flag = true;
            if (SICK_ACK !=m_rcvBuff[0])
            {
                if (m_serPort.Read(m_rcvBuff, 4))
                {
                    if ((SICK_STX == m_rcvBuff[0]) &&
                        (SICK_DESTR == m_rcvBuff[1]))
                    {
                        len = ((static_cast<unsigned short>(m_rcvBuff[3]) << 8) | m_rcvBuff[2])+2;
                        retVal = m_serPort.Read(&m_rcvBuff[4], len)? Status_e::MsgSuccess :Status_e::CommsError;
                    }
                }
            }
        }
        else{
            count++;
            if (10U == count)
            {
                flag = true;
            }
        }
    }
    return retVal;
}

void  CPLSComms::CreatePacket(CBuffAdas& buff, uint8_t* Data)
{
    char buffer[5] ={0};
    unsigned short crc;
    buff.Append(SICK_STX);
    buff.Append(SICK_DEST);
    for (int i = 1; i < Data[0]; i++)
    {
        buff.Append(Data[i]);
        sprintf(buffer,"%x\n\r",Data[i]);
        Serial.write(buffer, 5);
    }
    crc = CalcCrC(buff.GetData(), buff.GetLength());
    buff.Append(static_cast<uint8_t>(crc & 0x00FF));
    buff.Append(static_cast<uint8_t>((crc >> 8) & 0x00FF));
}