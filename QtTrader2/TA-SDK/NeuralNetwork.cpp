/**
 * Title:        TASDK
 * Description:  Technical Analysis Library
 * Copyright:    Copyright (c) 2002
 * Company:      Modulus Financial Engineering
 * @author		 Modulus FE
 * @version		 1.0
 */

#include "TASDK.h"
#include <time.h>

CNeuralNetwork::CNeuralNetwork()
{

}

CNeuralNetwork::~CNeuralNetwork()
{

}

  void CNeuralNetwork::initialize(){
	const double RndMax = 10000;
	double Rnd = 0;
	srand(time(NULL));
	Rnd = static_cast<double>(rand()) / RndMax;
    W13 = (Rnd  * -1) + 1;
	Rnd = static_cast<double>(rand()) / RndMax;
    W14 = (Rnd  * -1) + 1;
	Rnd = static_cast<double>(rand()) / RndMax;
    W15 = (Rnd  * -1) + 1;
	Rnd = static_cast<double>(rand()) / RndMax;
    W23 = (Rnd  * -1) + 1;
	Rnd = static_cast<double>(rand()) / RndMax;
    W24 = (Rnd  * -1) + 1;
	Rnd = static_cast<double>(rand()) / RndMax;
    W25 = (Rnd  * -1) + 1;
	Rnd = static_cast<double>(rand()) / RndMax;
    W36 = (Rnd  * -1) + 1;
	Rnd = static_cast<double>(rand()) / RndMax;
    W46 = (Rnd  * -1) + 1;
	Rnd = static_cast<double>(rand()) / RndMax;
    W56 = (Rnd  * -1) + 1;
	Rnd = static_cast<double>(rand()) / RndMax;
    B3 = (Rnd  * -1) + 1;
	Rnd = static_cast<double>(rand()) / RndMax;
    B4 = (Rnd  * -1) + 1;
	Rnd = static_cast<double>(rand()) / RndMax;
    B5 = (Rnd  * -1) + 1;
	Rnd = static_cast<double>(rand()) / RndMax;
    B6 = (Rnd  * -1) + 1;
	Rnd = static_cast<double>(rand()) / RndMax;
  }

  void CNeuralNetwork::train(double Input1, double Input2, double Target){

    double OutErr = 0;
    double HiddenErr = 0;

    feedForward(Input1, Input2);
    OutErr = (Target - Neuron6) * Neuron6 * (1 - Neuron6);
    W36 = W36 + m_LearningRate * OutErr * Neuron3;
    W46 = W46 + m_LearningRate * OutErr * Neuron4;
    W56 = W56 + m_LearningRate * OutErr * Neuron5;
    HiddenErr = Neuron3 * (1 - Neuron3) * OutErr * W36;
    W13 = W13 + m_LearningRate * HiddenErr * Neuron1;
    W23 = W23 + m_LearningRate * HiddenErr * Neuron2;
    HiddenErr = Neuron4 * (1 - Neuron4) * OutErr * W46;
    W14 = W14 + m_LearningRate * HiddenErr * Neuron1;
    W24 = W24 + m_LearningRate * HiddenErr * Neuron2;
    HiddenErr = Neuron5 * (1 - Neuron5) * OutErr * W56;
    W15 = W15 + m_LearningRate * HiddenErr * Neuron1;
    W25 = W25 + m_LearningRate * HiddenErr * Neuron2;

  }

  double CNeuralNetwork::feedForward(double Input1, double Input2){

    Neuron1 = Input1;
    Neuron2 = Input2;
    Neuron3 = activation((Neuron1 * W13) + (Neuron2 * W23) + B3);
    Neuron4 = activation((Neuron1 * W14) + (Neuron2 * W24) + B4);
    Neuron5 = activation((Neuron1 * W15) + (Neuron2 * W25) + B5);
    Neuron6 = activation((Neuron3 * W36) + (Neuron4 * W46) + (Neuron5 * W56) + B6);
    return Neuron6;

  }

  double CNeuralNetwork::activation(double Value){

	  return 1 / (1 + exp(Value * -1));

  }

  CRecordset* CNeuralNetwork::NeuralIndicator(CDatabase* Data, CField* Source,
      int Periods, double LearningRate, long Epochs,
      double PercentTrain, char* Alias){

	TASDK* TASDK1 = new TASDK();
    CRecordset* Results = new CRecordset();
    CField* Field1;
    CField* fInput1;
    CField* fInput2;
    CField* fTarget;
    CGeneral* G = new CGeneral();
    CNote* N = new CNote();
    int RecordCount = 0;
    int Record = 0;
    int TrainRecords = 0;
    int TestRecords = 0;
    int Epoch = 0;
    int Start = 0;
    double Input1 = 0;
    double Input2 = 0;
    double Target = 0;
    double Max = 0;
    double Min = 0;

    if(Alias == "" || Alias == NULL){
      Alias = (char*)"NeuralIndicator";
    }

    RecordCount = Data->getRecordCount();

    Field1 = new CField(RecordCount, Alias);

    if(PercentTrain > 98 || PercentTrain < 2){
		//SetLastError((DWORD) 0x10070);
        //cout << "NeuralIndicator: Invalid PercentTrain";
        return NULL;
    }

    //Divide Source into training set and forecast set
    double tr = (RecordCount * (PercentTrain / 100));
    TrainRecords = (int)tr;
    fInput1 = new CField(RecordCount, "Input1");
    fInput2 = new CField(RecordCount, "Input2");
    fTarget = new CField(RecordCount, "Target");

    //Build neural network data sets
    Start = Periods + 1;
    Data->setPosition(Start);
    for(Record = Start; Record < RecordCount + 1; Record++){

        Input1 = Source->getValue(Data->getPosition() - Periods);
        Input2 = Source->getValue(Data->getPosition());

        fInput1->setValue(Data->getPosition(), Input1);
        fInput2->setValue(Data->getPosition(), Input2);

        Data->MoveNext();

    } //Record

    //Training set target values
    Start = Periods + 1;
    Data->setPosition(Start);
    for(Record = Start; Record <  (TrainRecords - Periods) + 1; Record++){

        Target = Source->getValue(Data->getPosition()) -
        Source->getValue(Data->getPosition() + 1);

        fTarget->setValue(Data->getPosition(), Target);

        Data->MoveNext();

    }//Record

    //Normalize vectors
    N = G->MaxValue(fInput1, 1, RecordCount);
    Max = N->getValue();
    N = G->MinValue(fInput1, 1, RecordCount);
    Min = N->getValue();
    for(Record = 1; Record < RecordCount + 1; Record++){
        fInput1->setValue(Record, TASDK1->normalize(Max, Min, fInput1->getValue(Record)));
    }//Record

    N = G->MaxValue(fInput2, 1, RecordCount);
    Max = N->getValue();
    N = G->MinValue(fInput2, 1, RecordCount);
    Min = N->getValue();
    for(Record = 1; Record < RecordCount + 1; Record++){
        fInput2->setValue(Record, TASDK1->normalize(Max, Min, fInput2->getValue(Record)));
    }//Record

    N = G->MaxValue(fTarget, 1, RecordCount);
    Max = N->getValue();
    N = G->MinValue(fTarget, 1, RecordCount);
    Min = N->getValue();
    for(Record = 1; Record < TrainRecords + 1; Record++){
        fTarget->setValue(Record, TASDK1->normalize(Max, Min, fTarget->getValue(Record)));
    }//Record

    Start = TrainRecords;

    //Initialize neural network
    m_LearningRate = LearningRate;
    initialize();

    //Train neural network
    for(Epoch = 1; Epoch < Epochs + 1; Epoch++){
        for(Record = 1; Record < TrainRecords + 1; Record++){
            train(fInput1->getValue(Record),
            fInput2->getValue(Record),
            fTarget->getValue(Record));
        }//Record
    }//Epoch

    //Output neural network forecasts from TrainRecords + 1 to RecordCount
    for(Record = TrainRecords; Record < RecordCount + 1; Record++){
        Field1->setValue(Record, feedForward(fInput1->getValue(Record), fInput2->getValue(Record)));
    }//Record

    Data->MoveFirst();
    Results->addField(Field1);

	delete TASDK1;
	delete G;
	delete N;

    return Results;

  }
