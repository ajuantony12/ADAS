#include "PLSComms.h"
#include "Arduino.h"
  
CPLSComms::CPLSComms(CSerial& serPort)
    :m_serPort(serPort),
    m_status(Status_e::MsgSuccess),
    m_asyncDataFLag(false)
{
  //do nothing
}
CPLSComms::~CPLSComms()
{
  //do nothing
}

void CPLSComms::Init(void)
{

}

bool CPLSComms::GetMeasurements(uint8_t* buff[], uint8_t& len)
{
    return true;
}
bool CPLSComms::IsPFBreached(unsigned int& distToObj)
{
    return true;
}

void CPLSComms::AsyncMessageUpdate(uint8_t* buff[], uint8_t len)
{
    
}

CPLSComms::Status_e CPLSComms::GetStatus(void)
{
    return Status_e::MsgSuccess;
}
unsigned short CPLSComms::CalcCrC(uint8_t* data, unsigned short len)
{
    return 0;
}
CPLSComms::Status_e CPLSComms:: ParseMsg(void)
{
    return Status_e::MsgSuccess;
}