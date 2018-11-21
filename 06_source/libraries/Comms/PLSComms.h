/*H**********************************************************************
* FILENAME :        PLSCOmms.h             
*
* DESCRIPTION : PLS communication layer
*************************************************************************/
#include <ADAS_Types.h>

#ifndef COMMS_PLS_H
#define COMMS_PLS_H

class CPLSComms
{
public:
    CPLSComms();
    ~CPLSComms();
    /*
    * init
    */
    void init(void);
    /*
    * Send
    */
    bool send(uint8_t buff[], uint8_t len);
    /*
    * recieve
    */
    bool recieve(uint8_t buff[], uint8_t len);
private:
    static const MaxSndBuff = 10;
    static const MaxRecBuff = 10;
    uint8_t m_sndBuff[MaxSndBuff];
    uint8_t m_rcvBuff[MaxRecBuff];
};

#endif /*COMMS_PLS_H*/
