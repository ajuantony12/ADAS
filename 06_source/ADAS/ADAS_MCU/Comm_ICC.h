/***********************************************************************
  FILENAME :        Comm_ICC.h

  DESCRIPTION : Inter-Controller communication layer
*************************************************************************/

#include "ADAS_Types.h"
#include "ADAS_Cfg.h"
#include "HAL_Serial_IF.h"
#include "CBuffer.h"

#ifndef COMMS_ICC_H
#define COMMS_ICC_H


class CICCComms
{
  public:

    typedef struct {
      uint8_t cmd;
      signed int data; // CJ: Why can I not use int16_t????
    } Message_t;

    CICCComms(CSerial& serPort);
    ~CICCComms();
    /*
      init
    */
    virtual void Init(void);
    /*
       Run
    */
    virtual void Run(void);

    /*
      Add tx telegram to sending buffer
    */
    virtual void addTxMsg(uint8_t cmd, signed int data);

    /*
       Received telegram
    */
    Message_t msg;

  private:
    CSerial& m_serPort;
    uint8_t m_sndBuff[ICC_SND_BUFF_SIZE];
    uint8_t m_rcvBuff[ICC_RCV_BUFF_SIZE];

    /*
        Parse received telegram
    */
    void  ParseMsgContent(Message_t& msg);

    /*
        Send message to other controller
    */
    void SendMsg(Message_t& msg);

    /*
       Function to prepare Message_t to raw data
    */
    void CreatePacket(CBuffAdas& buff, Message_t& msg);
};


#endif /*COMMS_ICC_H*/
