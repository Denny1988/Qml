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

CMovingAverage::CMovingAverage()
{
	
}

CMovingAverage::~CMovingAverage()
{

}

  CRecordset* CMovingAverage::SimpleMovingAverage(CDatabase* pData,
                   CField* pSource, int Periods, const char* Alias){

    double Avg = 0;
    int Period  = 0;
    int Record = 0;
    int RecordCount = 0;
    int Start = 0;
    CField* Field1;

    CRecordset* Results = new CRecordset();

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Start = Periods + 1;
    pData->setPosition(Start);

    //Loop through each record in recordset
    for(Record = Start; Record < RecordCount + 1; Record++){

        Avg = 0;

        //Loop backwards through each period
        for(Period = 1; Period < Periods + 1; Period++){
            Avg += pSource->getValue(pData->getPosition());
            pData->MovePrevious();
        }//Period

        //Jump forward to last position
        pData->setPosition(pData->getPosition() + Periods);

        //Calculate moving average
        Avg = Avg / Periods;
        Field1->setValue(pData->getPosition(), Avg);

        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CMovingAverage::ExponentialMovingAverage(CDatabase* pData, CField* pSource, int periods, const char* Alias)
  {
	/* refactor from C# */
	double dPrime = 0;
	int iRecord;

	CRecordset* Results = new CRecordset();

	int iRecordCount = pData->getRecordCount();
	CField* emaField = new CField(iRecordCount, Alias);

	double dExp = 2.0 / (periods + 1);

	// To prime the EMA, get an average for the first n periods
	for (iRecord = 1; iRecord < periods + 1; iRecord++)
		dPrime += pSource->getValue(iRecord);
	dPrime /= periods;

	double dValue = (pSource->getValue(iRecord) * (1 - dExp)) + (dPrime * dExp);
	emaField->setValue(periods, dValue);

	//Loop through each record in recordset
	for (iRecord = periods + 1; iRecord < iRecordCount + 1; iRecord++)
	{
		dValue = (emaField->getValue(iRecord - 1) * (1 - dExp)) + (pSource->getValue(iRecord) * dExp);
		emaField->setValue(iRecord, dValue);
	}

	Results->addField(emaField);
	return Results;
  }

  CRecordset* CMovingAverage::TimeSeriesMovingAverage(CDatabase* pData,
                   CField* pSource, int Periods, const char* Alias){

    CLinearRegression* LR = new CLinearRegression();    
    int Record = 0;
    int RecordCount = 0;
    double Value = 0;
    CField* Field1;
    CRecordset* Results;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Results = LR->Regression(pData, pSource, Periods);
    
    pData->MoveFirst();
    for(Record = pData->getPosition(); Record < RecordCount + 1; Record++){
        Value = Results->getValue("Forecast", pData->getPosition());
        Field1->setValue(pData->getPosition(), Value);
        pData->MoveNext();
    }

    pData->MoveFirst();
    Results->addField(Field1);

	delete LR;

    return Results;

  }

  CRecordset* CMovingAverage::VariableMovingAverage(CDatabase* pData,
                   CField* pSource, int Periods, const char* Alias){

	COscillator* OS = new COscillator();
    int Period  = 0;
    int Record = 0;
    int RecordCount = 0;
    int Start = 0;
    double CMO = 0;
    double VMA = 0;
    double PrevVMA = 0;
    double Price = 0;
    CField* Field1;

	CRecordset* Results;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    //CMO must be overwritten    
    Results = OS->ChandeMomentumOscillator(pData, pSource, 9, "CMO");

    Start = 2;
    pData->setPosition(Start);
    for(Record = Start; Record < RecordCount + 1; Record++){
        pData->MovePrevious();
        PrevVMA = Field1->getValue(pData->getPosition());
        pData->MoveNext();
        CMO = Results->getValue("CMO", pData->getPosition()) / 100;
        Price = pSource->getValue(pData->getPosition());
		if(CMO < 0){CMO = -1 * CMO;}
        VMA = (CMO * Price) + (1 - CMO) * PrevVMA;
        Field1->setValue(pData->getPosition(), VMA);
        pData->MoveNext();
    }//Record

    pData->MoveFirst();
    Results->addField(Field1);    

	delete OS;

    return Results;

  }

  CRecordset* CMovingAverage::TriangularMovingAverage(CDatabase* pData, CField* pSource,
          int Periods, const char* Alias){

    int Record = 0;
    int RecordCount = 0;
    int Start = 0;
    int Period = 0;
    double MA1 = 0;
    double MA2 = 0;
    double Avg = 0;
    CField* Field1;
    CField* Field2;
    CRecordset* Results = new CRecordset();

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, "MA1");
    Field2 = new CField(RecordCount, Alias);

    if ((Periods % 2) > 0 ){ //Odd number
        MA1 = (int) ((double) Periods / 2) + 1;
        MA2 = MA1;
    }
    else{ //Even number
        MA1 = (double)Periods / 2;
        MA2 = MA1 + 1;
    }

    Start = Periods + 1;
    pData->setPosition(Start);

    //Loop through each record in recordset
    for(Record = Start; Record < RecordCount + 1; Record++){

        Avg = 0;

        //Loop backwards through each period
        for(Period = 1; Period < MA1 + 1; Period++){
            Avg += pSource->getValue(pData->getPosition());
            pData->MovePrevious();
        }//Period

        //Jump forward to last position
        pData->setPosition(pData->getPosition() + (int)MA1);

        //Calculate moving average
        Avg = Avg / MA1;
        Field1->setValue(pData->getPosition(), Avg);

        pData->MoveNext();

    }//Record

    pData->setPosition(Start);

    //Loop through each record in recordset
    for(Record = Start; Record < RecordCount + 1; Record++){

        Avg = 0;

        //Loop backwards through each period
        for(Period = 1; Period < MA2 + 1; Period++){
            Avg += Field1->getValue(pData->getPosition());
            pData->MovePrevious();
        }//Period

        //Jump forward to last position
        pData->setPosition(pData->getPosition() + (int)MA2);

        //Calculate moving average
        Avg = Avg / MA2;
        Field2->setValue(pData->getPosition(), Avg);

        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field2);
	delete Field1;
    return Results;

  }

  CRecordset* CMovingAverage::WeightedMovingAverage(CDatabase* pData, CField* pSource, int Periods, const char* Alias){

    double Total = 0;
    double Weight = 0;
    int Period = 0;
    int Start = 0;
    int Record = 0;
    int RecordCount = 0;
    CField* Field1;
    CRecordset* Results = new CRecordset();

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    for(Period = 1; Period < Periods + 1; Period++){
      Weight += Period;
    }//Period

    Start = Periods + 1;
    pData->setPosition(Start);

    //Loop through each record in recordset
    for(Record = Start; Record < RecordCount + 1; Record++){

        Total = 0;

        //Loop backwards through each period
        for(Period = Periods; Period > 0; Period--){
            Total += Period * pSource->getValue(pData->getPosition());
            pData->MovePrevious();
        }//Period

        //Jump forward to last position
        pData->setPosition(pData->getPosition() + Periods);

        //Calculate moving average
        Total = Total / Weight;
        Field1->setValue(pData->getPosition(), Total);

        pData->MoveNext();

    }//Record

    pData->MoveFirst();
    Results->addField(Field1);
    return Results;

  }

  CRecordset* CMovingAverage::VIDYA(CDatabase* pData, CField* pSource,
            int Periods, double R2Scale, const char* Alias){

    bool CleanUp = false;
    int Record = 0;
    int RecordCount = 0;
    int Start = 0;
    double R2Scaled = 0;
    double PreviousValue = 0;
    CField* Field1;
    CLinearRegression* LR = new CLinearRegression();
    CRecordset* Results;

    RecordCount = pData->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    Results = LR->Regression(pData, pSource, Periods);
     
    Start = 2;
    pData->setPosition(Start);
    for(Record = Start; Record < RecordCount + 1; Record++){
        pData->MovePrevious();
        PreviousValue = pSource->getValue(pData->getPosition());
        pData->MoveNext();
        R2Scaled = Results->getValue("RSquared", pData->getPosition()) * R2Scale;
        Field1->setValue(pData->getPosition(), R2Scaled *
        pSource->getValue(pData->getPosition()) + (1 - R2Scaled) * PreviousValue);
        pData->MoveNext();
    }//Record

	delete LR;    
    Results->addField(Field1);
    return Results;

  }

CRecordset* CMovingAverage::WellesWilderSmoothing(CField* pSource, 
												 int Periods, const char* Alias){
	
	CField* Field1;
    CRecordset* Results = new CRecordset();

	int Record = 0;
    int RecordCount = 0;
    double Value = 0;    

    RecordCount = pSource->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

	for(Record = 2; Record < RecordCount + 1; ++Record)
	{
		if (Record == 2)
		{
			Value = pSource->getValue(Record);  // 避免初始值过小
		}
		else
		{
			Value = Field1->getValue(Record - 1) + 1 / (double)Periods * (pSource->getValue(Record) - Field1->getValue(Record - 1));
		}
        Field1->setValue(Record, Value);
    } //Record

    Results->addField(Field1);
    
	return Results;
    
}

