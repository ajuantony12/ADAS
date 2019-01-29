/*H**********************************************************************
  FILENAME :        Navigation.h

  DESCRIPTION : Navigation module
*************************************************************************/
#include "Arduino.h"
#include "Task_if.h"
#include "ADAS_Types.h"
#include "ADAS_Cfg.h"
#include "ADAS_Debug.h"
#include "Comm_ICC.h"

#ifndef APP_NAVIGATION_H
#define APP_NAVIGATION_H

class CNavigation: public ITask_IF
{
  public:
    CNavigation(CICCComms& ICC);
    ~CNavigation();

    // States of navigation flow
    enum NAV_STATE {
      // Idle state
      STATE_IDLE,
      // Get offset to the side
      STATE_GET_OFFSET,
      // Rotate right until wall is 90 degrees to drive direction
      STATE_ROT_WALL_INFRONT,
      // Correct offset
      STATE_COR_OFFSET,
      // Rotate left 90 degrees to be parallel to wall after offset correction
      STATE_ROT_WALL_OFFSET,
      // Get wall angle
      STATE_GET_ANGLE,
      // Rotate parallel to wall
      STATE_ROT_WALL,
      // Drive along wall
      STATE_DRIVE_WALL,
    };

    /*
       init
    */
    virtual void Init(void);
    /*
       Run
    */
    virtual void Run(void);
    /*
       Stop
    */
    virtual void Stop(void);

    /*
       Set current offset, angle and next wall position
    */
    virtual void setPLSdata(uint16_t offset, int8_t angle, uint16_t nxt_wall);

    /*
       Continue drive after obstacle detection
    */
    virtual void contDrive(void);

    /*
       Pause drive due to obstacle detection
    */
    virtual void pauseDrive(void);


    /*
       Function to get current state
    */
    virtual NAV_STATE getCurrentState(void);

    /*
       Function to get next state
    */
    virtual NAV_STATE getNextState(void);


    /*
       Parameter to show that rotation is finished
    */
    volatile bool rotationDone;

    /*
       Paramter to show that distance is reached
    */
    volatile bool distanceDone;

    /*
       Debug function to print current status of state flow if changed
    */
    virtual void printChangedDebugInfo(void);

    /*
       Debug function to print current status of state flow
    */
    virtual void printDebugInfo(void);

    void setRotationDone(void);


    /*
       Function to stop if obstacle is detected
    */
    virtual void stopDrive(void);



    /*
       Function to contine if obstacle is clear
    */
    virtual void continueDrive(void);

    bool isCornerMode();




  private:
  
 


    // Parameter to pause/run state flow
    bool runFlow = false;
    bool cornerMode;

    bool newPLSdata;


    // Variables for the state machine
    NAV_STATE current_state = STATE_IDLE;
    NAV_STATE next_state = current_state;

    // Buffer of environmental data
    uint16_t buf_offset;
    int8_t buf_angle;
    uint16_t buf_nxt_wall;
    uint16_t cur_offset;
    int8_t cur_angle;
    uint16_t cur_nxt_wall;

    // Function to set the environmental data buffer to state data
    void getPLSBuf(void);

    // Function to set next state
    void getNextState(bool runActive);

    // Function to perfom transisition actions
    void doTransistionAction(NAV_STATE state, NAV_STATE next);

    // Function to print state
    void printState(NAV_STATE state);


    // Variables for old value for debug print
    NAV_STATE current_state_old;
    NAV_STATE next_state_old;
    uint16_t buf_offset_old;
    int8_t buf_angle_old;
    uint16_t buf_nxt_wall_old;
    uint16_t cur_offset_old;
    int8_t cur_angle_old;
    uint16_t cur_nxt_wall_old;

    CICCComms& m_ICC;

};

#endif /*APP_NAVIGATION_H*/

