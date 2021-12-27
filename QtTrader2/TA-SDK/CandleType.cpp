/**
 * Title:        TASDK
 * Description:  Technical Analysis Library
 * Copyright:    Copyright (c) 1999 to 2005
 * Company:      Modulus Financial Engineering
 * @author		 Modulus FE
 * @version		 6.0
 */

#include "TASDK.h"

CandleType::CandleType(char* Name, double Range, int BullBear)
{
    m_Name = Name;
    m_Range = Range;
    m_BullBear = BullBear;
}

CandleType::CandleType()
{

}

CandleType::~CandleType()
{

}

  char* CandleType::getName(){
    return m_Name;
  }

  double CandleType::getRange(){
    return m_Range;
  }

  double CandleType::getBullBear(){
    return m_BullBear;
  }

  void CandleType::setName(char* Name){
    m_Name = Name;
  }

  void CandleType::setRange(double Range){
    m_Range = Range;
  }

  void CandleType::setBullBear(int BullBear){
    m_BullBear = BullBear;
  }