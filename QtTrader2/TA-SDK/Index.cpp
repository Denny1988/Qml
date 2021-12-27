/**
 * Title:        TASDK
 * Description:  Technical Analysis Library
 * Copyright:    Copyright (c) 1999 to 2005
 * Company:      Modulus Financial Engineering
 * @author		 Modulus FE
 * @version		 6
 */

#include "TASDK.h"
#include <iostream>
using namespace std;

CIndex::CIndex()
{

}

CIndex::~CIndex()
{

}

  CRecordset* CIndex::MoneyFlowIndex(CDatabase* pData, CRecordset* pOHLCV,
            int Periods, const char* Alias){

    CRecordset* Results = new CRecordset();
    CField* Field1;
    int RecordCount = 0;
    int Record = 0;
    int Period = 0;
    int Start = 0;
    double Price1 = 0;
    double Price2 = 0;
    double V = 0;
    double Today = 0;
    double PosFlow = 0;
    double NegFlow = 0;
    double MoneyIndex = 0;
    double MoneyRatio = 0;

    RecordCount = pData->getRecordCount();

    if(Periods < 1 || Periods > RecordCount){
        cout << "Index->MoneyFlowIndex: Invalid Periods";
        return NULL;
    }

    Field1 = new CField(RecordCount, Alias);

    Start = Periods + 2;
    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 2; Record++){

        PosFlow = 0;
        NegFlow = 0;

        pData->setPosition(Record - Periods);

        for(Period = 1; Period < Periods + 1; Period++){

            pData->MovePrevious();
            Price1 = (pOHLCV->getField("High")->getValue(pData->getPosition()) +
                    pOHLCV->getField("Low")->getValue(pData->getPosition()) +
                    pOHLCV->getField("Close")->getValue(pData->getPosition())) / 3;
            pData->MoveNext();
            V = pOHLCV->getField("Volume")->getValue(pData->getPosition());
            if(V < 1){V = 1;}
            Price2 = (pOHLCV->getField("High")->getValue(pData->getPosition()) +
                    pOHLCV->getField("Low")->getValue(pData->getPosition()) +
                    pOHLCV->getField("Close")->getValue(pData->getPosition())) / 3;

            if(Price2 > Price1){
                PosFlow += Price2 * V;
            }
            else if(Price2 < Price1){
                NegFlow += Price2 * V;
            }

            pData->MoveNext();

        }//Period

        pData->MovePrevious();

        if(PosFlow != 0 && NegFlow != 0){
            MoneyRatio = PosFlow / NegFlow;
            MoneyIndex = 100 - (100 / (1 + MoneyRatio));
            Field1->setValue(pData->getPosition(), MoneyIndex);
        }

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CIndex::TradeVolumeIndex(CDatabase* pData, CField* pSource,
              CField* Volume, double MinTickValue, const char* Alias){

    CRecordset* Results = new CRecordset();
    CField* Field1;
    int RecordCount = 0;
    int Record = 0;
    int Period = 0;
    int Start = 0;
    int Direction = 0;
    int LastDirection = 0;
    double Change = 0;
    double TVI = 0;

    RecordCount = pData->getRecordCount();

    if(MinTickValue < 0){
        cout << "Index->TradeVolumeIndex: Invalid MinTickValue";
    }

    Field1 = new CField(RecordCount, Alias);

    Start = 2;
    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 1; Record++){

        Change = pSource->getValue(pData->getPosition()) -
                pSource->getValue(pData->getPosition() - 1);

        if(Change > MinTickValue){
            Direction = 1;
        }
        else if(Change < -MinTickValue){
            Direction = -1;
        }

        if(Change <= MinTickValue && Change >= -MinTickValue){
            Direction = LastDirection;
        }

        LastDirection = Direction;

        if(Direction == 1){
            TVI = TVI + Volume->getValue(pData->getPosition());
        }
        else if(Direction == -1){
            TVI = TVI - Volume->getValue(pData->getPosition());
        }

        Field1->setValue(pData->getPosition(), TVI);

        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CIndex::SwingIndex(CDatabase* pData, CRecordset* pOHLCV,
            double LimitMoveValue, const char* Alias){

	TASDK* TASDK1 = new TASDK();
	
    CRecordset* Results = new CRecordset();
    CField* Field1;
    int RecordCount = 0;
    int Record = 0;
    int Period = 0;
    int Start = 0;
    double Cy = 0;
    double Ct = 0;
    double Oy = 0;
    double Ot = 0;
    double Hy = 0;
    double Ht = 0;
    double Ly = 0;
    double Lt = 0;
    double K = 0;
    double R = 0;
    double A = 0;
    double B  = 0;
    double C = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    if(LimitMoveValue <= 0){
        cout << "Index->SwingIndex: Invalid LimitMoveValue";
        return NULL;
    }

    Field1 = new CField(RecordCount, Alias);

    Start = 2;
    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 1; Record++){

        Oy = pOHLCV->getField("Open")->getValue(pData->getPosition() - 1);
        Ot = pOHLCV->getField("Open")->getValue(pData->getPosition());
        Hy = pOHLCV->getField("High")->getValue(pData->getPosition() - 1);
        Ht = pOHLCV->getField("High")->getValue(pData->getPosition());
        Ly = pOHLCV->getField("Low")->getValue(pData->getPosition() - 1);
        Lt = pOHLCV->getField("Low")->getValue(pData->getPosition());
        Cy = pOHLCV->getField("Close")->getValue(pData->getPosition() - 1);
        Ct = pOHLCV->getField("Close")->getValue(pData->getPosition());

        K = TASDK1->maxVal(fabs(Ht - Cy), fabs(Lt - Cy));

        A = fabs(Ht - Cy);
        B = fabs(Lt - Cy);
        C = fabs(Ht - Lt);

        if(A > B && A > C){
            R = fabs(Ht - Cy) - 0.5 * fabs(Lt - Cy) + 0.25 * fabs(Cy - Oy);
        }
        else if(B > A && B > C){
            R = fabs(Lt - Cy) - 0.5 * fabs(Ht - Cy) + 0.25 * fabs(Cy - Oy);
        }
        else if(C > A && C > B){
            R = fabs(Ht - Lt) + 0.25 * fabs(Cy - Oy);
        }

        Value = 50 * ((Ct - Cy) + 0.5 * (Ct - Ot) + 0.25 *
                (Cy - Oy)) / R * K / LimitMoveValue;

        Field1->setValue(pData->getPosition(), Value);

        pData->MoveNext();

    }//Record

	delete TASDK1;
    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CIndex::AccumulativeSwingIndex(CDatabase* pData, CRecordset* pOHLCV,
            double LimitMoveValue, const char* Alias){

    CRecordset* Results = new CRecordset();
    CRecordset* RawSI;
    CField* Field1;
    CIndex* SI = new CIndex();
    int RecordCount = 0;
    int Record = 0;
    int Start = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    if(LimitMoveValue <= 0){
        cout << "Index->AccumulativeSwingIndex: Invalid LimitMoveValue";
    }

    RawSI = SI->SwingIndex(pData, pOHLCV, LimitMoveValue, "SI");

    Field1 = new CField(RecordCount, Alias);

    Start = 2;
    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 1; Record++){
        Value = RawSI->getValue("SI", pData->getPosition()) +
                Field1->getValue(pData->getPosition() - 1);
        Field1->setValue(pData->getPosition(), Value);
        pData->MoveNext();
    }//Record

    pData->MoveFirst();
	
	delete RawSI;
	delete SI;
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CIndex::RelativeStrengthIndex(CDatabase* pData, CField* pSource,
            int Periods, const char* Alias){

    CRecordset* Results = new CRecordset();
    CField* Field1;
    CField* AU;
    CField* AD;
    int RecordCount = 0;
    int Record = 0;
    int Period = 0;
    int Start = 0;
    double UT = 0;
    double DT = 0;
    double UpSum = 0;
    double DownSum = 0;
    double RS = 0;
    double RSI = 0;
	double value = 0;

    RecordCount = pData->getRecordCount();

    if(Periods < 1 || Periods > RecordCount){
        cout << "Index->RelativeStrengthIndex: Invalid Periods";
    }

    Field1 = new CField(RecordCount, Alias);
    AU = new CField(RecordCount, "AU");
    AD = new CField(RecordCount, "AD");
	
    pData->setPosition(2);
    for(Period = 1; Period < Periods + 1; Period++){
        UT = 0;
        DT = 0;
			
		if(value != NULL_VALUE){
			if(value > pSource->getValue(pData->getPosition() - 1)){
				UT = pSource->getValue(pData->getPosition()) -
						pSource->getValue(pData->getPosition() - 1);
				UpSum += UT;
			}
			else if(pSource->getValue(pData->getPosition()) <
						pSource->getValue(pData->getPosition() - 1)){
				DT = pSource->getValue(pData->getPosition() - 1) -
						pSource->getValue(pData->getPosition());
				DownSum += DT;
			}
		}
        pData->MoveNext();
    }//Period

    pData->MovePrevious();

    UpSum = UpSum / Periods;
    AU->setValue(pData->getPosition(), UpSum);
    DownSum = DownSum / Periods;
    AD->setValue(pData->getPosition(), DownSum);

    RS = UpSum / DownSum;

    RSI = 100 - (100 / (1 + RS));

    Start = Periods + 3;

    for(Record = Start; Record < RecordCount + 2; Record++){

        pData->setPosition(Record - Periods);

        UpSum = 0;
        DownSum = 0;

        for(Period = 1; Period < Periods + 1; Period++){
            UT = 0;
            DT = 0;
			value = pSource->getValue(pData->getPosition());
			if(value != NULL_VALUE){
				if(value > pSource->getValue(pData->getPosition() - 1)){
					UT = pSource->getValue(pData->getPosition()) -
							pSource->getValue(pData->getPosition() - 1);
					UpSum += UT;
				}
				else if(pSource->getValue(pData->getPosition()) <
							pSource->getValue(pData->getPosition() - 1)){
					DT = pSource->getValue(pData->getPosition() - 1) -
							pSource->getValue(pData->getPosition());
					DownSum += DT;
				}
			}
            pData->MoveNext();
        }//Period

        pData->MovePrevious();

        UpSum = (((AU->getValue(pData->getPosition() - 1) * (Periods - 1)) + UT)) / Periods;
        DownSum = (((AD->getValue(pData->getPosition() - 1) * (Periods - 1)) + DT)) / Periods;

        AU->setValue(pData->getPosition(), UpSum);
        AD->setValue(pData->getPosition(), DownSum);

        if(DownSum == 0) DownSum = UpSum;
        if(UpSum == 0){
            RS = 0;
        }
		else{
            RS = UpSum / DownSum;
        }

        RS = (UpSum / DownSum);
        RSI = 100 - (100 / (1 + RS));

        Field1->setValue(pData->getPosition(), RSI);

    }//Record

	delete AU;
	delete AD;
    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CIndex::ComparativeRelativeStrength(CDatabase* pData, CField* pSource1,
            CField* pSource2, const char* Alias){

    CRecordset* Results = new CRecordset();
    CField* Field1;
    int RecordCount = 0;
    int Record = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    for(Record = 1; Record < RecordCount + 1; Record++){

        Value = pSource1->getValue(pData->getPosition()) / pSource2->getValue(pData->getPosition());
		if(Value == 1) Value = NULL_VALUE;
        Field1->setValue(pData->getPosition(), Value);

        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CIndex::PriceVolumeTrend(CDatabase* pData, CField* pSource,
            CField* Volume, const char* Alias){

    CRecordset* Results = new CRecordset();
    CField* Field1;
    int RecordCount = 0;
    int Record = 0;
    int Start = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Start = 2;
    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 1; Record++){
        Value = (((pSource->getValue(pData->getPosition()) -
                pSource->getValue(pData->getPosition() - 1)) /
                pSource->getValue(pData->getPosition() - 1)) *
                Volume->getValue(pData->getPosition())) +
                Field1->getValue(pData->getPosition() - 1);
        Field1->setValue(pData->getPosition(), Value);
        pData->MoveNext();
    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CIndex::PositiveVolumeIndex(CDatabase* pData,
            CField* pSource, CField* Volume, const char* Alias){

    CRecordset* Results = new CRecordset();
    CField* Field1;
    int RecordCount = 0;
    int Record = 0;
    int Start = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Start = 2;
    pData->setPosition(Start);

    Field1->setValue(1, 1);

    for(Record = Start; Record < RecordCount + 1; Record++){

        if(Volume->getValue(pData->getPosition()) >
                Volume->getValue(pData->getPosition() - 1)){
            Value = Field1->getValue(pData->getPosition() - 1) +
                    (pSource->getValue(pData->getPosition()) -
                    pSource->getValue(pData->getPosition() - 1)) /
                    pSource->getValue(pData->getPosition() - 1) *
                    Field1->getValue(pData->getPosition() - 1);
        }
        else{
            Value = Field1->getValue(pData->getPosition() - 1);
        }

        Field1->setValue(pData->getPosition(), Value);

        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CIndex::NegativeVolumeIndex(CDatabase* pData,
            CField* pSource, CField* Volume, const char* Alias){

    CRecordset* Results = new CRecordset();	
    CField* Field1;
    int RecordCount = 0;
    int Record = 0;
    int Start = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Start = 2;
    pData->setPosition(Start);

    Field1->setValue(1, 1);

    for(Record = Start; Record < RecordCount + 1; Record++)
	{
		int pos = pData->getPosition();
		if (Volume->getValue(pos) < Volume->getValue(pos - 1))
		{
			Value = Field1->getValue(pos - 1) + (pSource->getValue(pos) - pSource->getValue(pos - 1)) / pSource->getValue(pos - 1) * Field1->getValue(pos - 1);
        }
        else
		{
			Value = Field1->getValue(pos - 1);
        }

		Field1->setValue(pos, Value);

        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CIndex::Performance(CDatabase* pData, CField* pSource, const char* Alias){

    CRecordset* Results = new CRecordset();
    CField* Field1;
    int RecordCount = 0;
    int Record = 0;
    int Start  = 0;
    double FirstPrice = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Start = 2;
    pData->setPosition(Start);

    FirstPrice = pSource->getValue(1);

    for(Record = Start; Record < RecordCount + 1; Record++){

        Value = ((pSource->getValue(pData->getPosition()) -
                FirstPrice) / FirstPrice) * 100;
        Field1->setValue(pData->getPosition(), Value);

        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CIndex::OnBalanceVolume(CDatabase* pData, CField* pSource,
            CField* Volume, const char* Alias){

    CRecordset* Results = new CRecordset();
    CField* Field1;
    int RecordCount = 0;
    int Record = 0;
    int Start = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Start = 2;
    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 1; Record++){

        if(pSource->getValue(pData->getPosition() - 1) <
            pSource->getValue(pData->getPosition())){
            Value = Field1->getValue(pData->getPosition() - 1) +
                Volume->getValue(pData->getPosition());
        }
        else if(pSource->getValue(pData->getPosition()) <
            pSource->getValue(pData->getPosition() - 1)){
            Value = Field1->getValue(pData->getPosition() - 1) -
                Volume->getValue(pData->getPosition());
        }
        else{
            Value = Field1->getValue(pData->getPosition() - 1);
        }

        Field1->setValue(pData->getPosition(), Value);

        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CIndex::MassIndex(CDatabase* pData, CRecordset* pOHLCV, int Periods, const char* Alias){

    CRecordset* Results = new CRecordset();
    CField* Field1;
    CGeneral* GE = new CGeneral();
    CMovingAverage* MA = new CMovingAverage();
    CField* Temp;
    CRecordset* HML;
    CRecordset* EMA1;
    CRecordset* EMA2;
    int RecordCount = 0;
    int Record = 0;
    int Start = 0;
    int Period = 0;
    double Sum = 0;

    RecordCount = pData->getRecordCount();

    if(Periods < 1 || Periods > RecordCount){
        cout << "Index->MassIndex: Invalid Periods";
        return NULL;
    }

    Field1 = new CField(RecordCount, Alias);

    HML = GE->HighMinusLow(pData, pOHLCV, "HML");
    Temp = HML->getField("HML");
    EMA1 = MA->ExponentialMovingAverage(pData, Temp, 9, "EMA");
    Temp = EMA1->getField("EMA");
    EMA2 = MA->ExponentialMovingAverage(pData, Temp, 9, "EMA");

    Start = (Periods * 2) + 1;
    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 2; Record++){

        Sum = 0;

        pData->setPosition(Record - Periods);
        for(Period = 1; Period < Periods + 1; Period++){
            Sum = Sum + (EMA1->getValue("EMA", pData->getPosition()) /
            EMA2->getValue("EMA", pData->getPosition()));
            pData->MoveNext();
        }//Period
        pData->MovePrevious();

        Field1->setValue(pData->getPosition(), Sum);

        pData->MoveNext();

    }//Record

    pData->MoveFirst();

	delete GE;
	delete MA;
	delete HML;
	delete EMA1;
	delete EMA2;

    Results->addField(Field1);
    return Results;

  }

  CRecordset* CIndex::ChaikinMoneyFlow(CDatabase* pData, CRecordset* pOHLCV, 
	  int Periods, const char* Alias){

    CRecordset* Results = new CRecordset();
    CField* Field1;
    
	int RecordCount = 0;
    int Record = 0;
    double Value = 0;
    double Sum = 0, SumV = 0;
	double a = 0, b = 0;
    
    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    
	for(Record = Periods + 1; Record < RecordCount + 1; Record++){
	
        Sum = 0;
        SumV = 0;

		double test = pOHLCV->getValue("Close", Record - 0);

		for(int n = 0; n != Periods; ++n){
			a = ((pOHLCV->getValue("Close", Record - n) - pOHLCV->getValue("Low", Record - n)) - 
            (pOHLCV->getValue("High", Record - n) - pOHLCV->getValue("Close", Record - n)));
			b = (pOHLCV->getValue("High", Record - n) - pOHLCV->getValue("Low", Record - n)) * 
            pOHLCV->getValue("Volume", Record - n);
            if(a != 0 && b != 0) Sum += a / b;
            SumV += pOHLCV->getValue("Volume", Record - n);
        }

        Value = Sum / SumV * pow(SumV, 2);
     
        Field1->setValue(Record, Value);
        
    } //Record

    pData->MoveFirst();
    
    Results->addField(Field1);
    return Results;

  }

CRecordset* CIndex::CommodityChannelIndex(CDatabase* pData, CRecordset* pOHLCV, 
	  int Periods, const char* Alias){

	CGeneral* GN = new CGeneral();
    CMovingAverage* MA = new CMovingAverage();
	CRecordset* Results = new CRecordset();
	CRecordset* TPrs;
	CRecordset* MArs;
	CField* Field1;
    
    double dMeanDeviation = 0;
    double dTmp = 0;
    long Count = 0;
	int RecordCount = 0;
    int Record = 0;
    
    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    TPrs = GN->TypicalPrice(pData, pOHLCV, "TP");
    MArs = MA->SimpleMovingAverage(pData, TPrs->getField("TP"), Periods, "TPMA");
    for(Record = 1; Record != (2 * Periods) + 1; ++Record){
        Field1->setValue(Record, 0);
    }
    
	for(Record = (2 * Periods); Record != RecordCount + 1; ++Record){
        dMeanDeviation = 0;
        for(Count = (Record - Periods); Count != Record +1; ++Count){
            dTmp = fabs(TPrs->getField("TP")->getValue(Count) - 
				MArs->getField("TPMA")->getValue(Count));
            dMeanDeviation = dMeanDeviation + dTmp;
        } //Count
        dMeanDeviation = dMeanDeviation / Periods;
        dTmp = (TPrs->getField("TP")->getValue(Record) - 
			MArs->getField("TPMA")->getValue(Record)) / (dMeanDeviation * 0.015);
        Field1->setValue(Record, dTmp);
    } //Record

    pData->MoveFirst();
    Results->addField(Field1);
    
	delete TPrs;
	delete MArs;
	delete GN;
	delete MA;

	return Results;

}