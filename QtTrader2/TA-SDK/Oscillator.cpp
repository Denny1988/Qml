/**
 * Title:        TASDK
 * Description:  Technical Analysis Library
 * Copyright:    Copyright (c) 1999 to 2005
 * Company:      Modulus Financial Engineering
 * @author		 Modulus FE
 * @version		 6
 */

#include "TASDK.h"
#include <string.h>
#include <iostream>
#include <math.h>

COscillator::COscillator()
{

}

COscillator::~COscillator()
{

}

  CRecordset* COscillator::ChandeMomentumOscillator(CDatabase* pData, CField* pSource, int Periods, const char* Alias){

    CField* Field1;
    CRecordset* Results = new CRecordset();
    int Record = 0;
    int RecordCount = 0;
    int Period = 0;
    double Today = 0;
    double Yesterday = 0;
    double UpSum = 0;
    double DownSum = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    pData->MoveFirst();

    for(Record = Periods + 2; Record < RecordCount + 2; Record++){

        //Move back n periods
        pData->setPosition (Record - Periods);
        UpSum = 0;
        DownSum = 0;

        for(Period = 1; Period < Periods + 1; Period++){

            pData->MovePrevious();
            Yesterday = pSource->getValue(pData->getPosition());
            pData->MoveNext();
            Today = pSource->getValue(pData->getPosition());

            if(Today > Yesterday){
                UpSum += (Today - Yesterday);
            }
            else if(Today < Yesterday){
                DownSum += (Yesterday - Today);
            }

            pData->MoveNext();

        }//Period

        pData->MovePrevious();
		if(UpSum + DownSum != 0){
			Value = 100 * (UpSum - DownSum) / (UpSum + DownSum);
		}
		else{
			Value = NULL_VALUE;
		}
        Field1->setValue(pData->getPosition(), Value);

        pData->MoveNext();

    }//Record

    pData->MoveFirst();

    Results->addField(Field1);

    return Results;

  }

  CRecordset* COscillator::Momentum(CDatabase* pData, CField* pSource, int Periods, const char* Alias){

    CRecordset* Results = new CRecordset();
    CField* Field1;
    int RecordCount = 0;
    int Record = 0;
    int Period = 0;
    int Start = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Start = Periods + 2;
    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 1; Record++){

        pData->setPosition(pData->getPosition() - Periods);
        Value = pSource->getValue(pData->getPosition());
        pData->setPosition (pData->getPosition() + Periods);
        Value = 100 + ((pSource->getValue(pData->getPosition()) - Value) / Value) * 100;
        Field1->setValue(pData->getPosition(), Value);
        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* COscillator::TRIX(CDatabase* pData, CField* pSource, int Periods, const char* Alias){

    CMovingAverage* MA = new CMovingAverage();
    CRecordset* Results = new CRecordset();
    CRecordset* RS;
    CField* EMA;
    CField* Field1;
    int RecordCount = 0;
    int Record = 0;
    int Period = 0;
    int Start = 0;
    double Value = 0;

    RS = MA->ExponentialMovingAverage(pData, pSource, Periods, "EMA1");
	EMA = RS->getField("EMA1");
	RS->removeField("EMA1");
	delete RS;

    RS = MA->ExponentialMovingAverage(pData, EMA, Periods, "EMA2");
	delete EMA;
	EMA = RS->getField("EMA2");
	RS->removeField("EMA2");	
	delete RS;	
	
    RS = MA->ExponentialMovingAverage(pData, EMA, Periods, "EMA3");
	delete EMA;
	EMA = RS->getField("EMA3");
	RS->removeField("EMA3");	
	delete RS;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Start = 2;
    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 1; Record++){

        pData->MovePrevious();
        Value = EMA->getValue(pData->getPosition());
        pData->MoveNext();
        if(Value != 0){
            Value = ((EMA->getValue(pData->getPosition()) - Value) / Value) * 100;
            Field1->setValue(pData->getPosition(), Value);
        }
        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);

	delete EMA;	
	delete MA;

    return Results;

  }

  CRecordset* COscillator::UltimateOscillator(CDatabase* pData, CRecordset* pOHLCV,
            int Cycle1, int Cycle2, int Cycle3, const char* Alias){

    CRecordset* Results = new CRecordset();
    CField* Field1;
    int RecordCount = 0;
    int Record = 0;
    int Period = 0;
    int Periods = 0;
    int Start = 0;
    double Value = 0;
    double TL = 0;
    double BP = 0;
    double TR = 0;
    double BPSum1 = 0;
    double BPSum2 = 0;
    double BPSum3 = 0;
    double TRSum1 = 0;
    double TRSum2 = 0;
    double TRSum3 = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Periods = Cycle1;
    if(Cycle2 > Periods){Periods = Cycle2;}
    if(Cycle3 > Periods){Periods = Cycle3;}

    Start = Periods + 2;
    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 2; Record++){

        BPSum1 = 0;
        BPSum2 = 0;
        BPSum3 = 0;
        TRSum1 = 0;
        TRSum2 = 0;
        TRSum3 = 0;

        pData->setPosition(Record - Cycle1);
        for(Period = 1; Period < Cycle1 + 1; Period++){
          if(pOHLCV->getField("Low")->getValue(pData->getPosition()) <
            pOHLCV->getField("Close")->getValue(pData->getPosition() - 1)){
            TL = pOHLCV->getField("Low")->getValue(pData->getPosition());
          }
          else{
                TL = pOHLCV->getField("Close")->getValue(pData->getPosition() - 1);
          }
            BP = pOHLCV->getField("Close")->getValue(pData->getPosition()) - TL;
            TR = pOHLCV->getField("High")->getValue(pData->getPosition()) -
                    pOHLCV->getField("Low")->getValue(pData->getPosition());
            if(TR < pOHLCV->getField("High")->getValue(pData->getPosition()) -
                    pOHLCV->getField("Close")->getValue(pData->getPosition() - 1)){
                TR = pOHLCV->getField("High")->getValue(pData->getPosition()) -
                        pOHLCV->getField("Close")->getValue(pData->getPosition() - 1);
            }
            if(TR < pOHLCV->getField("Close")->getValue(pData->getPosition() - 1) -
                    pOHLCV->getField("Low")->getValue(pData->getPosition())){
                TR = pOHLCV->getField("Close")->getValue(pData->getPosition() - 1) -
                        pOHLCV->getField("Low")->getValue(pData->getPosition());
            }
            BPSum1 += BP;
            TRSum1 += TR;
            pData->MoveNext();
        }//Period

        pData->setPosition(pData->getPosition() - Cycle2);
        for(Period = 1; Period < Cycle2 + 1; Period++){
            if(pOHLCV->getField("Low")->getValue(pData->getPosition()) <
                    pOHLCV->getField("Close")->getValue(pData->getPosition() - 1)){
                TL = pOHLCV->getField("Low")->getValue(pData->getPosition());
            }
            else{
                TL = pOHLCV->getField("Close")->getValue(pData->getPosition() - 1);
            }
            BP = pOHLCV->getField("Close")->getValue(pData->getPosition()) - TL;
            TR = pOHLCV->getField("High")->getValue(pData->getPosition()) -
                    pOHLCV->getField("Low")->getValue(pData->getPosition());
            if(TR < pOHLCV->getField("High")->getValue(pData->getPosition()) -
                    pOHLCV->getField("Close")->getValue(pData->getPosition() - 1)){
                TR = pOHLCV->getField("High")->getValue(pData->getPosition()) -
                        pOHLCV->getField("Close")->getValue(pData->getPosition() - 1);
            }
            if(TR < pOHLCV->getField("Close")->getValue(pData->getPosition() - 1) -
                    pOHLCV->getField("Low")->getValue(pData->getPosition())){
                TR = pOHLCV->getField("Close")->getValue(pData->getPosition() - 1) -
                        pOHLCV->getField("Low")->getValue(pData->getPosition());
            }
            BPSum2 += BP;
            TRSum2 += TR;
            pData->MoveNext();
        } //Period

        pData->setPosition(pData->getPosition() - Cycle3);
        for(Period = 1; Period < Cycle3 + 1; Period++){
            if(pOHLCV->getField("Low")->getValue(pData->getPosition()) <
                    pOHLCV->getField("Close")->getValue(pData->getPosition() - 1)){
                TL = pOHLCV->getField("Low")->getValue(pData->getPosition());
            }
            else{
                TL = pOHLCV->getField("Close")->getValue(pData->getPosition() - 1);
            }
            BP = pOHLCV->getField("Close")->getValue(pData->getPosition()) - TL;
            TR = pOHLCV->getField("High")->getValue(pData->getPosition()) -
                    pOHLCV->getField("Low")->getValue(pData->getPosition());
            if(TR < pOHLCV->getField("High")->getValue(pData->getPosition()) -
                    pOHLCV->getField("Close")->getValue(pData->getPosition() - 1)){
                TR = pOHLCV->getField("High")->getValue(pData->getPosition()) -
                        pOHLCV->getField("Close")->getValue(pData->getPosition() - 1);
            }
            if(TR < pOHLCV->getField("Close")->getValue(pData->getPosition() - 1) -
                    pOHLCV->getField("Low")->getValue(pData->getPosition())){
                TR = pOHLCV->getField("Close")->getValue(pData->getPosition() - 1) -
                        pOHLCV->getField("Low")->getValue(pData->getPosition());
            }
            BPSum3 += BP;
            TRSum3 += TR;
            pData->MoveNext();
        }//Period

        pData->MovePrevious();
        Value = (4 * (BPSum1 / TRSum1) + 2 * (BPSum2 / TRSum2) +
        (BPSum3 / TRSum3)) / (4 + 2 + 1) * 100;
        Field1->setValue(pData->getPosition(), Value);
        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* COscillator::VerticalHorizontalFilter(CDatabase* pData, CField* pSource,
         int Periods, const char* Alias){

    CField* Field1;
    CRecordset* Results = new CRecordset();
    int Record = 0;
    int RecordCount = 0;
    int Period = 0;
    int Start = 0;
    double HCP = 0;
    double LCP = 0;
    double Sum = 0;
	double Abs = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Start = Periods + 2;
    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 2; Record++){

        HCP = 0;
        LCP = pSource->getValue(pData->getPosition());

        pData->setPosition(Record - Periods);

        for(Period = 1; Period < Periods + 1; Period++){
            if(pSource->getValue(pData->getPosition()) < LCP){
                LCP = pSource->getValue(pData->getPosition());
            }
            else if(pSource->getValue(pData->getPosition()) > HCP){
                HCP = pSource->getValue(pData->getPosition());
            }
            pData->MoveNext();
        }//Period

        Sum = 0;
        pData->setPosition(Record - Periods);
        for(Period = 1; Period < Periods + 1; Period++){
			Abs = (pSource->getValue(pData->getPosition()) - 
				pSource->getValue(pData->getPosition() - 1));
			if(Abs < 0){Abs = -1 * Abs;}
            Sum += Abs;
            pData->MoveNext();
        }//Period

        pData->MovePrevious();
		Abs = (HCP - LCP) / Sum;
		if(Abs < 0){Abs = -1 * Abs;}
        Field1->setValue(pData->getPosition(), Abs);
        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* COscillator::WilliamsPctR(CDatabase* pData,
          CRecordset* pOHLCV, int Periods, const char* Alias){

    CField* Field1;
    CRecordset* Results = new CRecordset();
    int Record = 0;
    int RecordCount = 0;
    int Period = 0;
    int Start = 0;
    double HH = 0;
    double LL = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Start = Periods + 2;
    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 2; Record++){

        HH = 0;
        LL = pOHLCV->getField("Low")->getValue(pData->getPosition());

        pData->setPosition (Record - Periods);

        for(Period = 1; Period < Periods + 1; Period++){
            if(pOHLCV->getField("High")->getValue(pData->getPosition()) > HH){
               HH = pOHLCV->getField("High")->getValue(pData->getPosition());
            }
            if(pOHLCV->getField("Low")->getValue(pData->getPosition()) < LL){
               LL = pOHLCV->getField("Low")->getValue(pData->getPosition());
            }
            pData->MoveNext();
        }//Period

        pData->MovePrevious();
        Field1->setValue(pData->getPosition(),
        ((HH - pOHLCV->getField("Close")->getValue(pData->getPosition())) / (HH - LL)) * -100);
        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* COscillator::WilliamsAccumulationDistribution(CDatabase* pData, 
				CRecordset* pOHLCV, const char* Alias){

    CRecordset* Results = new CRecordset();
    CField* Field1;
    int RecordCount = 0;
    int Record = 0;
    int Start = 0;
    double TRH = 0;
    double TRL = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Start = 2;
    pData->setPosition(Start);

    for(Record = 1; Record < RecordCount; Record++){

        TRH = pOHLCV->getField("Close")->getValue(pData->getPosition() - 1);
        if(pOHLCV->getField("High")->getValue(pData->getPosition()) > TRH){
            TRH = pOHLCV->getField("High")->getValue(pData->getPosition());
        }

        TRL = pOHLCV->getField("Close")->getValue(pData->getPosition() - 1);
        if(pOHLCV->getField("Low")->getValue(pData->getPosition()) < TRL){
            TRL = pOHLCV->getField("Low")->getValue(pData->getPosition());
        }

        if(pOHLCV->getField("Close")->getValue(pData->getPosition()) >
                pOHLCV->getField("Close")->getValue(pData->getPosition() - 1)){
            Value = pOHLCV->getField("Close")->getValue(pData->getPosition()) - TRL;
        }
        else if(pOHLCV->getField("Close")->getValue(pData->getPosition()) <
                    pOHLCV->getField("Close")->getValue(pData->getPosition() - 1)){
            Value = pOHLCV->getField("Close")->getValue(pData->getPosition()) - TRH;
        }
        else{
            Value = 0;
        }

        Field1->setValue(pData->getPosition(), Value +
                Field1->getValue(pData->getPosition() - 1));

        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* COscillator::VolumeOscillator(CDatabase* pData, CField* Volume,
            int ShortTerm, int LongTerm,
            int PointsOrPercent, const char* Alias){

    CField* Field1;
    CMovingAverage* MA = new CMovingAverage();
    CRecordset* Results = new CRecordset();
    CRecordset* MA1;
    CRecordset* MA2;
    int Record = 0;
    int RecordCount = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    MA1 = MA->SimpleMovingAverage(pData, Volume, ShortTerm, "MA1");
    MA2 = MA->SimpleMovingAverage(pData, Volume, LongTerm, "MA2");
    
	for(Record = 1; Record < RecordCount + 1; Record++){

        if(PointsOrPercent == 1){
            Value = MA1->getField("MA1")->getValue(pData->getPosition()) -
                    MA2->getField("MA2")->getValue(pData->getPosition());
        }
        else if(PointsOrPercent == 2){
            if(MA2->getField("MA2")->getValue(pData->getPosition()) > 0){
                Value = ((MA1->getField("MA1")->getValue(pData->getPosition()) -
                        MA2->getField("MA2")->getValue(pData->getPosition())) /
                        MA2->getField("MA2")->getValue(pData->getPosition())) * 100;
            }
        }

        Field1->setValue(Record, Value);
        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);

	delete MA;
	delete MA1;
	delete MA2;

    return Results;

  }

  CRecordset* COscillator::ChaikinVolatility(CDatabase* pData, CRecordset* pOHLCV,
            int Periods, int ROC, int MAType, const char* Alias){

    CField* Field1;
    CMovingAverage* MA = new CMovingAverage();
    CRecordset* Results = new CRecordset();
    CRecordset* HLMA;
    CField* HL;
    int Record = 0;
    int RecordCount = 0;
    int Start = 0;
    double Value = 0;
    double MA1 = 0;
    double MA2 = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    HL = new CField(RecordCount, "HL");

    pData->MoveFirst();
    for(Record = 1; Record < RecordCount + 1; Record++){
        HL->setValue(pData->getPosition(), pOHLCV->getField("High")->getValue(pData->getPosition()) -
        pOHLCV->getField("Low")->getValue(pData->getPosition()));
        pData->MoveNext();
    }

    switch(MAType){
    case indSimpleMovingAverage:
        HLMA = MA->SimpleMovingAverage(pData, HL, Periods, "HLMA");
        break;
    case indExponentialMovingAverage:
        HLMA = MA->ExponentialMovingAverage(pData, HL, Periods, "HLMA");
        break;
    case indTimeSeriesMovingAverage:
        HLMA = MA->TimeSeriesMovingAverage(pData, HL, Periods, "HLMA");
        break;
    case indTriangularMovingAverage:
        HLMA = MA->TriangularMovingAverage(pData, HL, Periods, "HLMA");
        break;
    case indVariableMovingAverage:
        HLMA = MA->VariableMovingAverage(pData, HL, Periods, "HLMA");
        break;
    case indWeightedMovingAverage:
        HLMA = MA->WeightedMovingAverage(pData, HL, Periods, "HLMA");
        break;
    case indVIDYA:
        HLMA = MA->VIDYA(pData, HL, Periods, 0.65, "HLMA");
        break;
    }

    Start = ROC + 1;
    pData->setPosition(Start);
    for(Record = Start; Record < RecordCount + 1; Record++){
        MA1 = HLMA->getField("HLMA")->getValue(pData->getPosition() - ROC);
        MA2 = HLMA->getField("HLMA")->getValue(pData->getPosition());
        if(MA1 != 0 && MA2 != 0){Value = ((MA1 - MA2) / MA1) * -100;}
        Field1->setValue(Record, Value);
        pData->MoveNext();
    }//Record

    pData->MoveFirst();
	
	delete HL;
	delete MA;
	delete HLMA;

    Results->addField(Field1);
    return Results;

  }

  CRecordset* COscillator::StochasticOscillator(CDatabase* pData, CRecordset* pOHLCV,
            int KPeriods, int KSlowingPeriods, int DPeriods, int MAType){

    CMovingAverage* MA = new CMovingAverage();
    CField* Field1;
    CRecordset* PctD;	
    CRecordset* Results = new CRecordset();
    int Record = 0;
    int RecordCount = 0;
    int Period = 0;
    int Start = 0;
    double LL = 0;
    double HH = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, "%K");

    Start = KPeriods + 2;
    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 2; Record++){

        pData->setPosition(Record - KPeriods);
        HH = pOHLCV->getField("High")->getValue(pData->getPosition());
        LL = pOHLCV->getField("Low")->getValue(pData->getPosition());

        for(Period = 1; Period < KPeriods + 1; Period++){

            if(pOHLCV->getField("High")->getValue(pData->getPosition()) > HH){
                HH = pOHLCV->getField("High")->getValue(pData->getPosition());
            }

            if(pOHLCV->getField("Low")->getValue(pData->getPosition()) < LL){
                LL = pOHLCV->getField("Low")->getValue(pData->getPosition());
            }

            pData->MoveNext();

        } //Period

        pData->MovePrevious();
        Value = (pOHLCV->getField("Close")->getValue(pData->getPosition()) - LL) /
                (HH - LL) * 100;
        Field1->setValue(pData->getPosition(), Value);
        pData->MoveNext();

    }//Record

    if(KSlowingPeriods > 1){
		delete Results;
        switch(MAType){
        case indSimpleMovingAverage:
            Results = MA->SimpleMovingAverage(pData, Field1, KSlowingPeriods, "%K");
            break;
        case indExponentialMovingAverage:
            Results = MA->ExponentialMovingAverage(pData, Field1, KSlowingPeriods, "%K");
            break;
        case indTimeSeriesMovingAverage:
            Results = MA->TimeSeriesMovingAverage(pData, Field1, KSlowingPeriods, "%K");
        case indTriangularMovingAverage:
            Results = MA->TriangularMovingAverage(pData, Field1, KSlowingPeriods, "%K");
            break;
        case indVariableMovingAverage:
            Results = MA->VariableMovingAverage(pData, Field1, KSlowingPeriods, "%K");
            break;
        case indWeightedMovingAverage:
            Results = MA->WeightedMovingAverage(pData, Field1, KSlowingPeriods, "%K");
            break;
        case indVIDYA:
            Results = MA->VIDYA(pData, Field1, KSlowingPeriods, 0.65, "%K");
            break;
        }
    }
    else{
        Results->addField(Field1);
    }

	delete Field1;
    Field1 = Results->getField("%K");

     switch(MAType){
        case indSimpleMovingAverage:
            PctD = MA->SimpleMovingAverage(pData, Field1, DPeriods, "%D");
            break;
        case indExponentialMovingAverage:
            PctD = MA->ExponentialMovingAverage(pData, Field1, DPeriods, "%D");
            break;
        case indTimeSeriesMovingAverage:
            PctD = MA->TimeSeriesMovingAverage(pData, Field1, DPeriods, "%D");
            break;
        case indTriangularMovingAverage:
            PctD = MA->TriangularMovingAverage(pData, Field1, DPeriods, "%D");
            break;
        case indVariableMovingAverage:
            PctD = MA->VariableMovingAverage(pData, Field1, DPeriods, "%D");
            break;
        case indWeightedMovingAverage:
            PctD = MA->WeightedMovingAverage(pData, Field1, DPeriods, "%D");
            break;
        case indVIDYA:
            PctD = MA->VIDYA(pData, Field1, DPeriods, 0.65, "%D");
            break;
        }

    pData->MoveFirst();

    Results->addField(PctD->getField("%D"));	

	PctD->removeField("%D");
	delete PctD;	
	delete MA;

    return Results;

  }

  CRecordset* COscillator::PriceOscillator(CDatabase* pData, CField* pSource,
            int LongCycle, int ShortCycle, int MAType, const char* Alias){

	TASDK* TASDK1 = new TASDK();
    CRecordset* Results = new CRecordset();
    CField* Field1;
    CMovingAverage* MA = new CMovingAverage();
    CRecordset* LongMA;
    CRecordset* ShortMA;
    int RecordCount = 0;
    int Record = 0;
    int Start = 0;
    double Value = 0;

    if(LongCycle <= ShortCycle){
        std::cout << ("ShortCycle must be less than LongCycle");
        return Results;
    }

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    switch(MAType){
    case indSimpleMovingAverage:
        LongMA = MA->SimpleMovingAverage(pData, pSource, LongCycle, "MA");
        ShortMA = MA->SimpleMovingAverage(pData, pSource, ShortCycle, "MA");
        break;
    case indExponentialMovingAverage:
        LongMA = MA->ExponentialMovingAverage(pData, pSource, LongCycle, "MA");
        ShortMA = MA->ExponentialMovingAverage(pData, pSource, ShortCycle, "MA");
        break;
    case indTimeSeriesMovingAverage:
        LongMA = MA->TimeSeriesMovingAverage(pData, pSource, LongCycle, "MA");
        ShortMA = MA->TimeSeriesMovingAverage(pData, pSource, ShortCycle, "MA");
        break;
    case indTriangularMovingAverage:
        LongMA = MA->TriangularMovingAverage(pData, pSource, LongCycle, "MA");
        ShortMA = MA->TriangularMovingAverage(pData, pSource, ShortCycle, "MA");
        break;
    case indVariableMovingAverage:
        LongMA = MA->VariableMovingAverage(pData, pSource, LongCycle, "MA");
        ShortMA = MA->VariableMovingAverage(pData, pSource, ShortCycle, "MA");
        break;
    case indWeightedMovingAverage:
        LongMA = MA->WeightedMovingAverage(pData, pSource, LongCycle, "MA");
        ShortMA = MA->WeightedMovingAverage(pData, pSource, ShortCycle, "MA");
        break;
    case indVIDYA:
        LongMA = MA->VIDYA(pData, pSource, LongCycle, 0.65, "MA");
        ShortMA = MA->VIDYA(pData, pSource, ShortCycle, 0.65, "MA");
        break;
    }

    Start = TASDK1->maxVal(LongCycle, ShortCycle) + 1;

    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 1; Record++){

        Value = ((ShortMA->getValue("MA", pData->getPosition()) -
        LongMA->getValue("MA", pData->getPosition())) /
        LongMA->getValue("MA", pData->getPosition())) * 100;
        Field1->setValue(pData->getPosition(), Value);
        pData->MoveNext();

    }//Record

    pData->MoveFirst();

	delete TASDK1;
    delete MA;    
    delete LongMA;
    delete ShortMA;    

    Results->addField(Field1);
    return Results;

  }

  CRecordset* COscillator::MACD(CDatabase* pData, CField* pSource, int SignalPeriods, int LongCycle, int ShortCycle, const char* Alias)
  {
    CRecordset* Results = new CRecordset();
    CMovingAverage* MA = new CMovingAverage();
    CRecordset* EMA1;
    CRecordset* EMA2;
    int RecordCount = 0;
    int Period = 0;
    double macd = 0;

    RecordCount = pData->getRecordCount();

	CField* macdDatas = new CField(RecordCount, Alias);

	EMA1 = MA->ExponentialMovingAverage(pData, pSource, LongCycle, "EMA");
	EMA2 = MA->ExponentialMovingAverage(pData, pSource, ShortCycle, "EMA");

	//int start = SignalPeriods + 1;
 //   pData->setPosition(start);

 //   for(int Record = start; Record < RecordCount + 1; Record++)
	//{
 //       macd = EMA2->getValue("EMA", pData->getPosition()) - EMA1->getValue("EMA", pData->getPosition());
 //       macdDatas->setValue(pData->getPosition(), macd);
 //       pData->MoveNext();
 //   }

	int start = LongCycle;
	for (int index = start; index < RecordCount + 1; index++)
	{
		macd = EMA2->getValue("EMA", index) - EMA1->getValue("EMA", index);
		macdDatas->setValue(index, macd);
	}

	delete EMA1;
    EMA1 = MA->ExponentialMovingAverage(pData, macdDatas, SignalPeriods, "Signal");
	CField* signalDatas = EMA1->getField("Signal");
	EMA1->removeField("Signal");

	pData->MoveFirst();

    Results->addField(macdDatas);
    Results->addField(signalDatas);
	
	delete MA;
	delete EMA1;
	delete EMA2;

    return Results;

  }

  CRecordset* COscillator::EaseOfMovement(CDatabase* pData, CRecordset* pOHLCV,
            int Periods, int MAType, const char* Alias){

    CMovingAverage* MA = new CMovingAverage();
    CRecordset* Results = new CRecordset();
    CRecordset* EMVMA;
    CField* Field1;
    int RecordCount = 0;
    int Record = 0;
    int Start = 0;
    double MPM = 0;
    double EMV = 0;
    double BoxRatio = 0;
	double bd = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Start = 2;
    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 1; Record++){

        MPM = ((pOHLCV->getValue("High", pData->getPosition()) +
        pOHLCV->getValue("Low", pData->getPosition())) / 2) -
        ((pOHLCV->getValue("High", pData->getPosition() - 1) +
        pOHLCV->getValue("Low", pData->getPosition() - 1)) / 2);

        bd = (pOHLCV->getValue("High", pData->getPosition()) -
        pOHLCV->getValue("Low", pData->getPosition()));
			
		if(bd != 0){
			BoxRatio = pOHLCV->getValue("Volume", pData->getPosition()) / bd;
        }

        EMV = MPM / BoxRatio;

        Field1->setValue(pData->getPosition(), EMV * 10000);

        pData->MoveNext();

    }//Record

    switch(MAType){
    case indSimpleMovingAverage:
        EMVMA = MA->SimpleMovingAverage(pData, Field1, Periods, "MA");
        break;
    case indExponentialMovingAverage:
        EMVMA = MA->ExponentialMovingAverage(pData, Field1, Periods, "MA");
        break;
    case indTimeSeriesMovingAverage:
        EMVMA = MA->TimeSeriesMovingAverage(pData, Field1, Periods, "MA");
        break;
    case indTriangularMovingAverage:
        EMVMA = MA->TriangularMovingAverage(pData, Field1, Periods, "MA");
        break;
    case indVariableMovingAverage:
        EMVMA = MA->VariableMovingAverage(pData, Field1, Periods, "MA");
        break;
    case indWeightedMovingAverage:
        EMVMA = MA->WeightedMovingAverage(pData, Field1, Periods, "MA");
        break;
    case indVIDYA:
        EMVMA = MA->VIDYA(pData, Field1, Periods, 0.65, "MA");
        break;
    }

	delete Field1;
    Field1 = EMVMA->getField("MA");
    Field1->setName(Alias);

    pData->MoveFirst();

	EMVMA->removeField(Alias);
	delete EMVMA;
	delete MA;

    Results->addField(Field1);
    return Results;

  }

  CRecordset* COscillator::DetrendedPriceOscillator(CDatabase* pData, CField* pSource,
            int Periods, int MAType, const char* Alias){

    CMovingAverage* MA = new CMovingAverage();
    CRecordset* Results = new CRecordset();
    CRecordset* DPOMA;
    CField* Field1;
    int RecordCount = 0;
    int Record = 0;
    int Start = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    switch(MAType){
    case indSimpleMovingAverage:
        DPOMA = MA->SimpleMovingAverage(pData, pSource, Periods, "MA");
        break;
    case indExponentialMovingAverage:
        DPOMA = MA->ExponentialMovingAverage(pData, pSource, Periods, "MA");
        break;
    case indTimeSeriesMovingAverage:
        DPOMA = MA->TimeSeriesMovingAverage(pData, pSource, Periods, "MA");
        break;
    case indTriangularMovingAverage:
        DPOMA = MA->TriangularMovingAverage(pData, pSource, Periods, "MA");
    case indVariableMovingAverage:
        DPOMA = MA->VariableMovingAverage(pData, pSource, Periods, "MA");
        break;
    case indWeightedMovingAverage:
        DPOMA = MA->WeightedMovingAverage(pData, pSource, Periods, "MA");
        break;
    case indVIDYA:
        DPOMA = MA->VIDYA(pData, pSource, Periods, 0.65, "MA");
        break;
    }

    Start = Periods + 1;
    pData->setPosition(Start);

    for(Record = Start; Record < RecordCount + 1; Record++){
        Field1->setValue(pData->getPosition(), pSource->getValue(pData->getPosition()) -
        DPOMA->getValue("MA", pData->getPosition() - ((Periods / 2) + 1)));
        pData->MoveNext();
    }//Record

    pData->MoveFirst();
	
	delete DPOMA;
	delete MA;
    
	Results->addField(Field1);
    return Results;

  }

  CRecordset* COscillator::ParabolicSAR(CDatabase* pData, CField* HighPrice,
            CField* LowPrice, double MinAF, double MaxAF, const char* Alias){

    CRecordset* Results = new CRecordset();
    CField* Field1;

    int RecordCount = 0;
    int Record = 0;
    int Period = 0;
	int Start = 0;
	int Position = 0;
    double Max  = 0;
    double Min  = 0;
    double pSAR = 0;
    double pEP = 0;
    double pAF = 0;
    double SAR = 0;
    double AF = 0;
    double Hi = 0;
    double Lo = 0;
    double pHi = 0;
    double pLo = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Start = 2;
    pData->setPosition(Start);

    Max = HighPrice->getValue(1);
    Min = LowPrice->getValue(1);
    
    if(HighPrice->getValue(2) - HighPrice->getValue(1) < 
		LowPrice->getValue(2) - LowPrice->getValue(1)){
        pSAR = Max;
        Position = -1;
    }
	else{
        pSAR = Min;
        Position = 1;
    }

    pAF = MinAF;
    SAR = pSAR;
    Hi = Max;
    Lo = Min;
    pHi = Hi;
    pLo = Lo;
    AF = MinAF;

    for(Record = Start; Record < RecordCount + 1; ++Record){

		if(Position == 1){

			if(HighPrice->getValue(Record) > Hi){
                Hi = HighPrice->getValue(Record);
				if(AF < MaxAF) AF = AF + MinAF;
			}
                SAR = pSAR + pAF * (pHi - pSAR);
                if(LowPrice->getValue(Record) < SAR){
                    Position = -1;
                    AF = MinAF;
                    SAR = pHi;
                    Hi = 0;
                    Lo = LowPrice->getValue(Record);
                }

		}
		else if(Position == -1){
			if(LowPrice->getValue(Record) < Lo){
                Lo = LowPrice->getValue(Record);
				if(AF < MaxAF) AF = AF + MinAF;
			}
				SAR = pSAR + pAF * (pLo - pSAR);
                if(HighPrice->getValue(Record) > SAR){
                    Position = 1;
                    AF = MinAF;
                    SAR = pLo;
                    Lo = 0;
                    Hi = HighPrice->getValue(Record);
				}
		}

        pHi = Hi;
        pLo = Lo;
        pSAR = SAR;
        pAF = AF;
            
        Field1->setValue(Record, SAR);

        pData->MoveNext();
        

    } // Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

}

CRecordset* COscillator::DirectionalMovementSystem(CDatabase* pData, 
												   CRecordset* pOHLCV, int Periods){

	CMovingAverage* MA = new CMovingAverage();

    CRecordset* Results = new CRecordset();
    CRecordset* ADX = new CRecordset();

    CField* DX;
    CField* ADXR;
    CField* ADXF;
    CField* UpDMI;
    CField* DnDMI;
    CField* DIN;
    CField* DIP;
    CField* TR;
    
	CRecordset* wSumTR;
    CRecordset* wSumUpDMI;
    CRecordset* wSumDnDMI;
	CRecordset* rsTemp;
    
	double HDIF  = 0;
    double LDIF  = 0;

    int RecordCount = 0;
    int Record = 0;
	int Period = 0;
	double Value = 0;
    
	RecordCount = pData->getRecordCount();
        
	rsTemp = TrueRange(pData, pOHLCV, "TR");
	TR = rsTemp->getField("TR");
	rsTemp->removeField("TR");

    wSumTR = MA->WellesWilderSmoothing(TR, Periods, "TRSum");

    UpDMI = new CField(RecordCount, "UpDMI");
    DnDMI = new CField(RecordCount, "DnDMI");
    DIN = new CField(RecordCount, "DI-");
    DIP = new CField(RecordCount, "DI+");
    
	for(Record = 2; Record != RecordCount + 1; ++ Record){
        
        HDIF = pOHLCV->getValue("High", Record) - pOHLCV->getValue("High", Record - 1);
        LDIF = pOHLCV->getValue("Low", Record - 1) - pOHLCV->getValue("Low", Record);
        
        if((HDIF < 0 && LDIF < 0) || (HDIF == LDIF)){
            UpDMI->setValue(Record, 0);
            DnDMI->setValue(Record, 0);
		}
        else if(HDIF > LDIF){
            UpDMI->setValue(Record, HDIF);
            DnDMI->setValue(Record, 0);
		}
        else if(HDIF < LDIF){
            UpDMI->setValue(Record, 0);
            DnDMI->setValue(Record, LDIF);
        }

    } //Record
   
    wSumUpDMI = MA->WellesWilderSmoothing(UpDMI, Periods, "DM+Sum");
    wSumDnDMI = MA->WellesWilderSmoothing(DnDMI, Periods, "DM-Sum");
    
    for(Record = 2; Record != RecordCount + 1; ++ Record){
    
        DIP->setValue(Record, floor(100 * wSumUpDMI->getValue("DM+Sum", Record) / 
            wSumTR->getValue("TRSum", Record)));
        DIN->setValue(Record, floor(100 * wSumDnDMI->getValue("DM-Sum", Record) / 
            wSumTR->getValue("TRSum", Record)));

    } //Record
   
    DX = new CField(RecordCount, "DX");

	for(Record = 2; Record != RecordCount + 1; ++ Record){
    
		double a = abs(DIP->getValue(Record) - DIN->getValue(Record));
		double b = DIP->getValue(Record) + DIN->getValue(Record);
		if(a > 0 && b > 0){
			DX->setValue(Record, floor(100 * (a / b)));
		}

    } //Record
    
    ADXF = new CField(RecordCount, "ADX");
    ADX->addField(ADXF);

	for(Record = Periods + 1; Record != RecordCount + 1; ++Record){
        
        Value = floor(((ADX->getValue("ADX", Record - 1) * (Periods - 1)) + 
                DX->getValue(Record)) / Periods + 0.5);
        ADX->setValue("ADX", Record, Value);
    
    } //Record

    ADXR = new CField(RecordCount, "ADXR");
    
	for(Record = Periods + 1; Record != RecordCount + 1; ++Record){
		
        ADXR->setValue(Record, floor((ADX->getValue("ADX", Record) + 
                ADX->getValue("ADX", Record - 1)) / 2) + 0.5);
    
    } //Record
    
    pData->MoveFirst();	
       
    Results->addField(ADX->getField("ADX"));
    Results->addField(ADXR);
    Results->addField(DX);
    Results->addField(wSumTR->getField("TRSum"));
    Results->addField(DIN);
    Results->addField(DIP);

	ADX->removeField("ADX");
	ADX->removeField("ADXF");
	wSumTR->removeField("TRSum");	

	delete rsTemp;
	delete wSumTR;
    delete wSumUpDMI;
    delete wSumDnDMI;
	delete ADX;
	delete UpDMI;
	delete DnDMI;
	delete TR;
	delete MA;

    return Results;
        
}

  CRecordset* COscillator::TrueRange(CDatabase* pData, CRecordset* pOHLCV, const char* Alias){

    CRecordset* Results = new CRecordset();	
    CField* Field1;

    int RecordCount = 0;
    int Record = 0;
    int Period = 0;
	int Start = 0;
    double T1 = 0;
    double T2 = 0;
    double T3 = 0;
    double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Start = 2;
    pData->setPosition(Start);

	for(Record = 2; Record != RecordCount + 1; ++Record){

        T1 = pOHLCV->getValue("High", Record) - pOHLCV->getValue("Low", Record);
        T2 = abs( pOHLCV->getValue("High", Record) - pOHLCV->getValue("Close", Record - 1));
        T3 = abs(pOHLCV->getValue("Close", Record - 1) - pOHLCV->getValue("Low", Record));
        Value = 0;
        if(T1 > Value) Value = T1;
        if(T2 > Value) Value = T2;
        if(T3 > Value) Value = T3;
        
        Field1->setValue(Record, Value);
        
    } //Record

    Field1->setName(Alias);

    pData->MoveFirst();
    
	Results->addField(Field1);
    
	return Results;

  }

  CRecordset* COscillator::Aroon(CDatabase* pData, CRecordset* pOHLCV, int Periods){

    CRecordset* Results = new CRecordset();
	
	CField* AUp;
	CField* ADn;
	CField* AOs;

    int RecordCount = 0;
    int Record = 0;
    int Period = 0;
	
	double HighestHigh = 0;
    double LowestLow = 0;
	int HighPeriod = 0;
	int LowPeriod = 0;

    RecordCount = pData->getRecordCount();

    AUp = new CField(RecordCount, "Aroon Up");
	ADn = new CField(RecordCount, "Aroon Down");
	AOs = new CField(RecordCount, "Aroon Oscillator");

    for(Record = Periods + 1; Record != RecordCount + 1; ++Record){
    
        HighestHigh = pOHLCV->getValue("High", Record);
        LowestLow = pOHLCV->getValue("Low", Record);
        HighPeriod = Record;
        LowPeriod = Record;
        
        for(Period = Record - Periods; Period != Record; ++Period){
            
            if(pOHLCV->getValue("High", Period) > HighestHigh){
                HighestHigh = pOHLCV->getValue("High", Period);
                HighPeriod = Period;
            }
            
            if(pOHLCV->getValue("Low", Period) < LowestLow){
                LowestLow = pOHLCV->getValue("Low", Period);
                LowPeriod = Period;
            }
            
        } // Period
        
		AUp->setValue(Record, (double)(((double)Periods - (double)(Record - HighPeriod)) / Periods) * 100);
        ADn->setValue(Record, (double)(((double)Periods - (double)(Record - LowPeriod)) / Periods) * 100);
        
        AOs->setValue(Record, (AUp->getValue(Record) - ADn->getValue(Record)));
        
    } // Record
    
    Results->addField(AUp);
    Results->addField(ADn);
	Results->addField(AOs);
    
    return Results;
  
 }

 CRecordset* COscillator::RainbowOscillator(CDatabase* pData, 
	 CField* pSource, int Levels, int MAType, const char* Alias){

	CMovingAverage* MA = new CMovingAverage();

    CRecordset* Results = new CRecordset();
	CRecordset* rsMA;
	
	CField* Field1;

    int RecordCount = 0;
    int Record = 0;
	int Level = 0;
	double Value = 0;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);
	    
    for(Level = 2; Level != Levels + 1; ++Level){

        switch(MAType){
        case indSimpleMovingAverage:
            rsMA = MA->SimpleMovingAverage(pData, 
				pSource, Levels, "MA");
			break;
        case indExponentialMovingAverage:
            rsMA = MA->ExponentialMovingAverage(pData,
                    pSource, Levels, "MA");
			break;
        case indTimeSeriesMovingAverage:
            rsMA = MA->TimeSeriesMovingAverage(pData,
                    pSource, Levels, "MA");
			break;
        case indTriangularMovingAverage:
            rsMA = MA->TriangularMovingAverage(pData,
                    pSource, Levels, "MA");
			break;
        case indVariableMovingAverage:
            rsMA = MA->VariableMovingAverage(pData,
                    pSource, Levels, "MA");
			break;
        case indWeightedMovingAverage:
            rsMA = MA->WeightedMovingAverage(pData, 
                    pSource, Levels, "MA");
			break;
        case indVIDYA:
            rsMA = MA->VIDYA(pData, pSource,
                    Level, 0.65, "MA");
			break;
		}
        
        for(Record = 1; Record != RecordCount + 1; ++Record){
            Value = rsMA->getValue("MA", Record);
            Field1->setValue(Record, (pSource->getValue(Record) - Value) + 
				Field1->getValue(Record));
        } // Record
        
        delete rsMA;

    } // Level
    
    for(Record = 1; Record != RecordCount + 1; ++Record){
        Value = Field1->getValue(Record);
        Field1->setValue(Record, (Field1->getValue(Record) / (double)Levels));
    } // Record

	delete MA;

    Results->addField(Field1);
    return Results;

 }

   CRecordset* COscillator::FractalChaosOscillator(CDatabase* pData, CRecordset* pOHLCV,
            int Periods, const char* Alias){

    CRecordset* Results = new CRecordset();
        
    int RecordCount = pData->getRecordCount();
	int Record = 0;
    
    if(Periods < 1) Periods = 100;
    
    CField* fH = pOHLCV->getField("High");
    CField* fL = pOHLCV->getField("Low");
    CField* fFR = new CField(RecordCount, "FR");

    CField* fH1 = new CField(RecordCount, "High 1");
    CField* fH2 = new CField(RecordCount, "High 2");
    CField* fH3 = new CField(RecordCount, "High 3");
    CField* fH4 = new CField(RecordCount, "High 4");

    CField* fL1 = new CField(RecordCount, "Low 1");
    CField* fL2 = new CField(RecordCount, "Low 2");
    CField* fL3 = new CField(RecordCount, "Low 3");
    CField* fL4 = new CField(RecordCount, "Low 4");
    
	for(Record = 5; Record < RecordCount + 1; ++Record){    
    
        fH1->setValue(Record, fH->getValue(Record - 4));
        fL1->setValue(Record, fL->getValue(Record - 4));
        
        fH2->setValue(Record, fH->getValue(Record - 3));
        fL2->setValue(Record, fL->getValue(Record - 3));
        
        fH3->setValue(Record, fH->getValue(Record - 2));
        fL3->setValue(Record, fL->getValue(Record - 2));
        
        fH4->setValue(Record, fH->getValue(Record - 1));
        fL4->setValue(Record, fL->getValue(Record - 1));
        
    }

    for(Record = 2; Record < RecordCount + 1; ++Record){

        if((fH3->getValue(Record) > fH1->getValue(Record)) &&
            (fH3->getValue(Record) > fH2->getValue(Record)) &&
            (fH3->getValue(Record) >= fH4->getValue(Record)) &&
            (fH3->getValue(Record) >= fH->getValue(Record))){
            fFR->setValue(Record, 1);
        }
    
        if((fL3->getValue(Record) < fL1->getValue(Record)) &&
            (fL3->getValue(Record) < fL2->getValue(Record)) &&
            (fL3->getValue(Record) <= fL4->getValue(Record)) &&
            (fL3->getValue(Record) <= fL->getValue(Record))){
            fFR->setValue(Record, -1);
        }
   
    }    

	fFR->setName(Alias);
    Results->addField(fFR);

	delete fH1;
	delete fH2;
	delete fH3;
	delete fH4;
	delete fL1;
	delete fL2;
	delete fL3;
	delete fL4;
    
    return Results;
    
}
