/**
 * Title:        TASDK
 * Description:  Technical Analysis Library
 * Copyright:    Copyright (c) 2002
 * Company:      Modulus Financial Engineering
 * @author		 Modulus FE
 * @version		 1.0
 */

#include "TASDK.h"

CCandleStick::CCandleStick()
{

}

CCandleStick::~CCandleStick()
{

}

  CNote* CCandleStick::IdentifyPattern(CRecordset* OHLCV, int Period){

	TASDK* TASDK1 = new TASDK();
	std::vector<BarType*> Bars(4);
	std::vector<CandleType*> Cndl(4);
    BarType* NBar = new BarType();
    CNote* RetVal = new CNote();
    CandleType* Candle1 = new CandleType();
    CandleType* Candle2 = new CandleType();
    CandleType* Candle3 = new CandleType();
    int N = 0;
    double Min = 0;
    double Max = 0;
	double HL[3][4];
    double R[4];
    double Value = 0;

	RetVal->setNote("NO PATTERN");

    if(Period < 3){
		//SetLastError((DWORD) 0x10030);
        //cout << "CandleStick->IdentifyPattern: EndPeriod must be greater than 3";
		return RetVal;
    }

    //Get 3 bars--------------------------------------------
    Bars[3] = new BarType();
    Bars[3]->OpenPrice = OHLCV->getValue("Open", Period);
    Bars[3]->HighPrice = OHLCV->getValue("High", Period);
    Bars[3]->LowPrice = OHLCV->getValue("Low", Period);
    Bars[3]->ClosePrice = OHLCV->getValue("Close", Period);

    Bars[2] = new BarType();
    Bars[2]->OpenPrice = OHLCV->getValue("Open", Period - 1);
    Bars[2]->HighPrice = OHLCV->getValue("High", Period - 1);
    Bars[2]->LowPrice = OHLCV->getValue("Low", Period - 1);
    Bars[2]->ClosePrice = OHLCV->getValue("Close", Period - 1);

    Bars[1] = new BarType();
    Bars[1]->OpenPrice = OHLCV->getValue("Open", Period - 2);
    Bars[1]->HighPrice = OHLCV->getValue("High", Period - 2);
    Bars[1]->LowPrice = OHLCV->getValue("Low", Period - 2);
    Bars[1]->ClosePrice = OHLCV->getValue("Close", Period - 2);

    for(N = 1; N < 4; N++){
        HL[1][N] = TASDK1->normalize(Bars[N]->HighPrice, Bars[N]->LowPrice, Bars[N]->OpenPrice);
        HL[2][N] = TASDK1->normalize(Bars[N]->HighPrice, Bars[N]->LowPrice, Bars[N]->ClosePrice);
    }//N

    //Get max and min then normalize for candle pattern

    Max = TASDK1->Abs(Bars[1]->HighPrice - Bars[1]->LowPrice);
    Min = TASDK1->Abs(Bars[1]->HighPrice - Bars[1]->LowPrice);

    for(N = 1; N < 4; N++){
        if(TASDK1->Abs(Bars[N]->HighPrice - Bars[N]->LowPrice) > Max){
                Max = TASDK1->Abs(Bars[N]->HighPrice - Bars[N]->LowPrice);
        }
        if(TASDK1->Abs(Bars[N]->HighPrice - Bars[N]->LowPrice) < Min){
                Min = TASDK1->Abs(Bars[N]->HighPrice - Bars[N]->LowPrice);
        }
    }//N

    for(N = 1; N < 4; N++){
        if(Max == Min){
            R[N] = 1;
        }
        else{
            R[N] = TASDK1->normalize(Max, Min, TASDK1->Abs(Bars[N]->HighPrice - Bars[N]->LowPrice));
        }
    }//N

    Max = Bars[1]->HighPrice;
    if(Bars[2]->HighPrice > Max){Max = Bars[2]->HighPrice;}
    if(Bars[3]->HighPrice > Max){Max = Bars[3]->HighPrice;}

    Min = Bars[1]->LowPrice;
    if(Bars[2]->LowPrice < Min){Min = Bars[2]->LowPrice;}
    if(Bars[3]->LowPrice < Min){Min = Bars[3]->LowPrice;}

    Bars[1]->OpenPrice = TASDK1->normalize(Max, Min, Bars[1]->OpenPrice);
    Bars[1]->HighPrice = TASDK1->normalize(Max, Min, Bars[1]->HighPrice);
    Bars[1]->LowPrice = TASDK1->normalize(Max, Min, Bars[1]->LowPrice);
    Bars[1]->ClosePrice = TASDK1->normalize(Max, Min, Bars[1]->ClosePrice);

    Bars[2]->OpenPrice = TASDK1->normalize(Max, Min, Bars[2]->OpenPrice);
    Bars[2]->HighPrice = TASDK1->normalize(Max, Min, Bars[2]->HighPrice);
    Bars[2]->LowPrice = TASDK1->normalize(Max, Min, Bars[2]->LowPrice);
    Bars[2]->ClosePrice = TASDK1->normalize(Max, Min, Bars[2]->ClosePrice);

    Bars[3]->OpenPrice = TASDK1->normalize(Max, Min, Bars[3]->OpenPrice);
    Bars[3]->HighPrice = TASDK1->normalize(Max, Min, Bars[3]->HighPrice);
    Bars[3]->LowPrice = TASDK1->normalize(Max, Min, Bars[3]->LowPrice);
    Bars[3]->ClosePrice = TASDK1->normalize(Max, Min, Bars[3]->ClosePrice);

    Cndl[1] = new CandleType();
    Cndl[2] = new CandleType();
    Cndl[3] = new CandleType();

    //Identify Bars
     for(N = 1; N < 4; N++){

        //Long body------------------------------------
        if(TASDK1->Abs(HL[1][N] - HL[2][N]) > 0.5 && R[N] >= 0.5){
            Cndl[N]->setName((char*)"Long Body");
            if(Bars[N]->ClosePrice > Bars[N]->OpenPrice){
                Cndl[N]->setBullBear(1);
            }
            else if(Bars[N]->ClosePrice < Bars[N]->OpenPrice){
                Cndl[N]->setBullBear(-1);
            }
        }

        //Doji------------------------------------------
        else if(TASDK1->Abs(HL[1][N] - HL[2][N]) < 0.2 && R[N] >= 0.5){
            Cndl[N]->setName((char*)"Doji");
            Cndl[N]->setBullBear(0);
        }

        //Hammer----------------------------------------
        else if(TASDK1->Abs(HL[1][N] - HL[2][N]) < 0.4 && ((HL[1][N] >
            0.65) || (HL[2][N] < 0.35))){
            if(HL[1][N] > 0.65){
                Cndl[N]->setName((char*)"Hammer");
                Cndl[N]->setBullBear(1);
            }
            else if(HL[2][N] < 0.35){
                Cndl[N]->setName((char*)"Hammer");
                Cndl[N]->setBullBear(-1);
            }
        }

        //Star-----------------------------------------
        else if(TASDK1->Abs(HL[1][N] - HL[2][N]) < 0.66 &&
            HL[1][N] < 0.8 && HL[2][N] > 0.2){
            if(TASDK1->Abs(HL[1][N] - HL[2][N]) < 0.1){
                Cndl[N]->setName((char*)"Doji Star");
                Cndl[N]->setBullBear(0);
                }
            else{
                Cndl[N]->setName((char*)"Star");
                Cndl[N]->setBullBear(0);
            }

        }

    }//N

    /*First, try to identify a pattern with 3 candles
    (Morning/Evening Star)-> If this pattern doesn't exist
    then look for patterns of 2 candles-> If none
    exist then look for a single candle pattern->*/

    //Morning Star-------------------------------------------------
    if(Cndl[1]->getName() == "Long Body" && Cndl[1]->getBullBear() == -1 &&
        (Cndl[2]->getName() == "Star" || Cndl[2]->getName() == "Doji Star" ||
        Cndl[2]->getName() == "Hammer") && Cndl[3]->getName() == "Long Body" &&
        Cndl[3]->getBullBear() == 1){
            RetVal->setNote("Morning Star");
            RetVal->setValue(1);
    }

    //Evening Star---------------------------------------------------
    else if(Cndl[1]->getName() == "Long Body" && Cndl[1]->getBullBear() == 1 &&
        (Cndl[2]->getName() == "Star" || Cndl[2]->getName() == "Doji Star" ||
        Cndl[2]->getName() == "Hammer") && Cndl[3]->getName() == "Long Body" &&
        Cndl[3]->getBullBear() == -1){
            RetVal->setNote("Evening Star");
            RetVal->setValue(-1);
    }

    //Piecering Line------------------------------------------------
    else if(Cndl[2]->getName() == "Long Body" && Cndl[2]->getBullBear() == -1 &&
         Cndl[3]->getName() == "Long Body" && Cndl[3]->getBullBear() == 1 &&
         Bars[3]->LowPrice < Bars[2]->LowPrice && Bars[3]->ClosePrice >=
         ((Bars[2]->HighPrice + Bars[2]->LowPrice) / 2)){
            RetVal->setNote("Piercing Line");
            RetVal->setValue(1);
    }

    //Bullish Engulfing Line----------------------------------------
    else if(Bars[3]->ClosePrice > TASDK1->maxVal(Bars[2]->ClosePrice, Bars[2]->OpenPrice) &&
         Bars[3]->LowPrice < TASDK1->minVal(Bars[2]->ClosePrice, Bars[2]->OpenPrice) &&
         Bars[2]->ClosePrice < Bars[2]->OpenPrice && Bars[3]->OpenPrice <
         Bars[3]->ClosePrice){
         RetVal->setNote("Engulfing Line");
         RetVal->setValue(1);
    }

    //Bullish Doji Star---------------------------------------------
    else if(Cndl[2]->getName() == "Long Body" && Cndl[3]->getName() == "Doji Star" &&
         TASDK1->minVal(Bars[2]->ClosePrice, Bars[2]->OpenPrice) >
         TASDK1->maxVal(Bars[3]->ClosePrice, Bars[3]->OpenPrice)){
         RetVal->setNote("Doji Star");
         RetVal->setValue(1);
    }

    //Hanging Man---------------------------------------------------
    else if(Cndl[2]->getName() == "Hammer" && Cndl[3]->getName() == "Hammer" &&
         ((Cndl[2]->getBullBear() == -1 && Cndl[3]->getBullBear() == -1) ||
         (Cndl[2]->getBullBear() == 1 && Cndl[3]->getBullBear() == 1))){
         RetVal->setNote("Hanging Man");
         RetVal->setValue(-1);
    }

    //Dark Cloud----------------------------------------------------
    else if(TASDK1->maxVal(Bars[3]->ClosePrice, Bars[3]->OpenPrice) >
         TASDK1->maxVal(Bars[2]->ClosePrice, Bars[2]->OpenPrice) &&
         TASDK1->minVal(Bars[3]->ClosePrice, Bars[3]->OpenPrice) > Bars[2]->LowPrice){
         RetVal->setNote("Dark Cloud Cover");
         RetVal->setValue(-1);
    }

    //Bearish Engulfing Line----------------------------------------
    else if(Cndl[3]->getName() == "Long Body" && Cndl[3]->getBullBear() == -1 &&
         Bars[2]->LowPrice > TASDK1->minVal(Bars[3]->ClosePrice, Bars[3]->OpenPrice)){
         RetVal->setNote("Engulfing Line");
         RetVal->setValue(-1);
    }

    //Bearish Doji Star---------------------------------------------
    else if(Cndl[2]->getName() == "Long Body" && Cndl[3]->getName() == "Doji Star" &&
         TASDK1->minVal(Bars[2]->ClosePrice, Bars[2]->OpenPrice) <
         TASDK1->maxVal(Bars[3]->ClosePrice, Bars[3]->OpenPrice)){
         RetVal->setNote("Doji Star");
         RetVal->setValue(-1);
    }

    //Shooting Star-------------------------------------------------
    else if(Cndl[2]->getName() == "Long Body" && Cndl[3]->getName() == "Star" &&
         TASDK1->minVal(Bars[2]->ClosePrice, Bars[2]->OpenPrice) <
         TASDK1->maxVal(Bars[3]->ClosePrice, Bars[3]->OpenPrice)){
         RetVal->setNote("Shooting Star");
         RetVal->setValue(-1);
    }

    //Spinning Tops-------------------------------------------------
    else if(Cndl[2]->getName() == "Star" && Cndl[3]->getName() == "Star"){
         RetVal->setNote("Spinning Tops");
         RetVal->setValue(0);
    }

    //Harami Cross--------------------------------------------------
    else if(Cndl[2]->getName() == "Long Body" && Cndl[3]->getName() == "Doji Star" &&
         TASDK1->maxVal(Bars[2]->ClosePrice, Bars[2]->OpenPrice) > Bars[3]->HighPrice &&
         TASDK1->minVal(Bars[2]->ClosePrice, Bars[2]->OpenPrice) > Bars[3]->LowPrice){
         RetVal->setNote("Harami Cross");
         RetVal->setValue(-1);
    }

    if(RetVal->getNote() == "" || RetVal->getNote() == NULL){
        //No pattern so return last candle
        RetVal->setNote(Cndl[3]->getName());
        RetVal->setValue(Cndl[3]->getBullBear());
    }

	delete TASDK1;
	delete Candle1;
	delete Candle2;
	delete Candle3;

    return RetVal;

  }
