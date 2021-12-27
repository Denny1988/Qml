/**
 * Title:        TASDK
 * Description:  Technical Analysis Library
 * Copyright:    Copyright (c) 1999 to 2004
 * Company:      Modulus Financial Engineering
 * @author		 Modulus FE
 * @version		 5.01
 */

#include "TASDK.h"

CField::CField(int RecordCount, const char* Name){  
  TASDK1 = new TASDK();
  m_Name = Name;
  m_RecordCount = RecordCount;
  m_Note = new CNote();
  strpData.resize(RecordCount + 2);
  dblpData.resize(RecordCount + 2);
}

CField::~CField(){
	delete m_Note;
	delete TASDK1;
	strpData.clear();
	dblpData.clear();	
}

  void CField::setNote(CNote* NewNote){
    m_Note = NewNote;
  }

  CNote* CField::getNote(){
    return m_Note;
  }

  int CField::getRecordCount(){
    return m_RecordCount;
  }

  void CField::setName(const char* NewFieldName){
    m_Name = NewFieldName;
  }

  const char* CField::getName(){
    return m_Name;
  }

  void CField::setValue(int RowIndex, double Value){

    try{
	  dblpData[RowIndex] = Value;
    }
    catch(...){
    }

  }

  double CField::getValue(int RowIndex){

    try{
		double ret = dblpData[RowIndex];
		if(ret > 1E+100 || ret < -1E+100) ret = NULL_VALUE;
		return ret;
    }
    catch(...){
      return -1;
    }

  }


  void CField::setStrValue(int RowIndex, const char* Value){

    try{
	  strpData[RowIndex] = Value;
    }
    catch(...){

    }

  }

  const char* CField::getStrValue(int RowIndex) {

    try{
	  return strpData[RowIndex];
    }
    catch(...){
      return "";
    }

  }

 