/*H**********************************************************************
* FILENAME :        PLSCOmms.h             
*
* DESCRIPTION : PLS communication layer
*************************************************************************/
#include <ADAS_Types.h>
#include <ADAS_Cfg.h>
#include <Serial_IF.h>

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
    /*
    * Calculate CRC
    */
    unsigned short CalcCrC(uint8_t* data, unsigned short len);
    /*
    * Parse recieved message
    */
    Status_e  ParseMsg(void);
    
    CSerial& m_serPort;
    Status_e m_status;
    bool m_asyncDataFLag;
    uint8_t m_sndBuff[SND_BUFF_SIZE];
    uint8_t m_rcvBuff[RCV_BUFF_SIZE];
};

#endif /*COMMS_PLS_H*/
