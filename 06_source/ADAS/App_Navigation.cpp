#include "App_Navigation.h"
#include <Arduino.h>
  
CNavigation::CNavigation(CPLSComms& plsComms):
    m_plsComms(plsComms)
{
  //do nothing
}
CNavigation::~CNavigation(){
  //do nothing
}

void CNavigation::Init(void)
{

}

void CNavigation::Run(void)
{
	//do nothing
}

void CNavigation::Stop(void)
{
	//do nothing
}

bool CNavigation::isTurning()
{
    return false;
}
bool CNavigation::isCornerMode()
{
    return false;
}