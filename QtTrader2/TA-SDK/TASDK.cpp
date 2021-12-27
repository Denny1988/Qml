// TASDK.cpp : Defines the entry point for the DLL application.
//

#include "TASDK.h"
#include <string>

//BOOL APIENTRY DllMain( HANDLE hModule, 
//                       DWORD  ul_reason_for_call, 
//                       LPVOID lpReserved
//					 )
//{
//    switch (ul_reason_for_call)
//	{
//		case DLL_PROCESS_ATTACH:
//		case DLL_THREAD_ATTACH:
//		case DLL_THREAD_DETACH:
//		case DLL_PROCESS_DETACH:
//			break;
//    }
//    return TRUE;
//}



TASDK::TASDK(){

}

TASDK::~TASDK(){

}

    //Returns max of two values
	double TASDK::maxVal(double Value1, double Value2){
    if(Value1 > Value2){
        return Value1;
    }
    else if(Value2 > Value1){
        return Value2;
    }
    else{
      return 0;
    }
  }

  //Returns min of two values
  double TASDK::minVal(double Value1, double Value2){
    if(Value1 < Value2){
        return Value1;
    }
    else if(Value2 < Value1){
        return Value2;
    }
    else{
      return 0;
    }
  }

  //Returns max of two values
  int TASDK::maxVal(int Value1, int Value2){
    if(Value1 > Value2){
        return Value1;
    }
    else if(Value2 > Value1){
        return Value2;
    }
    else{
      return 0;
    }
  }

  //Returns min of two values
  int TASDK::minVal(int Value1, int Value2){
    if(Value1 < Value2){
        return Value1;
    }
    else if(Value2 < Value1){
        return Value2;
    }
    else{
      return 0;
    }
  }

  //Normalizes a value
  double TASDK::normalize(double Max, double Min, double Value){
    if (Max == Min){
      return 0;
    }
    else{
      return (Value - Min) / (Max - Min);
    }
  }

  //Returns the absolute value of a double
  double TASDK::Abs(double Value){
	  if(Value < 0){
		return -1 * Value;
	  }
	  else{
		return Value;
	  }
  }

   //Concatenates two char pointers
  char* TASDK::concatCharPtr(char* String1, char* String2){
	std::string stdString1 = String1;
	std::string stdString2 = String2;
	std::string temp = stdString1 + stdString2;
	char* psz = new char[temp.size() + 1];
	strcpy(psz, temp.c_str());				
	return psz;
  }

  int TASDK::compareChar(register char *p, register char *q)
  {
		while (*p) {
			if (!*q || *p > *q)
				return GREATER;
			if (*p++ < *q++)
				return LESS;
		} return *q? LESS: EQUAL;
  }
