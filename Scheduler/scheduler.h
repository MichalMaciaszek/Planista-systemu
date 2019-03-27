#pragma once
#include "Zarzadzanie_procesami.h"
#include <iostream>
#include <ctime>
#include <string>
#include <cmath>
#include <list>
#include <vector>
static PCB *Running;
static PCB* czekajacy;
static bool lockdoczekania = true;

class Scheduler {
public: 
	//static PCB *Running;
	
	int a;
	
	static const int InstructionsPerQuantum;
	static const int PriorityAmount;
	static const int QuantumAmountToBalance;
	static const int QuantumAmountToRunBalanceSetManager;
	static const int DefaultQuantumToGive;
	static short QuantumAmountCounter;
	static const bool IsExpropriated;
public:
	//static int VARIABLE_CLASS_THREAD_PRIORITY_TIME_CRITICAL;
	//static int VARIABLE_CLASS_THREAD_PRIORITY_NORMAL;
	//static int VARIABLE_CLASS_THREAD_PRIORITY_IDLE;
//	 int REALTIME_CLASS_THREAD_PRIORITY_TIME_CRITICAL;
	//static int REALTIME_CLASS_THREAD_PRIORITY_NORMAL;
	//static int REALTIME_CLASS_THREAD_PRIORITY_IDLE;
//static	int INITPROCESS_THREAD_PRIORITY = 0;
	
	//Scheduler(PCB* Init);
	Scheduler(PCB* Init);
	static void Wyswietlkolejke();
	static void addtoreadyList(PCB* p);
	static void Scheduler::usun(PCB* p);
	static void MaskaBitowa();
	static PCB* FindReadyThread();
	static void ReadyThread(PCB* process);
	static void IsReadyThreadExpropriating(PCB* process);
	static void BalanceSetManager();
	static void Go();
	static void ReadyThread(PCB* process, bool isItSemaphore);


static void	pomocustawlock(bool a);

};

