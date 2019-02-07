/**
  @file App_VirtualMapping.h
  @author Aju Antony Jose
  @date January 30, 2019
  @brief Virtual mapping application header file

*/

#include "Task_if.h"
#include "ADAS_Types.h"
#include "Comm_PLS.h"
#include "App_Navigation.h"

#ifndef APP_V_MAPPING_H
#define APP_V_MAPPING_H

//! Virtual mapping application class
/*!
  Virtual mapping application class.
  Contains functionalities to request scan details from PLS and
  determine offset to wall and angle of wall with respect to autonomus vechicle
*/
class CVMapping: public ITask_IF
{
  public:
    //! Virtual mapping app class constructor.
    /*!
      \brief  Instantiation and initialization
      \param[in] NAV reference to navigation app class object
      \param[in] plsComms refernce to pls communication class object
    */
    CVMapping(CNavigation& NAV, CPLSComms& plsComms);
    //! Virtual mapping app class destructor.
    ~CVMapping();
    //! Initialization function of virtual mapping Application
    /*!
      \brief Realization of pure virtual function task_IF
             Called by task controller for initializing applications
    */
    virtual void Init(void);
    //! Run function of virtual mapping Application
    /*!
      \brief Realization of pure virtual function task_IF
             Called by task controller for running functionality
    */
    virtual void Run(void);
    //! Stop/shutdown function of virtual mapping Application
    /*!
      \brief Realization of pure virtual function task_IF
             Called by task controller for shutdown
    */
    virtual void Stop(void);
  private:
    //! organize the data buffer by removing unwanted fields
    /*!
      \param[in,out] data pointer to scanned data buffer
      \param[in] len length of buffer
      \return length of segements in data buffer
    */
    uint16_t OrganizeData(const uint16_t* data, const uint16_t len);
    //! search for field breach
    /*!
      \brief check for protective field breach
             or right side of warning field breach
      \param[in] data pointer to scanned data buffer
      \param[in] len length of buffer
      \param[out] pfStatus protective field breach status
      \return boolean
      \retval true protective fiel or right side warning field breached
              false no breach detected
    */
    bool searchforPFBreach(const uint16_t* data, const uint16_t len, bool& pfStatus);
    //! Calculate and update wall information to Navigation application
    /*!
      \brief calculate wall information
             update new values to navigation application.
      \param[in] data pointer to scanned data buffer
      \param[in] len length of buffer
      \param[in] wfStatus warning field breach status
      \param[in] pfStatus warning field breach status
    */
    void calculateWallInfo(const uint16_t* data, const uint16_t len,
                           const bool wfStatus, const bool pfStatus);
    //! cordic algorithm to find inverse tangent
    /*!
      \brief check for protective field breach
             or right side of warning field breach
      \param[in] y oppsite side length
      \param[in] x adjacent side length
      \return sint16_t degree inverse tangent of input
    */
    sint16_t CordicATan(sint32_t y, sint32_t x);

    CNavigation& m_NAV;                     /*!< Navigation app object reference */
    CPLSComms&   m_plsComms;                /*!< PLS communication object reference */
    bool m_driveState;                      /*!< Vehicle drivinng status, true for in motion*/
    bool m_dataRequested;                   /*!< Data request send status*/
};

#endif /*APP_V_MAPPING_H*/
