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
#include "math.h"
using namespace std;
CGeneral::CGeneral()
{

}

CGeneral::~CGeneral()
{

}

  CRecordset* CGeneral::HighMinusLow(CDatabase* pData, CRecordset* pOHLCV, const char* Alias){

    CField* Field1;
    CRecordset* Results = new CRecordset();
    int Record = 0;
    int RecordCount = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    pData->MoveFirst();

    for(Record = 1; Record < RecordCount + 1; Record++){
        Value = (pOHLCV->getField("High")->getValue(Record) -
        pOHLCV->getField("Low")->getValue(Record));
        Field1->setValue(Record,Value);
        pData->MoveNext();
    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CGeneral::MedianPrice(CDatabase* pData, CRecordset* pOHLCV, const char* Alias){

    CField* Field1;
    CRecordset* Results = new CRecordset();
    int Record = 0;
    int RecordCount = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    pData->MoveFirst();

    for(Record = 1; Record < RecordCount + 1; Record++){
        Value = (pOHLCV->getField("High")->getValue(Record) +
        pOHLCV->getField("Low")->getValue(Record)) / 2;
        Field1->setValue(Record, Value);
        pData->MoveNext();
    } //Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CGeneral::TypicalPrice(CDatabase* pData, CRecordset* pOHLCV, const char* Alias){

    CField* Field1;
    CRecordset* Results = new CRecordset();
    int Record = 0;
    int RecordCount = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    pData->MoveFirst();

    for(Record = 1; Record < RecordCount + 1; Record++){
        Value = (pOHLCV->getField("High")->getValue(Record) +
                pOHLCV->getField("Low")->getValue(Record) +
                pOHLCV->getField("Close")->getValue(Record)) / 3;
        Field1->setValue(Record, Value);
        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CGeneral::WeightedClose(CDatabase* pData, CRecordset* pOHLCV, const char* Alias){

    CField* Field1;
    CRecordset* Results = new CRecordset();
    int Record = 0;
    int RecordCount = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    pData->MoveFirst();

    for(Record = 1; Record < RecordCount + 1; Record++){
       Value = (pOHLCV->getField("High")->getValue(Record) +
                pOHLCV->getField("Low")->getValue(Record) +
                (pOHLCV->getField("Close")->getValue(Record) * 2)) / 4;
        Field1->setValue(Record, Value);
        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CGeneral::VolumeROC(CDatabase* pData, CField* Volume, int Periods, const char* Alias){

    CField* Field1;
    CRecordset* Results = new CRecordset();
    int Record = 0;
    int RecordCount = 0;
    double Value = 0;
    int Start = 0;
    double PrevVolume = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Start = Periods + 1;

    pData->setPosition(Start);

     for(Record = Start; Record < RecordCount + 1; Record++){
        pData->setPosition(pData->getPosition() - Periods);
        PrevVolume = Volume->getValue(pData->getPosition());
        pData->setPosition(pData->getPosition() + Periods);
        if(PrevVolume != 0){
            Value = ((Volume->getValue(pData->getPosition()) - PrevVolume) / PrevVolume) * 100;
        }
        Field1->setValue(Record, Value);
        pData->MoveNext();
    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CGeneral::PriceROC(CDatabase* pData, CField* pSource, int Periods, const char* Alias){

    CField* Field1;
    CRecordset* Results = new CRecordset();
    int Record = 0;
    int RecordCount = 0;
    double Value = 0;
    int Start = 0;
    double PrevPrice = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Start = Periods + 1;

    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 1; Record++){
        PrevPrice = pSource->getValue(pData->getPosition() - Periods);
        Value = ((pSource->getValue(pData->getPosition()) - PrevPrice) / PrevPrice) * 100;
        Field1->setValue(Record, Value);
        pData->MoveNext();
    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  double CGeneral::CorrelationAnalysis(CField* pSource1, CField* pSource2){

    int Record = 0;
    int RecordCount = 0;
    double Total = 0;
    double A = 0;
    double B = 0;

    RecordCount = pSource1->getRecordCount();

    for (Record = 2; Record < RecordCount + 1; Record++){
        A = (pSource1->getValue(Record) - pSource1->getValue(Record - 1));
        B = (pSource2->getValue(Record) - pSource2->getValue(Record - 1));
		if (A < 0){A = -1 * A;}
		if (B < 0){B = -1 * B;}
        Total += (A * B);
    }//Record

    Total = Total / (RecordCount - 2);

    return 1 - Total;

  }

CRecordset* CGeneral::StandardDeviation(CDatabase* pData, CField* pSource,
            int Periods, int StandardDeviations,
            int MAType, const char* Alias){

    CRecordset* Results;

    CMovingAverage* MA = new CMovingAverage();
    CField* Field1;
    int Period = 0;
    int RecordCount = 0;
    int Record = 0;
    int Start = 0;
    double Sum = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    switch (MAType)
    {
    case indSimpleMovingAverage:
        Results = MA->SimpleMovingAverage(pData, pSource, Periods, "Temp");
        break;
    case indExponentialMovingAverage:
        Results = MA->ExponentialMovingAverage(pData, pSource, Periods, "Temp");
        break;
    case indTimeSeriesMovingAverage:
        Results = MA->TimeSeriesMovingAverage(pData, pSource, Periods, "Temp");
        break;
    case indTriangularMovingAverage:
        Results = MA->TriangularMovingAverage(pData, pSource, Periods, "Temp");
        break;
    case indVariableMovingAverage:
        Results = MA->VariableMovingAverage(pData, pSource, Periods, "Temp");
        break;
    case indWeightedMovingAverage:
        Results = MA->WeightedMovingAverage(pData, pSource, Periods, "Temp");
        break;
    case indVIDYA:
        Results = MA->VIDYA(pData, pSource, Periods, 0.65, "Temp");
        break;
    }

    Start = Periods + 1;
    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 1; Record++){

        Sum = 0;
        Value = Results->getValue("Temp", pData->getPosition());

        for(Period = 1; Period < Periods + 1; Period++){
            Sum += (pSource->getValue(pData->getPosition()) - Value) *
            (pSource->getValue(pData->getPosition()) - Value);
            pData->MovePrevious();
        }//Period

        pData->setPosition(pData->getPosition() + Periods);

        Value = StandardDeviations *  sqrt(Sum / Periods);
        Field1->setValue(pData->getPosition(), Value);

        pData->MoveNext();

    } //Record

    Results->addField(Field1);
    
	delete MA;
    return Results;

  }

  CNote* CGeneral::MaxValue(CField* pSource, int StartPeriod, int EndPeriod){

    int Record = 0;
    double HH = 0;
    int HHP = 0;
    CNote* Value = new CNote();

    if(StartPeriod > EndPeriod){
        return Value;
    }

    for(Record = StartPeriod; Record < EndPeriod + 1; Record++){
        if(pSource->getValue(Record) > HH){
            HH = pSource->getValue(Record);
            HHP = Record;
        }
    }//Record

    Value->setPeriod(HHP);
    Value->setValue(HH);

    return Value;

  }

  CNote* CGeneral::MinValue(CField* pSource, int StartPeriod, int EndPeriod){

    int Record = 0;
    double LL = 0;
    int LLP = 0;
    CNote* Value = new CNote();

    if(StartPeriod > EndPeriod){
        return Value;
    }

    LL = pSource->getValue(StartPeriod);
    LLP = StartPeriod;

    for(Record = StartPeriod; Record < EndPeriod + 1; Record++){
        if(pSource->getValue(Record) < LL){
            LL = pSource->getValue(Record);
            LLP = Record;
        }
    }//Record

    Value->setPeriod(LLP);
    Value->setValue(LL);

    return Value;

  }
