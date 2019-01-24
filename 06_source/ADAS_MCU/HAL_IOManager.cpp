#include "HAL_IOManager.h"
#include <Arduino.h>

CIOManager::Pin_s CIOManager::m_Ports[CIOManager::Max_IO] = { {1, 1,1,0},
                                               {1, 1,1,0}};
CIOManager::CIOManager()
{
  //do nothing
}
CIOManager::~CIOManager(){
  //do nothing
}

void CIOManager::Init(void){
	// to do
}

void CIOManager::SetPin(uint8_t pinID){
  //to do
}

void CIOManager::ResetPin(uint8_t pinID){
  //to do
}

void CIOManager::TogglePin(uint8_t pinID){
  //to do
}