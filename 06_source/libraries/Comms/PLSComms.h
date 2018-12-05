/*H**********************************************************************
* FILENAME :        PLSCOmms.h             
*
* DESCRIPTION : PLS communication layer
*************************************************************************/
#include <ADAS_Types.h>
#include <ADAS_Cfg.h>
#include <Serial_IF.h>
#include <CBuffer.h>

#ifndef COMMS_PLS_H
#define COMMS_PLS_H



class CPLSComms
{
public:
    typedef enum {
        MsgSuccess,
        CRCCheckFail,
        CommsError
    } Status_e;

    CPLSComms(CSerial& serPort);
    ~CPLSComms();
    /*
    * init
    */
    void Init(void);
    /*
    * Get Measurements
    */
    bool GetMeasurements(uint8_t* buff[], uint8_t& len);
    /*is protectuve field breached
    */
    bool IsPFBreached(unsigned int& distToObj);
    /*
    * Asynchronous Data update from PLS
    */
    void AsyncMessageUpdate(uint8_t* buff[], uint8_t len);
    /*
    * Get status of PLS
    */
    Status_e GetStatus(void);
private:
    typedef struct {
        unsigned int len;
        uint8_t messageID;
        uint8_t* data;
    } Message_t;
    /*
    * Calculate CRC
    */
    unsigned short CalcCrC(uint8_t* data, unsigned short len);
    /*
    * Parse recieved message
    */
    Status_e  ParseMsgContent(Message_t& msg);
    /*
    * Recieve a packet
    */
    Status_e  RecievePkt(void);
    /*
    * Parse recieved message
    */
    void  CreatePacket(CBuffAdas& buff, uint8_t* Data);
    
    CSerial& m_serPort;
    uint8_t m_sndBuff[SND_BUFF_SIZE];
    uint8_t m_rcvBuff[RCV_BUFF_SIZE];
    Status_e m_status;
    bool m_asyncDataFLag;
};

#endif /*COMMS_PLS_H*/
