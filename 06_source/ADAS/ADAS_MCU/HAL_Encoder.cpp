// ##### Includes #####
#include "HAL_Encoder.h"
#include "Arduino.h"
#include "ADAS_Debug.h"
#include "ADAS_Cfg.h"

  //! Constructor of CEncoder
CEncoder::CEncoder(EncoderID_e ID):
  m_ID(ID),
  peaks(0),
  count(0),
  sum(0)
{
//! Destructor of CEncoder
}
CEncoder::~CEncoder(){
  m_ID = E1;
}

//! Initialization of CEncoder
/*!
  The pin of the encoder is initialized 
*/
void CEncoder::Init(){
  //Initialize encoder interrupts
  pinMode(m_ID, INPUT_PULLUP);
  
}

//! Function to read out the peak count per 150ms
/*!
  In this function the peaks are counted. It is called by the MCU every 150ms, which defines the
  counting time of the peaks. 
*/
uint16_t CEncoder::ReadCount()
{
	  count = peaks;
    peaks = 0;
    return count;
}

//! Function to sum up the counted peaks
/*!
  In this function the counted peaks per 150ms are summed up. It is called by the MCU every 150ms.  
*/
uint16_t CEncoder::ReadSum()
{
    sum = sum + count;

    return sum;
}

//! Function to reset all values of the encoder
void CEncoder::reset(void){
    peaks = 0;
    count = 0;
    sum = 0;
  }
//! Interrupt Service Routine of the encoder
/*!
  In this ISR the peaks of the left encoder are counted.
*/
void CEncoder::EncISR(void)
{
  peaks++;
}
