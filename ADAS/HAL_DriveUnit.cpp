#include "HAL_DriveUnit.h"
#include <Arduino.h>
  
CDriveUnit::CDriveUnit(DriveID_e ID):
  m_ID(ID)
{
  //do nothing
}
CDriveUnit::~CDriveUnit(){
  m_ID = Drive1;
}

void CDriveUnit::Init(void){
	// to do :
}

 bool CDriveUnit::SetSpeed(uint8_t speed){
      // to do
 }
