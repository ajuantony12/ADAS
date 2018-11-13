/*H**********************************************************************
* FILENAME :        InterialComms.h             
*
* DESCRIPTION : Inertial system communication layer
*************************************************************************/

#ifndef COMMS_INERTIAL_SYS_H
#define COMMS_INERTIAL_SYS_H

#define MAX_RCV_BUF_LEN	10
#define MAX_SND_BUF_LEN	10

class CInertialComm
{
public:
	CInertialComm();
	~CInertialComm();
  /*
   * init
   */
   void init(void);
  /*
   * Send
   */
  bool send(unsigned char buff[]);
  /*
   * Stop
   */
  bool recieve(unsigned char buff[]);
private:
	unsigned char m_sndBuff[MAX_SND_BUF_LEN];
	unsigned char m_rcvBuff[MAX_SND_BUF_LEN];
};

#endif /*COMMS_INERTIAL_SYS_H */
