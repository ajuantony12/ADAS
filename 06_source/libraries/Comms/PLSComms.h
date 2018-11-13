/*H**********************************************************************
* FILENAME :        PLSCOmms.h             
*
* DESCRIPTION : PLS communication layer
*************************************************************************/

#ifndef COMMS_PLS_H
#define COMMS_PLS_H

#define MAX_RCV_BUF_LEN	10
#define MAX_SND_BUF_LEN	10

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
	bool send(unsigned char buff[]);
	/*
	* Stop
	*/
	bool recieve(unsigned char buff[]);
private:
	unsigned char m_sndBuff[MAX_SND_BUF_LEN];
	unsigned char m_rcvBuff[MAX_SND_BUF_LEN];
};

#endif /*COMMS_PLS_H*/
