#include "App_Navigation.h"
#include <Arduino.h>


#define PIN_BTN 12 // test only

CNavigation::CNavigation(CICCComms& ICC): m_ICC(ICC)
{
  //do nothing
}
CNavigation::~CNavigation() {
  // set default values
  rotationDone = false;
  distanceDone = false;
  cornerMode = false;
  newPLSdata = false;
}

void CNavigation::Init(void)
{

}

void CNavigation::Run(void)
{    
  // Process ICC
  m_ICC.Run();

  // Set current environemtal data to state machine data
  getPLSBuf();

  // Call next state logic
  getNextState(runFlow);

  // Call transistion acitions
  doTransistionAction(current_state, next_state);

  // Set next state to current state
  if(newPLSdata || cornerMode)
  {
      newPLSdata = false;
      current_state = next_state; 
  }

}

void CNavigation::Stop(void)
{
  m_ICC.addTxMsg(ICC_CMD_PAUSE_DRIVE, 0);
  current_state = STATE_IDLE;
  next_state = STATE_IDLE;
  stopDrive();

}

bool CNavigation::isCornerMode()
{
  return cornerMode;
}

// Function to set the current value of offset position to wall,
// current angle of the wall and current distance to wall in front to the input buffer
void CNavigation::setPLSdata(uint16_t offset, int8_t angle, uint16_t nxt_wall)
{
  // Set input values
  buf_offset = offset;
  buf_angle = angle;
  buf_nxt_wall = nxt_wall;
  
  newPLSdata = true;
}


// Next state logic
void CNavigation::getNextState(bool runActive)
{
  if (runActive)
  {
    // running navigation run
    switch (current_state) {

      case STATE_IDLE:
        next_state = STATE_GET_OFFSET;
        cornerMode = false;
        break;

      case STATE_GET_OFFSET:
        cornerMode = false;
        // Decide if offset is okay or has to be corrected
        if ((cur_offset < NAV_SET_OFFSET + NAV_TOL_OFFSET) && (cur_offset > NAV_SET_OFFSET - NAV_TOL_OFFSET))
        { // Offset is in tolerance
          next_state = STATE_GET_ANGLE;
        } else {
          // Offset has to be corrected
          next_state = STATE_ROT_WALL_INFRONT;
        }
        break;


      case STATE_ROT_WALL_INFRONT:
        cornerMode = true;
        if (rotationDone)
        {
          next_state = STATE_COR_OFFSET;
          rotationDone = false;
        }
        break;

      case STATE_COR_OFFSET:
        cornerMode = true;
        if (distanceDone)
        {
          next_state = STATE_ROT_WALL_OFFSET;
          distanceDone = false;
        }
        break;


      case STATE_ROT_WALL_OFFSET:
        cornerMode = true;
        if (rotationDone)
        {
          next_state = STATE_DRIVE_WALL;
          rotationDone = false;
        }
        break;


      case STATE_GET_ANGLE:
        if (rotationDone)
        {
          cornerMode = false;
          next_state = STATE_DRIVE_WALL;
          rotationDone = false;
        }
        break;


      case STATE_DRIVE_WALL:
         cornerMode = false;
        if (abs(cur_angle) > NAV_TOL_ANGLE_DRIVE)
        {
          next_state = STATE_GET_ANGLE;
        }
        break;


      default:
        cornerMode = false;
        next_state = STATE_IDLE;
        break;
    }
  } else {
    // paused navigation run

  }
}


// Function to read environmetal data buffer to state machine variables
void CNavigation::getPLSBuf(void)
{
  cur_offset = buf_offset;
  cur_angle = buf_angle;
  cur_nxt_wall = buf_nxt_wall;
}

// Debug function to print current status of state flow if changed
void CNavigation::printChangedDebugInfo(void)
{


  if ((current_state_old != current_state) ||
      (next_state_old != next_state)       ||
      (buf_offset_old != buf_offset)       ||
      (buf_angle_old != buf_angle)         ||
      (buf_nxt_wall_old != buf_nxt_wall)   ||
      (cur_offset_old != cur_offset)       ||
      (cur_angle_old != cur_angle)         ||
      (cur_nxt_wall_old != cur_nxt_wall))
  {
    printDebugInfo();
  }

  // Write current value to old
  current_state_old = current_state;
  next_state_old = next_state;
  buf_offset_old = buf_offset;
  buf_angle_old = buf_angle;
  buf_nxt_wall_old = buf_nxt_wall;
  cur_offset_old = cur_offset;
  cur_angle_old = cur_angle;
  cur_nxt_wall_old = cur_nxt_wall;
}


// Debug function to print current status of state flow
void CNavigation::printDebugInfo(void)
{
  DPRINTLN2("-----------------------------");
  DPRINTLN2("App_Navigation:\n");

  // Print current and next state
  DPRINT2("Current state: ");
  printState(current_state);
  DPRINT2("\t");
  DPRINT2("Next state: ");
  printState(next_state);
  DPRINTLN2("");

  // Print environmatal parameters
  DPRINT2("Offset: ");
  DPRINT2(cur_offset);
  DPRINT2(" (current)\t");
  DPRINT2(buf_offset);
  DPRINTLN2(" (buffer)");

  DPRINT2("Angle: ");
  DPRINT2(cur_angle);
  DPRINT2(" (current)\t");
  DPRINT2(buf_angle);
  DPRINTLN2(" (buffer)");

  DPRINT2("Next Wall: ");
  DPRINT2(cur_nxt_wall);
  DPRINT2(" (current)\t");
  DPRINT2(buf_nxt_wall);
  DPRINTLN2(" (buffer)");


  DPRINT2("newPLSdata: ");
  DPRINT2(newPLSdata);
  DPRINTLN2("");

  DPRINTLN2("-----------------------------");
}

// Function to print state
void CNavigation::printState(NAV_STATE state)
{
  switch (state)
  {
    case STATE_IDLE:
      DPRINT2("STATE_IDLE");
      break;

    case STATE_GET_OFFSET:
      DPRINT2("STATE_GET_OFFSET");
      break;

    case STATE_ROT_WALL_INFRONT:
      DPRINT2("STATE_ROT_WALL_INFRONT");
      break;

    case STATE_COR_OFFSET:
      DPRINT2("STATE_COR_OFFSET");
      break;

    case STATE_ROT_WALL_OFFSET:
      DPRINT2("STATE_ROT_WALL_OFFSET");
      break;

    case STATE_GET_ANGLE:
      DPRINT2("STATE_GET_ANGLE");
      break;

    case STATE_ROT_WALL:
      DPRINT2("STATE_ROT_WALL");
      break;

    case STATE_DRIVE_WALL:
      DPRINT2("STATE_DRIVE_WALL");
      break;

    default:
      DPRINT2("unknown state (");
      DPRINT2(state);
      DPRINT2(")");
      break;
  }
}


// Function to contine if obstacle is clear
void CNavigation::continueDrive(void)
{
  if (!runFlow)
  {
      m_ICC.addTxMsg(ICC_CMD_CONT_DRIVE, 0);
      runFlow = true;
      DPRINTLN2("Nav: Continue drive!");
  }
  

}


// Function to stop if obstacle is detected
void CNavigation::stopDrive(void)
{
  runFlow = false;
  DPRINTLN2("Nav: Pause drive!");
}


// Function to perfom transisition actions
void CNavigation::doTransistionAction(NAV_STATE state, NAV_STATE next)
{
  switch (state)
  {
    case STATE_IDLE:

      break;

    case STATE_GET_OFFSET:
      if (next == STATE_GET_ANGLE) {
        DPRINTLN2("Nav: Offset is okay.");
        DPRINT2("Nav: Rotate parallel to wall by "); DPRINT2(-cur_angle); DPRINTLN2(" deg");
        m_ICC.addTxMsg(ICC_CMD_ROT_ANGLE, -cur_angle);

      } else if (next == STATE_ROT_WALL_INFRONT)
      {
        DPRINTLN2("Nav: Offset has to be corrected...");
        DPRINT2("Nav: Rotate to wall by "); DPRINT2(-cur_angle + 90); DPRINTLN2(" deg");
        m_ICC.addTxMsg(ICC_CMD_ROT_ANGLE, -cur_angle + 90);
      }
      break;

    case STATE_ROT_WALL_INFRONT:
      if (next == STATE_COR_OFFSET)
      {
        DPRINTLN2("Nav: Rotation done. Correct offset...");
        DPRINT2("Nav: Drive "); DPRINT2(static_cast<sint16_t>(cur_nxt_wall) - static_cast<sint16_t>(NAV_SET_OFFSET)); DPRINTLN2(" cm");
        m_ICC.addTxMsg(ICC_CMD_DRIVE_DIST, static_cast<sint16_t>(cur_nxt_wall) - static_cast<sint16_t>(NAV_SET_OFFSET));
      }
      break;

    case STATE_COR_OFFSET:
      if ( next == STATE_ROT_WALL_OFFSET)
      {
        DPRINTLN2("Nav: Offset corrected...");
        DPRINTLN2("Nav: Rotate left 90 degrees");
        m_ICC.addTxMsg(ICC_CMD_ROT_ANGLE, -90);
      }
      break;

    case STATE_ROT_WALL_OFFSET:
      if (next == STATE_DRIVE_WALL)
      {
        DPRINTLN2("Nav: Drive forward");
        m_ICC.addTxMsg(ICC_CMD_CONT_DRIVE_IN, 1);
      }

      break;

    case STATE_GET_ANGLE:
      if (next == STATE_DRIVE_WALL)
      {
        DPRINTLN2("Nav: Drive forward");
        m_ICC.addTxMsg(ICC_CMD_CONT_DRIVE_IN, 1);
      }
      break;

    case STATE_ROT_WALL:
      break;

    case STATE_DRIVE_WALL:
      if (next == STATE_GET_ANGLE)
      {
        DPRINTLN2("Nav: Angle out of limit! Stop drive!");
        DPRINT2("Nav: Rotate parallel to wall by "); DPRINT2(-cur_angle); DPRINTLN2(" deg");
        m_ICC.addTxMsg(ICC_CMD_ROT_ANGLE, -cur_angle);
        cornerMode = true;
      }
      break;

  }
}


// Continue drive after obstacle detection
void CNavigation::contDrive(void)
{
  // start position correction
  current_state = STATE_GET_OFFSET;
  next_state = STATE_GET_OFFSET;
}


// Pause drive due to obstacle detection
void CNavigation::pauseDrive(void)
{
    if(runFlow)
    { 
        runFlow = false;
        m_ICC.addTxMsg(ICC_CMD_PAUSE_DRIVE, 0);
        DPRINTLN2("Nav: Pause drive!");
    }

}


// Function to get current state
CNavigation::NAV_STATE CNavigation::getCurrentState(void)
{
  return current_state;
}

// Function to get next state
CNavigation::NAV_STATE CNavigation::getNextState(void)
{
  return next_state;
}


void CNavigation::setRotationDone(void)
{
  DPRINTLN("Rotation done received");
  rotationDone = true;
}

