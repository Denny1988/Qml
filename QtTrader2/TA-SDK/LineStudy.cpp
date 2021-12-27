/**
 * Title:        TASDK
 * Description:  Technical Analysis Library
 * Copyright:    Copyright (c) 2002
 * Company:      Modulus Financial Engineering
 * @author		 Modulus FE
 * @version		 1.0
 */

#include "TASDK.h"

CLineStudy::CLineStudy()
{

}

CLineStudy::~CLineStudy()
{

}

  CRecordset* CLineStudy::QuadrantLines(CDatabase* Data, CRecordset* OHLCV, 
				int StartPeriod, int EndPeriod){

    CField* Field1;
    CField* Field2;
    CField* Field3;
    CField* Field4;
    CRecordset* Results = new CRecordset();
    CGeneral* G = new CGeneral();
    CNote* N = new CNote();
    int Record = 0;
    int RecordCount = 0;
    int Periods = 0;
    int Period = 0;
    double HH = 0;
    double LL = 0;

    RecordCount = Data->getRecordCount();

    Field1 = new CField(RecordCount, "QuadrantLines1");
    Field2 = new CField(RecordCount, "QuadrantLines2");
    Field3 = new CField(RecordCount, "QuadrantLines3");
    Field4 = new CField(RecordCount, "QuadrantLines4");

    Periods = EndPeriod - StartPeriod;
    if(RecordCount < EndPeriod){
        //cout << "QuadrantLines: Invalid end period";
        return NULL;
    }
    else if(StartPeriod < 1){
		//SetLastError((DWORD) 0x10006);
        //cout << "QuadrantLines: Invalid start period";
        return NULL;
    }

    N = G->MaxValue(OHLCV->getField("High"), StartPeriod, EndPeriod);
    HH = N->getValue();
    N = G->MinValue(OHLCV->getField("Low"), StartPeriod, EndPeriod);
    LL = N->getValue();

    Data->setPosition(StartPeriod - 1);
    for(Record = StartPeriod; Record < EndPeriod + 1; Record++){
        Field1->setValue(Record, HH);
        Field2->setValue(Record, LL + (((HH - LL) / 4) * 3));
        Field3->setValue(Record, LL + ((HH - LL) / 4));
        Field4->setValue(Record, LL);
        Data->MoveNext();
    }//Record

    Data->MoveFirst();

    Results->addField(Field1);
    Results->addField(Field2);
    Results->addField(Field3);
    Results->addField(Field4);

	delete G;
	delete N;

    return Results;

  }

  CRecordset* CLineStudy::TironeLevel(CDatabase* Data, CRecordset* OHLCV, 
				int StartPeriod, int EndPeriod){

    CField* Field1;
    CField* Field2;
    CField* Field3;
    CRecordset* Results = new CRecordset();
    CGeneral* G = new CGeneral();
    CNote* N = new CNote();
    int Record = 0;
    int RecordCount = 0;
    int Periods = 0;
    int Period = 0;
    double HH = 0;
    double LL = 0;
    double Top = 0;
    double Median = 0;
    double Bottom = 0;

    RecordCount = Data->getRecordCount();

    Field1 = new CField(RecordCount, "TironeTop");
    Field2 = new CField(RecordCount, "TironeMedian");
    Field3 = new CField(RecordCount, "TironeBottom");

    Periods = EndPeriod - StartPeriod;
    if(RecordCount < EndPeriod){
		//SetLastError((DWORD) 0x10040);
        //cout << "TironeLevel: Invalid end period";
        return NULL;
    }
    else if(StartPeriod < 1){
		//SetLastError((DWORD) 0x10041);
        //cout << "TironeLevel: Invalid start period";
        return NULL;
    }

    N = G->MaxValue(OHLCV->getField("High"), StartPeriod, EndPeriod);
    HH = N->getValue();
    N = G->MinValue(OHLCV->getField("Low"), StartPeriod, EndPeriod);
    LL = N->getValue();

    Top = HH - (HH - LL) / 3;
    Median = LL + (HH - LL) / 2;
    Bottom = LL + (HH - LL) / 3;

    Data->setPosition(StartPeriod - 1);
    for(Record = StartPeriod; Record < EndPeriod + 1; Record++){
        Field1->setValue(Record, Top);
        Field2->setValue(Record, Median);
        Field3->setValue(Record, Bottom);
        Data->MoveNext();
    }//Record

    Data->MoveFirst();

    Results->addField(Field1);
    Results->addField(Field2);
    Results->addField(Field3);

	delete G;
	delete N;

    return Results;

  }

  CRecordset* CLineStudy::SpeedResistanceLines(CDatabase* Data,
            CRecordset* OHLCV, int StartPeriod, int EndPeriod){

    CField* Field1;
    CField* Field2;
    CField* Field3;
    CRecordset* Results = new CRecordset();
    CGeneral* G = new CGeneral();
    CNote* N = new CNote();
    int Record = 0;
    int RecordCount = 0;
    int Periods = 0;
    int Period = 0;
    int NewStartPeriod = 0;
    int NewEndPeriod = 0;
    double Value = 0;
    double HH = 0;
    double LL = 0;
    double Top = 0;
    double Median = 0;
    double Bottom = 0;
    double Line1 = 0;
    double Line2 = 0;
    double Line3 = 0;
    double Delta = 0;
    double Angle = 0;

    RecordCount = Data->getRecordCount();
    Field1 = new CField(RecordCount, "SpeedResistanceLineTop");
    Field2 = new CField(RecordCount, "SpeedResistanceLineMedian");
    Field3 = new CField(RecordCount, "SpeedResistanceLineBottom");

    Periods = EndPeriod - StartPeriod;
    if(RecordCount < EndPeriod){
		//SetLastError((DWORD) 0x10040);
        //cout << "SpeedResistanceLines: Invalid end period";
        return NULL;
    }
    else if(StartPeriod < 1){
		//SetLastError((DWORD) 0x10041);
        //cout << "SpeedResistanceLines: Invalid start period";
        return NULL;
    }

    N = G->MaxValue(OHLCV->getField("High"), StartPeriod, EndPeriod);
    HH = N->getValue();
    NewEndPeriod = N->getPeriod();
    N = G->MinValue(OHLCV->getField("Low"), StartPeriod, EndPeriod);
    LL = N->getValue();
    NewStartPeriod = N->getPeriod();

    StartPeriod = NewStartPeriod;
    EndPeriod = NewEndPeriod;

    Value = (HH - LL) / 3;
    Line1 = Value + LL;
    Line2 = (Value * 2) + LL;
    Line3 = (Value * 3) + LL;

    Delta = (Line1 - LL) / (EndPeriod - StartPeriod);
    Angle = 0;
    Data->setPosition(StartPeriod - 1);
    for(Record = StartPeriod; Record < EndPeriod + 1; Record++){
        Angle += Delta;
        Field1->setValue(Record, Angle + LL);
        Data->MoveNext();
    } //Record

    Delta = (Line2 - LL) / (EndPeriod - StartPeriod);
    Angle = 0;
    Data->setPosition(StartPeriod - 1);
    for(Record = StartPeriod; Record < EndPeriod + 1; Record++){
        Angle += Delta;
        Field2->setValue(Record, Angle + LL);
        Data->MoveNext();
    }//Record

    Delta = (Line3 - LL) / (EndPeriod - StartPeriod);
    Angle = 0;
    Data->setPosition(StartPeriod - 1);
    for(Record = StartPeriod; Record < EndPeriod + 1; Record++){
        Angle += Delta;
        Field3->setValue(Record, Angle + LL);
        Data->MoveNext();
    }//Record

    Data->MoveFirst();

    Results->addField(Field1);
    Results->addField(Field2);
    Results->addField(Field3);

	delete G;
	delete N;

    return Results;

  }

  CRecordset* CLineStudy::Fibonacci(CField* Source, int StartPeriod, int EndPeriod, char* Alias){

    CRecordset* Results = new CRecordset();
    CField* Field1;
    int Record = 0;
    int RecordCount = 0;
    int Period = 0;
    double Fib1 = 0;
    double Fib2 = 0;
    double Fib3 = 0;
    int Total = 0;

    Fib1 = 1;

    if(Alias == "" || Alias == NULL){
      Alias = (char*)"Fibonacci";
    }

    RecordCount = Source->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Total = EndPeriod - StartPeriod;

    for(Period = StartPeriod; Period < EndPeriod + 1; Period++){
        Fib3 = Fib2;
        Fib2 = Fib1;
        Fib1 = Fib2 + Fib3;
        if(Period + Fib1 > EndPeriod){
            break;
        }
        else{
            Field1->setValue((int)(Period + Fib1) - 1, Fib1);
        }
    }//Period

    Results->addField(Field1);
    return Results;

  }
