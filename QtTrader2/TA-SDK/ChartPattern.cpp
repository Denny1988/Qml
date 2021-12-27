/**
 * Title:        TASDK
 * Description:  Technical Analysis Library
 * Copyright:    Copyright (c) 2002 to 2005
 * Company:      Modulus Financial Engineering
 * @author		 Modulus FE
 * @version		 6.0
 */

#include "TASDK.h"

CChartPattern::CChartPattern()
{

}

CChartPattern::~CChartPattern()
{

}

   CNote* CChartPattern::PercentRetracement(CDatabase* Data,
            CField* Source, int StartPeriod, int EndPeriod){
    
	CGeneral* G = new CGeneral();
    CNote* N = new CNote();
    int Record = 0;
    int RecordCount = 0;
    int Periods = 0;
    int Period = 0;
    int HHP = 0;
    int LLP = 0;
    double HH = 0;
    double LL = 0;
    double LastPrice = 0;
    double EndPrice = 0;
    double Pct = 0;
    double Range = 0;

    RecordCount = Data->getRecordCount();
    Periods = EndPeriod - StartPeriod;

    if(RecordCount < EndPeriod){
        //SetLastError((DWORD) 0x10040);
		//cout << "ChartPattern->PercentRetracement: Invalid end period";
        return NULL;
    }
    else if(StartPeriod < 1){
		//SetLastError((DWORD) 0x10041);
        //cout << "ChartPattern->PercentRetracement: Invalid start period";
        return NULL;
    }

    N = G->MaxValue(Source, StartPeriod, EndPeriod);
    HH = N->getValue();
    HHP = N->getPeriod();
    N = G->MinValue(Source, StartPeriod, EndPeriod);
    LL = N->getValue();
    LLP = N->getPeriod();

    LastPrice = Source->getValue(EndPeriod);
    Range = HH - LL;

    if(HHP > LLP){
        N = G->MinValue(Source, HHP, EndPeriod);
        EndPrice = N->getValue();
        Pct = 100 - ((LastPrice - LL) / Range) * 100;
        N->setNote("ADVANCING");
        N->setPeriod(0);
        N->setValue(Pct);
    }
    else if(LLP > HHP){
        N = G->MaxValue(Source, LLP, EndPeriod);
        EndPrice = N->getValue();
        Pct = 100 - ((HH - LastPrice) / Range) * 100;
        N->setNote("DECLINING");
        N->setPeriod(0);
        N->setValue(Pct);
    }

	delete G;

    return N;

  }
