/**
 * Title:        TASDK
 * Description:  Technical Analysis Library
 * Copyright:    Copyright (c) 1999 to 2005
 * Company:      Modulus Financial Engineering
 * @author		 Modulus FE
 * @version		 6
 */
  
#include "TASDK.h"
//#include <iostream.h>

CRecordset::CRecordset()
{
	FieldCount = 0;
}

CRecordset::~CRecordset()
{	
	for(int n = 0; n != FieldData.size(); ++n){
		try{
			if(NULL != FieldData[n]){
				delete FieldData[n];
			}
		}
		catch(...){}
	}
	FieldData.clear();
}

  void CRecordset::setDatabase(CDatabase* NewDatabase){
	  m_CDatabase = NewDatabase;
  }

  CDatabase* CRecordset::getDatabase(){
	  return m_CDatabase;
  }

  void CRecordset::addField(CField* NewField){
      FieldCount += 1;
	  FieldData.resize(FieldCount + 1);
      FieldData[FieldCount] = NewField;
  }

  void CRecordset::renameField(const char* OldFieldName, const char* NewFieldName){

    int FieldIndex;

    try{
      FieldIndex = getIndex(OldFieldName);
      FieldData[FieldIndex]->setName(NewFieldName);
    }
    catch(...){

    }

  }

  void CRecordset::removeField(const char* FieldName){

    int n = 0;
    std::vector<CField*> NewField(FieldCount);
    for (int i = 1; i < FieldCount + 1; i ++){
      if (strcmp(FieldData[i]->getName(), FieldName) != 0){
        n = n + 1;
        NewField[n] = FieldData[i];
      }	  
    }

    FieldData = NewField;
    FieldCount -= 1;

  }

  double CRecordset::getValue(const char* FieldName, int RowIndex){

    try{
      return FieldData[getIndex(FieldName)]->getValue(RowIndex);
    }

    catch(...){
      return -1;
    }

  }

  void CRecordset::setValue(const char* FieldName, int RowIndex, double Value){

    try{
      FieldData[getIndex(FieldName)]->setValue(RowIndex, Value);
    }

    catch(...){

    }

  }

  CField* CRecordset::getField(const char* FieldName){

    try{
      return FieldData[getIndex(FieldName)];
    }

    catch(...){
	   CField *e = new CField(0,"");
       return e;
    }

  }

  int CRecordset::getIndex(const char* FieldName){

    int FieldIndex;
    bool found = false;

    for (FieldIndex = 1; FieldIndex < FieldCount + 1; FieldIndex++){		
        if (strcmp(FieldData[FieldIndex]->getName(), FieldName) == 0){
            found = true;
            break;
        }
    }

    if (!found){
      return -1;
    }

    else{
        return FieldIndex;
    }

  }

  const char* CRecordset::getName(int FieldIndex){

    try{
      return FieldData[FieldIndex]->getName();
    }

   catch(...){
    return "";
   }

  }

  int CRecordset::getFieldCount(){
    return FieldCount;
  }

  bool CRecordset::isValidField(const char* FieldName){
    bool retval = false;
    for (int FieldIndex = 1; FieldIndex < FieldCount + 1; FieldIndex ++){
        if (strcmp(FieldData[FieldIndex]->getName(), FieldName) == 0){
            retval = true;
            break;
        }
    }
    return retval;
  }
