#include "App_Navigation.h"
#include <Arduino.h>

CNavigation::CNavigation(CICCComms& ICC): m_ICC(ICC)
{
  //do nothing
}
CNavigation::~CNavigation() {
  // set default values
  rotationDone = false;
  distanceDone = false;
}

void CNavigation::Init(void)
{
    continueDrive();

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
  current_state = next_state;
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
  return false;
}

// Function to set the current value of offset position to wall,
// current angle of the wall and current distance to wall in front to the input buffer
void CNavigation::setPLSdata(uint16_t offset, int8_t angle, uint16_t nxt_wall)
{
  // Set input values
  buf_offset = offset;
  buf_angle = angle;
  buf_nxt_wall = nxt_wall;
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
        break;

      case STATE_GET_OFFSET:
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
        if (rotationDone)
        {
          next_state = STATE_COR_OFFSET;
        }
        break;

      case STATE_COR_OFFSET:
        if (distanceDone)
        {
          next_state = STATE_ROT_WALL_OFFSET;
        }
        break;


      case STATE_ROT_WALL_OFFSET:
        if (rotationDone)
        {
          next_state = STATE_DRIVE_WALL;
        }
        break;


      case STATE_GET_ANGLE:
        if (rotationDone)
        {
          next_state = STATE_DRIVE_WALL;
        }
        break;


      case STATE_DRIVE_WALL:
        if (abs(cur_angle) > NAV_TOL_ANGLE_DRIVE)
        {
          next_state = STATE_GET_ANGLE;
        }
        break;


      default:
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
  DPRINTLN("-----------------------------");
  DPRINTLN("App_Navigation:\n");

  // Print current and next state
  DPRINT("Current state: ");
  printState(current_state);
  DPRINT("\t");
  DPRINT("Next state: ");
  printState(next_state);
  DPRINTLN("");

  // Print environmatal parameters
  DPRINT("Offset: ");
  DPRINT(cur_offset);
  DPRINT(" (current)\t");
  DPRINT(buf_offset);
  DPRINTLN(" (buffer)");

  DPRINT("Angle: ");
  DPRINT(cur_angle);
  DPRINT(" (current)\t");
  DPRINT(buf_angle);
  DPRINTLN(" (buffer)");

  DPRINT("Next Wall: ");
  DPRINT(cur_nxt_wall);
  DPRINT(" (current)\t");
  DPRINT(buf_nxt_wall);
  DPRINTLN(" (buffer)");

  DPRINTLN("-----------------------------");
}

// Function to print state
void CNavigation::printState(NAV_STATE state)
{
  switch (state)
  {
    case STATE_IDLE:
      DPRINT("STATE_IDLE");
      break;

    case STATE_GET_OFFSET:
      DPRINT("STATE_GET_OFFSET");
      break;

    case STATE_ROT_WALL_INFRONT:
      DPRINT("STATE_ROT_WALL_INFRONT");
      break;

    case STATE_COR_OFFSET:
      DPRINT("STATE_COR_OFFSET");
      break;

    case STATE_ROT_WALL_OFFSET:
      DPRINT("STATE_ROT_WALL_OFFSET");
      break;

    case STATE_GET_ANGLE:
      DPRINT("STATE_GET_ANGLE");
      break;

    case STATE_ROT_WALL:
      DPRINT("STATE_ROT_WALL");
      break;

    case STATE_DRIVE_WALL:
      DPRINT("STATE_DRIVE_WALL");
      break;

    default:
      DPRINT("unknown state (");
      DPRINT(state);
      DPRINT(")");
      break;
  }
}


// Function to contine if obstacle is clear
void CNavigation::continueDrive(void)
{
  runFlow = true;
  DPRINTLN("Nav: Continue drive!");
}


// Function to stop if obstacle is detected
void CNavigation::stopDrive(void)
{
  runFlow = false;
  DPRINTLN("Nav: Pause drive!");
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
        DPRINTLN("Nav: Offset is okay.");
        DPRINT("Nav: Rotate parallel to wall by "); DPRINT(-cur_angle); DPRINTLN(" deg");
      } else if (next == STATE_ROT_WALL_INFRONT)
      {
        DPRINTLN("Nav: Offset has to be corrected...");
        DPRINT("Nav: Rotate to wall by "); DPRINT(-cur_angle + 90); DPRINTLN(" deg");
      }
      break;

    case STATE_ROT_WALL_INFRONT:
      if (next == STATE_COR_OFFSET)
      {
        DPRINTLN("Nav: Rotation done. Correct offset...");
        DPRINT("Nav: Drive "); DPRINT(cur_nxt_wall - NAV_SET_OFFSET); DPRINTLN(" cm");
      }
      break;

    case STATE_COR_OFFSET:
      if ( next == STATE_ROT_WALL_OFFSET)
      {
        DPRINTLN("Nav: Offset corrected...");
        DPRINTLN("Nav: Rotate left 90 degrees");
      }
      break;

    case STATE_ROT_WALL_OFFSET:
      if (next == STATE_DRIVE_WALL)
      {
        DPRINTLN("Nav: Drive forward");
      }

      break;

    case STATE_GET_ANGLE:
      if (next == STATE_DRIVE_WALL)
      {
        DPRINTLN("Nav: Drive forward");
      }
      break;

    case STATE_ROT_WALL:
      break;

    case STATE_DRIVE_WALL:
      if (next == STATE_GET_ANGLE)
      {
        DPRINTLN("Nav: Angle out of limit! Stop drive!");
        DPRINT("Nav: Rotate parallel to wall by "); DPRINT(-cur_angle); DPRINTLN(" deg");
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
  m_ICC.addTxMsg(ICC_CMD_PAUSE_DRIVE, 0);
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

