/**
* @file Comm_Inertial.cpp
* @author Hannes Bähr, Juliane Müller
* @date January 31, 2019
* @brief Application file for intertial communication
* 
*/



// ##### Includes #####
#include "Comm_Inertial.h"
#include <Arduino.h>



//! Constructor of CInertialComm  
CInertialComm::CInertialComm()
{
  //do nothing
}

//! Destructor of CInertialComm  
CInertialComm::~CInertialComm(){
  //do nothing
}

//! Intitialization function of Inertial Communication
void CInertialComm::Init(void)
{

}
//! Send function of Inertial Communication
bool CInertialComm::send(uint8_t buff[], uint8_t len)
{
    //do nothing
    return true;
}
//! Receive function of Inertial Communication
uint8_t CInertialComm::recieve(uint8_t buff[], uint8_t len)
{
    //do nothing
    return 0;
}
