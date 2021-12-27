/**
 * Title:        TASDK
 * Description:  Technical Analysis Library
 * Copyright:    Copyright (c) 1999 to 2004
 * Company:      Modulus Financial Engineering
 * @author 		 Modulus FE
 * @version 	 6.0
 */
#pragma warning (disable : 4275)
#include "TASDK.h"
//#include <windows.h>
#include <fstream>
#include <sstream>
using namespace std;

template<class T>
T FromString(const std::string s ){
  T t;
  std::istringstream iss(s);
  iss >> t;
  return t;
}

CDatabase::CDatabase() : m_Index(-1), m_RecordCount(-1), m_Recordset(nullptr)
{
}

CDatabase::~CDatabase()
{
	if (m_Recordset)
	{
		delete m_Recordset;
	}
}

  CRecordset* CDatabase::getRecordset(){
	  return m_Recordset;
  }

  int CDatabase::getRecordCount(){
    return m_RecordCount;
  }

  void CDatabase::setPosition(int Index){
    if (Index > 0){
        m_Index = Index;
    }
  }

  int CDatabase::getPosition(){
    return m_Index;
  }

  void CDatabase::MoveNext(){
    setPosition(getPosition() + 1);
  }

  void CDatabase::MovePrevious(){
    setPosition(getPosition() - 1);
  }

  void CDatabase::MoveFirst(){
    setPosition(1);
  }

  void CDatabase::MoveLast(){
	  setPosition(getRecordCount());
  }

  void CDatabase::setRecordset(CRecordset* recordset, int recordCount)
  {
	  m_Recordset = recordset;
	  m_RecordCount = recordCount;
	  if (m_RecordCount > 0)
	  {
		  setPosition(1);
	  }
  }

  void CDatabase::OpenDatabase(char* fileName, char* delimiter){

    //Requires 6 fields: Date, Open, High, Low, Close, Volume

	//!!!
    // Replace this function with you're own data function, 
	// if you wish to load from SQL, XML, etc.

	double d = 0;
	int found = 0;
	int index = 0;
	int recordCount = 0;
	std::string record;
	std::string temp;
	char* psz;

    m_RecordCount = 0;

    m_Recordset = new CRecordset();

	try{
		//Get record count
		ifstream rec(fileName);
		if(rec.is_open()){
			//Loop through file
			while (getline(rec, record)){
				recordCount++;
			}
			rec.close();	
			m_RecordCount = recordCount;
		}
	}
	catch(...){
		//SetLastError((DWORD) 0x10001);
	}

    m_Date = new CField(m_RecordCount,"Date");
    m_Open = new CField(m_RecordCount,"Open");
    m_High = new CField(m_RecordCount,"High");
    m_Low = new CField(m_RecordCount,"Low");
    m_Close = new CField(m_RecordCount,"Close");
    m_Volume = new CField(m_RecordCount,"Volume");

    try{

		//Read file
		ifstream in(fileName);
		if(!in)
		{
			//SetLastError((DWORD) 0x10001);
			//std::cout << "General file IO error reading " << fileName << endl;
		}
		else{
			//Loop through file
			while (getline(in, record)){
				
				index++;
				
				//Parse record
				found = record.find(delimiter);
				if(found != 0){
					temp = record.substr(0, found).c_str();
					psz = new char[record.size() + 1];
					strcpy(psz, temp.c_str());
					m_Date->setStrValue(index, psz);
					record = record.substr(found + 1);
				}
				found = record.find(delimiter);
				if(found != 0){
					m_Open->setValue(index, FromString<double>(record.substr(0, found)));
					record = record.substr(found + 1);
				}
				found = record.find(delimiter);
				if(found != 0){
					m_High->setValue(index, FromString<double>(record.substr(0, found)));
					record = record.substr(found + 1);
				}
				found = record.find(delimiter);
				if(found != 0){
					m_Low->setValue(index, FromString<double>(record.substr(0, found)));
					record = record.substr(found + 1);
				}
				found = record.find(delimiter);
				if(found != 0){
					m_Close->setValue(index, FromString<double>(record.substr(0, found)));
					record = record.substr(found + 1);
				}
				found = record.find(delimiter);
				if(found != 0){
					m_Volume->setValue(index, FromString<double>(record.substr(0, found)));
					record = record.substr(found + 1);
				}

			}
			in.close();
		}
    }

    catch(...){
		//SetLastError((DWORD) 0x10001);
	}

    setPosition(1);

	m_Recordset = new CRecordset();
    m_Recordset->addField(m_Date);
	m_Recordset->addField(m_Open);
    m_Recordset->addField(m_High);
    m_Recordset->addField(m_Low);
    m_Recordset->addField(m_Close);
    m_Recordset->addField(m_Volume);

	//Ignore error #87
//	if(GetLastError() == 87){
//		//SetLastError(0);
//	}

  }

  void CDatabase::CloseDatabase(){
    m_RecordCount = -1;
    m_Index = -1;
  }
