/**
  @file App_VirtualMapping.h
  @author Aju Antony Jose
  @date January 30, 2019
  @brief Virtual mapping application header file

*/

#include "ADAS_Types.h"
#include "ADAS_Cfg.h"
#include "HAL_Serial_IF.h"
#include "CBuffer.h"

#ifndef COMMS_PLS_H
#define COMMS_PLS_H
//! PLS communication class
/*!
  This class provides functionality to communicate to PLS device
  initialize and request PLS for scan data
*/
class CPLSComms
{
  public:
    /*!
        @brief PLS communication status
    */
    typedef enum {
      MsgSuccess = 0,     /*!< Message status success. */
      CRCCheckFail = 1,   /*!< CRC check failure. */
      CommsError = 2,     /*!< communication error.*/
      PartialMsg = 3,     /*!< Partial message recieved. */
      Msg_NCK = 4,        /*!< Message not acknowledged. */
      InvalidReq = 5      /*!< Invalid request. */
    } Status_e;
    /**
    *   @brief Structure for ICC messages
    */
    typedef struct {
      uint8_t* data;            /**< buffer pointer*/
      uint16_t len;             /**< length of buffer */
      uint16_t start;           /**< location of start byte*/
      uint8_t messageID;        /**< message ID*/
    } Message_t;
    //! PLS communicator class constructor.
    /*!
      \brief  Instantiation and initialization
      \param[in] serPort reference to Serial interface object
    */
    CPLSComms(CSerial& serPort);
    //! PLS communicator class destructor.
    ~CPLSComms();
    //! PLS communication initialization
    /*!
      \brief  intialize class and PLS
      \return pls initialization status
    */
    Status_e Init(void);
    //! Request measurement from PLS
    /*!
      \param[in] onlyVert request only vertical if true
      \return pls initialization status
    */
    void RequestMeasurements(const bool onlyVert);
    //! Get asynchronous data recieved from PLS
    /*!
      \param[in,out] msg message recieved from pls
      \param[in,out] len message length
      \return false if no data available and true vice versa
    */
    bool GetAsyncData(Message_t& msg, uint16_t& len);
    //! check if any data recieved from PLS
    /*!
      \return true if recieved
    */
    bool DataAvailable();
    //! get pls communication status
    /*!
      \return Status_e communication status
    */
    Status_e GetStatus(void);
    //! Search recieved data for message
    /*!
      \brief search for message with message ID 'ID'
      \param[in] msg Message recieved from PLS
      \param[in] ID ID of message to be searched
      \param[in] len length of message
      \return boolean true if message found
    */
    bool SearchMsg(Message_t& msg, const uint8_t ID, const uint16_t len);
  private:
    //! Calculate CRC of message
    /*!
      \param[in] data Mpointer to buffer
      \param[in] len length of message
      \return boolean true if message found
    */
    uint16_t CalcCrC(const uint8_t* data, uint16_t len);
    //! Parse message content
    /*!
      \brief Parse and fill message structure with first message in data buffer
      \param[in,out] msg Message recieved from PLS
      \param[in] len length of message
      \return Status_e status of communication
    */
    Status_e  ParseMsgContent(Message_t& msg, const uint16_t len);
    //! Create PLS telegram packet
    /*!
      \param[in,out] buff buffer
      \param[in] Data message format buffer pointer
    */
    void  CreatePacket(CBuffAdas& buff, uint8_t* Data);

    CSerial& m_serPort;                         /*!< Serial interface object reference */
    uint8_t m_sndBuff[PLS_SND_BUFF_SIZE];       /*!< send buffer  */
    uint8_t m_rcvBuff[PLS_RCV_BUFF_SIZE];       /*!< recieve buffer  */
    Status_e m_status;                          /*!< status*/
};

#endif /*COMMS_PLS_H*/
