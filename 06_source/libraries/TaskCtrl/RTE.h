/*H**********************************************************************
* FILENAME :        RTE.h             
*
* DESCRIPTION : Runtime enviroment for data storage
*************************************************************************/

#ifndef RTE_BLOCK_H
#define RTE_BLOCK_H

class CRTE
{
public:
	CRTE();
	~CRTE();
  /*
   * init
   */
   void init(void);
  /*
   * Read Data
   */
  unsigned char ReadData(void);
  /*
   * Read Data
   */
  void WriteData(unsigned char& data);
private:
	unsigned char m_data;
  
};

#endif /*RTE_BLOCK_H*/
