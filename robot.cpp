#include <propeller.h>
#include "simpletools.h"
#include "abdrive.h"
#include "robot.h"
#include "stream.h"

#define DEBUG

Robot::Robot()
  : m_wheelLeft(0), m_wheelRight(0)
{
  high(26);
  setSpeed(m_wheelLeft, m_wheelRight);
}

Robot::~Robot()
{
  setSpeed(0, 0);
}

bool Robot::processButton(int whichWay)            
{ 
  toggle(26);
      
  switch (whichWay) {
  
  case 'F': // forward
    #ifdef DEBUG
      Serial.println("Forward");
    #endif
    if (m_wheelLeft > m_wheelRight)
      m_wheelRight = m_wheelLeft;
    else if (m_wheelLeft < m_wheelRight) 
      m_wheelLeft = m_wheelRight;
    else {           
      m_wheelLeft = m_wheelLeft + 32;
      m_wheelRight = m_wheelRight + 32;
    }      
    break;    
    
  case 'R': // right
    #ifdef DEBUG
      Serial.println("Right");
    #endif
    m_wheelLeft = m_wheelLeft + 16;
    m_wheelRight = m_wheelRight - 16;
    break;
    
  case 'L': // left
    #ifdef DEBUG
      Serial.println("Left");
    #endif
    m_wheelLeft = m_wheelLeft - 16;
    m_wheelRight = m_wheelRight + 16;
    break;
    
  case 'B': // reverse
    #ifdef DEBUG
      Serial.println("Reverse");
    #endif
    if(m_wheelLeft < m_wheelRight)
      m_wheelRight = m_wheelLeft;
    else if (m_wheelLeft > m_wheelRight) 
      m_wheelLeft = m_wheelRight;
    else {           
      m_wheelLeft = m_wheelLeft - 32;
      m_wheelRight = m_wheelRight - 32;
    }
    break;  
        
  case 'S': // stop
    #ifdef DEBUG
      Serial.println("Stop");
    #endif
    m_wheelLeft = 0;
    m_wheelRight = 0;
    break;
    
  default:  // unknown request
    return false;
  }    
  
  if (m_wheelLeft > 128) m_wheelLeft = 128;
  if (m_wheelLeft < -128) m_wheelLeft = -128;
  if (m_wheelRight > 128) m_wheelRight = 128;
  if (m_wheelRight < -128) m_wheelRight = -128;
  
  setSpeed(m_wheelLeft, m_wheelRight);
    
  return true;
}

void Robot::setSpeed(int left, int right)
{  
  #ifdef DEBUG
    Serial.print("L ");
    Serial.print(m_wheelLeft);
    Serial.print(", R ");
    Serial.println(m_wheelRight);
  #endif
  
  m_wheelLeft = left;
  m_wheelRight = right;
  drive_speed(m_wheelLeft, m_wheelRight);
}
