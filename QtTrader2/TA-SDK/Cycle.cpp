/**
 * Title:        TASDK
 * Description:  Technical Analysis Library
 * Copyright:    Copyright (c) 2002
 * Company:      Modulus Financial Engineering
 * @author		 Modulus FE
 * @version		 1.0
 */

#include "TASDK.h"

CCycle::CCycle()
{

}

CCycle::~CCycle()
{

}

  int CCycle::Analyze(CField* Source){

    int Period = 0;
    int Record = 0;
    int RecordCount = 0;
    int Start = 0;
    double StartValue = 0;
    double EndValue1 = 0;
    double EndValue2 = 0;
    int Total = 0;
    int N = 0;

    RecordCount = Source->getRecordCount();

    for(Record = 2; Record < RecordCount; Record++){
        Start = Record + 2;
        StartValue = Source->getValue(Record);
        EndValue1 = Source->getValue(Record - 1);
        EndValue2 = Source->getValue(Record + 1);
        for(Period = Start; Period < RecordCount + 1; Period++){
            if(Source->getValue(Period) > EndValue2 &&
               Source->getValue(Period) < EndValue1){
                EndValue1 = 0;
                break;
            }
        }//Period
        if(EndValue1 == 0){Total += (Period - Start) + 1;}
        N += 1;
    } //Record

    Total = Total / (RecordCount - 2);

    return Total + 1;

  }
