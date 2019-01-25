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
  cornerMode = false;
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
          rotationDone = false;
        }
        break;

      case STATE_COR_OFFSET:
        if (distanceDone)
        {
          next_state = STATE_ROT_WALL_OFFSET;
          distanceDone = false;
        }
        break;


      case STATE_ROT_WALL_OFFSET:
        if (rotationDone)
        {
          next_state = STATE_DRIVE_WALL;
          rotationDone = false;
        }
        break;


      case STATE_GET_ANGLE:
        if (rotationDone)
        {
          next_state = STATE_DRIVE_WALL;
          rotationDone = false;
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
  Serial.println("-----------------------------");
  Serial.println("App_Navigation:\n");

  // Print current and next state
  Serial.print("Current state: ");
  printState(current_state);
  Serial.print("\t");
  Serial.print("Next state: ");
  printState(next_state);
  Serial.println("");

  // Print environmatal parameters
  Serial.print("Offset: ");
  Serial.print(cur_offset);
  Serial.print(" (current)\t");
  Serial.print(buf_offset);
  Serial.println(" (buffer)");

  Serial.print("Angle: ");
  Serial.print(cur_angle);
  Serial.print(" (current)\t");
  Serial.print(buf_angle);
  Serial.println(" (buffer)");

  Serial.print("Next Wall: ");
  Serial.print(cur_nxt_wall);
  Serial.print(" (current)\t");
  Serial.print(buf_nxt_wall);
  Serial.println(" (buffer)");


  Serial.print("rotationDone: ");
  Serial.print(rotationDone);
  Serial.print("\tdistanceDone: ");
  Serial.print(distanceDone);
  Serial.println("");

  Serial.println("-----------------------------");
}

// Function to print state
void CNavigation::printState(NAV_STATE state)
{
  switch (state)
  {
    case STATE_IDLE:
      Serial.print("STATE_IDLE");
      break;

    case STATE_GET_OFFSET:
      Serial.print("STATE_GET_OFFSET");
      break;

    case STATE_ROT_WALL_INFRONT:
      Serial.print("STATE_ROT_WALL_INFRONT");
      break;

    case STATE_COR_OFFSET:
      Serial.print("STATE_COR_OFFSET");
      break;

    case STATE_ROT_WALL_OFFSET:
      Serial.print("STATE_ROT_WALL_OFFSET");
      break;

    case STATE_GET_ANGLE:
      Serial.print("STATE_GET_ANGLE");
      break;

    case STATE_ROT_WALL:
      Serial.print("STATE_ROT_WALL");
      break;

    case STATE_DRIVE_WALL:
      Serial.print("STATE_DRIVE_WALL");
      break;

    default:
      Serial.print("unknown state (");
      Serial.print(state);
      Serial.print(")");
      break;
  }
}


// Function to contine if obstacle is clear
void CNavigation::continueDrive(void)
{
  runFlow = true;
  Serial.println("Nav: Continue drive!");
}


// Function to stop if obstacle is detected
void CNavigation::stopDrive(void)
{
  runFlow = false;
  Serial.println("Nav: Pause drive!");
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
        Serial.println("Nav: Offset is okay.");
        Serial.print("Nav: Rotate parallel to wall by "); Serial.print(-cur_angle); Serial.println(" deg");
        cornerMode = false;
        m_ICC.addTxMsg(ICC_CMD_ROT_ANGLE, -cur_angle);

      } else if (next == STATE_ROT_WALL_INFRONT)
      {
        Serial.println("Nav: Offset has to be corrected...");
        cornerMode = true;
        Serial.print("Nav: Rotate to wall by "); Serial.print(-cur_angle + 90); Serial.println(" deg");
        m_ICC.addTxMsg(ICC_CMD_ROT_ANGLE, -cur_angle + 90);
      }
      break;

    case STATE_ROT_WALL_INFRONT:
      if (next == STATE_COR_OFFSET)
      {
        Serial.println("Nav: Rotation done. Correct offset...");
        cornerMode = false;
        Serial.print("Nav: Drive "); Serial.print(cur_nxt_wall - NAV_SET_OFFSET); Serial.println(" cm");
        m_ICC.addTxMsg(ICC_CMD_DRIVE_DIST, cur_nxt_wall - NAV_SET_OFFSET);
      }
      break;

    case STATE_COR_OFFSET:
      if ( next == STATE_ROT_WALL_OFFSET)
      {
        Serial.println("Nav: Offset corrected...");
        Serial.println("Nav: Rotate left 90 degrees");
        cornerMode = true;
        m_ICC.addTxMsg(ICC_CMD_ROT_ANGLE, -90);
      }
      break;

    case STATE_ROT_WALL_OFFSET:
      if (next == STATE_DRIVE_WALL)
      {
        Serial.println("Nav: Drive forward");
        cornerMode = false;
        m_ICC.addTxMsg(ICC_CMD_CONT_DRIVE_IN, 1);
      }

      break;

    case STATE_GET_ANGLE:
      if (next == STATE_DRIVE_WALL)
      {
        Serial.println("Nav: Drive forward");
        cornerMode = false;
        m_ICC.addTxMsg(ICC_CMD_CONT_DRIVE_IN, 1);
      }
      break;

    case STATE_ROT_WALL:
      break;

    case STATE_DRIVE_WALL:
      if (next == STATE_GET_ANGLE)
      {
        Serial.println("Nav: Angle out of limit! Stop drive!");
        cornerMode = false;
        Serial.print("Nav: Rotate parallel to wall by "); Serial.print(-cur_angle); Serial.println(" deg");
        m_ICC.addTxMsg(ICC_CMD_ROT_ANGLE, -cur_angle);
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


void CNavigation::setRotationDone(void)
{
  DPRINTLN("Rotation done received");
  rotationDone = true;
}

