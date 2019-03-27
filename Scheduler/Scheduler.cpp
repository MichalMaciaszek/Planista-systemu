#include <iostream>
#include <ctime>
#include <string>
#include <cmath>
#include <list>
#include <vector>
#include "scheduler.h"
#include <iterator>

using namespace std;


int REALTIME_CLASS_THREAD_PRIORITY_TIME_CRITICAL;
int VARIABLE_CLASS_THREAD_PRIORITY_TIME_CRITICAL;
static int VARIABLE_CLASS_THREAD_PRIORITY_NORMAL;
static int VARIABLE_CLASS_THREAD_PRIORITY_IDLE;
//	 int REALTIME_CLASS_THREAD_PRIORITY_TIME_CRITICAL;
static int REALTIME_CLASS_THREAD_PRIORITY_NORMAL;
static int REALTIME_CLASS_THREAD_PRIORITY_IDLE;
static	int INITPROCESS_THREAD_PRIORITY = 0;
//PCB *Running;
static	bool KiReadySummary[8];

static vector<list<PCB*>> KiDispatcherReadyListHead;
bool czywywlaszczone=false;
/* Flaga wywlaszczenia procesu */
  PCB *InitProcess;
 



	/*
	* InstructionsPerQuantum - Maksymalna ilosc rozkazow do wykonania w kwancie czasu (4 rozkazy)
	*/
	const int  Scheduler::InstructionsPerQuantum = 4;
	/*
	* PriorityAmount - Ilosc priorytetow dopuszczalna dla planisty (wliczajac watek postojowy)
	*/
	const int Scheduler::PriorityAmount = 8;
	/*
	* QuantumAmountToBalance - Ilosc kwantow czasu po ktorych procesy powinny przestac byc glodzone
	*/
	const int Scheduler::QuantumAmountToBalance = 4;				//zmniejszone dla pokazu z 8
	/*
	* QuantumAmountToRunBalanceSetManager - Ilosc kwantow czasu po ktorych planista powinien uruchomic funkcje BalanceSetManager
	*/
	const int Scheduler::QuantumAmountToRunBalanceSetManager = 4;
	/*
	* DefaultQuantumToGive - Domyslna wartosc ilosci kwantow danych dla procesow przed przelaczeniem kontekstu (informacja dla Process Manager)
	*/
	const int Scheduler::DefaultQuantumToGive = 2;
	/*
	* QuantumAmountCounter - Licznik wykonanych kwantow procesora(glownie aby wiedziec kiedy uruchomic funkcje BalanceSetManager)
	*/
	short Scheduler::QuantumAmountCounter = 0;





	/*
	* KiReadySummary - Maska bitowa identyfikujaca niepuste kolejki w tablicy KiDispatcherReadyListHead (przyspieszenie wyszukiwania gotowego procesu na wzor Windows)
	*/


	
	void Scheduler::MaskaBitowa() {
		if (Scheduler::PriorityAmount > 0) {
			/* Utworzenie maski o ilosci bitow odpowiadajacej ilosci priorytetow */
			KiReadySummary[8] = new bool[Scheduler::PriorityAmount];
			/* Wyzerowanie bitow na masce*/
			for (auto iterator = REALTIME_CLASS_THREAD_PRIORITY_TIME_CRITICAL; iterator >=  VARIABLE_CLASS_THREAD_PRIORITY_IDLE; iterator--) {
				KiReadySummary[iterator] = false;
			}
			/* Ustawienie wartosci true na bicie watku postojowego, poniewaz jest on zawsze dostepny */
			KiReadySummary[0] = true;
		}

	}

	



	Scheduler::Scheduler(PCB* Init) {
		InitProcess = Init;
	Running = InitProcess;

																																//Modyfikatory

		REALTIME_CLASS_THREAD_PRIORITY_TIME_CRITICAL = Scheduler::PriorityAmount - 1;	
		VARIABLE_CLASS_THREAD_PRIORITY_IDLE = 1;
		VARIABLE_CLASS_THREAD_PRIORITY_TIME_CRITICAL = Scheduler::PriorityAmount / 2;
		REALTIME_CLASS_THREAD_PRIORITY_IDLE = (VARIABLE_CLASS_THREAD_PRIORITY_TIME_CRITICAL + 1);
		VARIABLE_CLASS_THREAD_PRIORITY_NORMAL = ((VARIABLE_CLASS_THREAD_PRIORITY_TIME_CRITICAL - VARIABLE_CLASS_THREAD_PRIORITY_IDLE) / 2 + VARIABLE_CLASS_THREAD_PRIORITY_IDLE);
		REALTIME_CLASS_THREAD_PRIORITY_NORMAL = ((REALTIME_CLASS_THREAD_PRIORITY_TIME_CRITICAL - REALTIME_CLASS_THREAD_PRIORITY_IDLE) / 2 + REALTIME_CLASS_THREAD_PRIORITY_IDLE);



																												//Ustawienie wartosci poczatkowych na masce bitowej 
		Scheduler::MaskaBitowa();


		KiDispatcherReadyListHead = vector<list<PCB*>>(Scheduler::PriorityAmount);

		KiDispatcherReadyListHead.resize(Scheduler::PriorityAmount);
		for (auto iterator = REALTIME_CLASS_THREAD_PRIORITY_TIME_CRITICAL; iterator >= INITPROCESS_THREAD_PRIORITY; iterator--) {			//Tworzenie list na vectorze
			list<PCB*> a;
			KiDispatcherReadyListHead[iterator] =  a;
		}
		KiDispatcherReadyListHead[0].push_back(InitProcess);															
	}
	



	
		void Scheduler::addtoreadyList(PCB* p) {
		if (p->PCB::getStan() == 3) {
			int naktoraliste = p->PCB::getPriorytet();
			KiDispatcherReadyListHead[naktoraliste].push_front(p);

			if (KiReadySummary[naktoraliste] == false) {
				KiReadySummary[naktoraliste] = true;
			}
		}
		else {};
		}



	void Scheduler::usun(PCB* p) {
		if (p != InitProcess) {
			int zktorejlisty = p->PCB::getPriorytet();
			KiDispatcherReadyListHead[zktorejlisty].remove(p);


			if (KiDispatcherReadyListHead[zktorejlisty].empty()) {
				KiReadySummary[zktorejlisty] = false;
			}
		}
		else {
			cout << "Nie usuwamy procesu bezczynnosci (INIT)" << endl;
		};
		
	}

	PCB* foundThread;
	PCB* Scheduler::FindReadyThread()
	{
		for (auto iterator = REALTIME_CLASS_THREAD_PRIORITY_TIME_CRITICAL; iterator >= VARIABLE_CLASS_THREAD_PRIORITY_IDLE; iterator--) {
			/* Sprawdzenie bitu maski bitowej odpowiadajacej za dany numer priorytetu*/
			if (KiReadySummary[iterator] == true) {
				/* Pobranie pierwszego procesu z kolejki*/
				foundThread = KiDispatcherReadyListHead[iterator].back();

				/* Sprawdzenie czy nie wystapil blad
				* - proces ma inny stan niz gotowy
				* - proces z jakiegos powodu zostal usuniety(null) ale nadal jest dostepny w kolejce
				* */

				if (foundThread->PCB::getStan() == 3) {
					/* Jesli nie ma bledu przekaz zwroc proces planiscie  */
					return foundThread;
				}
				else{
					KiDispatcherReadyListHead[iterator].remove(foundThread);
					if (KiDispatcherReadyListHead[iterator].empty()) {
						KiReadySummary[iterator] = false;
					}
					iterator += 1;
				}
				//	
			}
		}

		return InitProcess;
	}

	
	void Scheduler::Wyswietlkolejke()
	{
		for (auto iterator = REALTIME_CLASS_THREAD_PRIORITY_TIME_CRITICAL; iterator >= VARIABLE_CLASS_THREAD_PRIORITY_IDLE; iterator--) {
			/* Sprawdzenie bitu maski bitowej odpowiadajacej za dany numer priorytetu*/
			if (KiReadySummary[iterator] == true) {
				
				list<PCB*> tymczasowa = KiDispatcherReadyListHead[iterator];
			//	tymczasowa[iterator];
			/*	std::list<int>::iterator iterator;*/
			/*	for (iterator = tymczasowa.begin(); iterator != tymczasowa.end(); ++iterator) {
					std::cout << *iterator;
				}*/

				for (auto it : tymczasowa) {
					cout << it->getNazwa()<<endl;
				}
			
			/* Pobranie pierwszego procesu z kolejki*/
				//foundThread = tymczasowa[1];
			//	cout << foundThread->PCB::getNazwa()<<endl;
				/* Sprawdzenie czy nie wystapil blad
				* - proces ma inny stan niz gotowy
				* - proces z jakiegos powodu zostal usuniety(null) ale nadal jest dostepny w kolejce
				* */

				
				//	
			}
		}


	}

	void Scheduler::ReadyThread(PCB* process) {
		process->PCB::ustawStan(3);
		//IsReadyThreadExpropriating(process);
		Scheduler::addtoreadyList(process);
		IsReadyThreadExpropriating(process);
		
		if (process->PCB::getPriorytet() > Running->PCB::getPriorytet()) {
			if (Running != InitProcess) {
				Scheduler::addtoreadyList(Running);
				Running = process;
			}


		}
		else {
			Scheduler::addtoreadyList(process);

		}				//wywlaszcanie
		/*	if (czywywlaszczone == true) {
	
			Scheduler::usun(process);

		}*/


	}

	
		
	//READYTHREAD(WERSJA DLA MODULU SEMAFORA)
		// Metoda udostepniona modulowi semafora do poinformowania planisty o stanie gotowosci danego procesu i podwyzszeniu jego priorytetu ze wzgledu na oczekiwanie
		
		void Scheduler::ReadyThread(PCB* process, bool isItSemaphore) {
			process->PCB::ustawStan(3);
		if (isItSemaphore == true) {
			int temporaryPriorityNumber = process->PCB::getPriorytet() + 1;
			/* Tymczasowe podwyzszenie priorytetu oczekujacego(wczesniej) na semafor procesu*/
			process->PCB::ustawPriorytet(temporaryPriorityNumber);
			int givenQuantumAmount = process->PCB::getGivenQuantumAmount();
			/*Ilosc jednostek kwantu czasu procesora jest zmniejszana o jeden jesli to semafor podwyzsza tymczasowo priorytet procesowi
			*Po wykorzystaniu kwantu ilosc jednostek powroci do stanu domyslnego */
			if (givenQuantumAmount>1) {
				process->::PCB::setGivenQuantumAmount(givenQuantumAmount - 1);
			}

		}
		if (process->PCB::getPriorytet() > Running->PCB::getPriorytet()) {					//wyw³aszczanie!
			if (Running != InitProcess) {
				Scheduler::addtoreadyList(Running);
				Running = process;
			}


		}
		else {
			Scheduler::addtoreadyList(process);

		}
	}
	/*
	* READYTHREAD (WERSJA DLA MODULU SEMAFORA)
	*/




	void Scheduler::IsReadyThreadExpropriating(PCB* process) {
		if (Running != nullptr) {
			if (process->PCB::getPriorytet()>Running->PCB::getPriorytet()) {
				czywywlaszczone = true;
			}
		}
		else {
			czywywlaszczone = true;
		}
		
	}



	//*Funkcja wywolywana co jakis czas przez planiste zapobiegajaca glodzeniu procesow o niskim priorytecie
	//	* Jezeli czas oczekiwania na kwant czasu przez dany proces przekroczy dopuszczalna wartosc proces ten uzyskuje "na chwile" najwyzszy priorytet w klasie priorytetow
	//	* zmiennych i jego ilosc dostepnych jednostek kwantu czasu wydluza sie dwukrotnie, po ich wykorzystaniu proces z powrotem wraca do swoich wartosci domyslnych

	void Scheduler::BalanceSetManager()
	{
		for (auto iterator2 = VARIABLE_CLASS_THREAD_PRIORITY_TIME_CRITICAL; iterator2 >= VARIABLE_CLASS_THREAD_PRIORITY_IDLE; iterator2--) {
		   /* Sprawdzenie bitu maski bitowej odpowiadajacej za dany numer priorytetu*/
		if (KiReadySummary[iterator2] == true) {
			/* Iteracja po procesach w danej kolejce w celu znalezienia glodzonych watkow */
			for (auto it = 0; it<KiDispatcherReadyListHead[iterator2].size(); it++) {
				if (!KiDispatcherReadyListHead[iterator2].empty()) {
					PCB* process = KiDispatcherReadyListHead[iterator2].front();
				
					/* Sprawdzenie czy watek zostal zaglodzony */
					if (Scheduler::QuantumAmountCounter - process->PCB::lastQuantumCounter >= Scheduler::QuantumAmountToBalance) {
						/* Podwyzszenie priorytetu zaglodzonego watku do najwyzszego mozliwego w klasie priorytetow zmiennych */
						process->ustawPriorytet(VARIABLE_CLASS_THREAD_PRIORITY_TIME_CRITICAL);
						/* Wydluzenie ilosci jednostek kwantu czasu dwukrotnie (na wzor Windows) */
						int quantumExtended = (process->PCB::getDefaultGivenQuantumAmount() * 2);
						process->setGivenQuantumAmount(quantumExtended);
						/* Usuniecie procesu z kolejki z ktorej pochodzil */
						KiDispatcherReadyListHead[iterator2].remove(process);
						/* Po zmianie danych dodanie procesu do odpowiadajacej mu aktualnie kolejki */
						Scheduler::addtoreadyList(process);
					
						if (KiDispatcherReadyListHead[iterator2].empty()) {
							KiReadySummary[iterator2] = false;
						}
					}
					else {
						KiDispatcherReadyListHead[iterator2].pop_front();
						KiDispatcherReadyListHead[iterator2].push_back(process);				//dzieki temu przejrzy cala kolejke
					}
				}
				}
			}
		}

	}


		
	void Scheduler::Go() 
	{
		bool WasExpriopriatedDuringQuantum;
		/*Zmienna pomocnicza zliczajaca ilosc wykonanych instrukcji w danym kwancie czasu */
		int InstructionsExecuted = 0;

		 WasExpriopriatedDuringQuantum = false;
	

	
		/* Sprawdzenie czy juz nie czas odpalic BalanceSetManager do zaglodzonych watkow */
		if (QuantumAmountCounter % QuantumAmountToRunBalanceSetManager == 0) {
			Scheduler::BalanceSetManager();
		}

		/* Znalezienie procesu o najwyzszym priorytecie do wykonania*/
				Running = Scheduler::FindReadyThread();
				if (Running != InitProcess) {
					Scheduler::usun(Running);
				}
		cout<<"Wybrany proces w Go " + Running->PCB::getNazwa()<<endl;
		Running->PCB::ustawStan(4);


		///////////////////////////////////////////

		/* Petla wykonujaca rozkazy odpowiadajaca jednemu kwantowi czasu procesora */
		while (InstructionsExecuted < Scheduler::InstructionsPerQuantum) {											
			
		

		///////////////////////////////////////////

		if (Running != nullptr && Running != InitProcess) {
			/* Zapisanie informacji o stanie licznika procesora w momencie ostatniego przydzialu procesora procesowi(dla Balance) */
			Running->PCB::setSchedulersLastQuantumAmountCounter(QuantumAmountCounter);


			

			/*
			* ======================================
			* SEKCJA WYKONYWANIA ROZKAZU
			* ======================================
			*/

			cout << "Wykonano rozkaz" << endl;

			if (Running->PCB::czyczekaa()) {							//pierwszoplanowy ma czekac?
			//	Running->PCB::ustawczyczeka(false);
				czekajacy = Running;
				Scheduler::usun(Running);
				Running = Scheduler::FindReadyThread();
				lockdoczekania = false;
				cout << "Poprzedni proces czeka na cos, wiec odpalamy " << Running->PCB::getNazwa()<<endl;
				Running->PCB::setGivenQuantumAmount(Running->PCB::getGivenQuantumAmount() + 1); 				//uczciwosc dla procesu! jezeli dostal dopiero teraz,
			} 
			if (czekajacy != nullptr) {
				if (lockdoczekania == true) {
					czekajacy->ustawczyczeka(false);
						czekajacy->PCB::ustawStan(3);
					czekajacy->PCB::ustawPriorytet(4);	
					Scheduler::addtoreadyList(czekajacy);
					Scheduler::pomocustawlock(false);//ustaw na wysoki, bo czekal
						//lockdoczekania = true;
				}
			}
				
			/*
			* ======================================
			* SEKCJA WYKONYWANIA ROZKAZU - KONIEC
			* ======================================
			*/


		}


		/////////////////////////////////////////// Je¿eli proces konczylby sie na wczesniejszym niz czwartym rozkazie, to u niego wywola sie FindReadyThread
		else {
			/* Proba znalezienia procesu(na wypadek jakby proces dotychczas wykonywany sie zakonczyl i watek ustawiony bylby na null) */
			Running = FindReadyThread();
			if (Running != nullptr && Running != InitProcess) {
				/* Obnizenie licznika wykonywanych instrukcji, ktory zostanie podbity na koniec petli */
				InstructionsExecuted--;
				/* Zmiana stanu na running */
				Running->PCB::ustawStan(4);
				/* Ustawienie flagi "niepelnego kwantu" */
				WasExpriopriatedDuringQuantum = true;																				//Jezeli zakonczy sie wczesniej!
			}
			else {
				
			}

		}

	

			/* Podbicie licznika wykonanych instrukcji */
			InstructionsExecuted++;
	
		}
	
		
	
	


		Running->PCB::GivenQuantumAmount--;				//minal kwant, wiec zmniejszamy o jeden

		if (Running->PCB::GivenQuantumAmount <= 0) {
			Running->PCB::GivenQuantumAmount = 2;
			Running->PCB::ustawStan(3);
			if (Running->PCB::getPriorytet() != Running->PCB::getPriorytetBazowy()) { Running->PCB::ustawPriorytet(Running->PCB::getPriorytetBazowy()); }
			if (Running != InitProcess) {
				Scheduler::addtoreadyList(Running);
			}
			QuantumAmountCounter++;
			Running = InitProcess;						//To dosyc prosto, jezeli skoncza sie kwanty, no to szukamy nowego
			return;
		}

		Running->PCB::ustawStan(3);
		if (Running != InitProcess) {
			Scheduler::addtoreadyList(Running);
		}
		Running = InitProcess;
		//Running = Scheduler::FindReadyThread();
		QuantumAmountCounter++;
	


	}
	

	void Scheduler::pomocustawlock(bool a)
	{
		lockdoczekania = a;
	}
	;







	///*Sprawdzenie czy nie jest ustawiony watek postojowy, zabezpieczenie przed bledem */
	//if (Running != nullptr && Running !=InitProcess) {
	//	
	//	/* Po uplynieciu kwantu czasu ustawiamy stan procesu ktory wykonal ostatni rozkaz na stan Ready pod warunkiem ze wczesniej jego stan nie zostal zmieniony(np. na Waiting) */
	//	if (Running->PCB::getStan() == 4) {
	//	Running->PCB::ustawStan(3);
	//	}
	//	else {
	//		/* Jezeli proces po wykorzystaniu swojego kwantu czasu zmienil stan (np. na Waiting lub Terminated) powinien zostac usuniety z kolejki procesowy gotowych*/
	//		usun(Running);
	//	}



	//	/* Sprawdzenie czy w trakcie kwantu procesora nastapilo wywlaszczenie, bo jesli bylo to zaden z procesow uczestniczacych nie wyczerpal pelnego kwantu czasu i przysluguje mu nadal pelny */
	//	if (WasExpriopriatedDuringQuantum == false) {
	//		/* Po wykorzystaniu kwantu przez proces aktualizacja informacji w jego procesie kontrolnym */
	//		Running->PCB::setSchedulersLastQuantumAmountCounter(QuantumAmountCounter);
	//		Running->PCB::setUsedQuantumAmount((Running->PCB::getUsedQuantumAmount() + 1));



	//		/* Jesli proces wykorzystal ilosc jednostek kwantow czasu nalezy powziac odpowiednie kroki*/
	//		if (Running->PCB::getUsedQuantumAmount() == Running->PCB::getGivenQuantumAmount()) {

	//			/* Usuniecie procesu z poczatku kolejki*/
	//			Scheduler::usun(Running);

	//			/* Wyzerowanie ilosci uzytych jednostek kwantow czasu*/
	//			Running->PCB::setUsedQuantumAmount(0);
	//			/* Jesli tymczasowo podwyzszono lub zmniejszono ilosc danych jednostek kwantow czasu dla danego procesu, po ich wykorzystaniu nalezy wrocic do wartosci domyslnych*/
	//			if (Running->PCB::getGivenQuantumAmount() != Running->PCB::getDefaultGivenQuantumAmount()) {
	//				Running->PCB::setGivenQuantumAmount(Running->PCB::getDefaultGivenQuantumAmount());
	//			}
	//			/* Jesli tymczasowo podwyzszono priorytet procesu i proces wykorzystal swoj kwant czasu powinien wrocic do priorytetu bazowego*/
	//			if (Running->PCB::getPriorytet() != Running->PCB::getPriorytetBazowy()) {
	//				Running->PCB::ustawPriorytet(Running->PCB::getPriorytetBazowy());
	//			}

	//			/* Zebezpieczenie przed bledem dodania do kolejki procesu o innym stanie niz Ready */
	//			if (Running->PCB::getStan() == 3) {
	//				/* Dodanie procesu na koniec kolejki o odpowiednim priorytecie */
	//				Scheduler::addtoreadyList(Running);
	//			}
	//		}

	//	}
	//}

	/* Po wykonaniu kwantu czasu uruchomienie watku postojowego */
	//		Running = InitProcess;
	/* Zwiekszenie wartosci licznika wykonanych kwantow przez procesor */


	///* Sprawdzenie czy watek ktory otrzymal kwant czasu zostal wywlaszczony */
	//if (czywywlaszczone == true) {
	//	std::cout << "wywlaszczenie";
	//	WasExpriopriatedDuringQuantum = true;
	//	/* Zabezpieczenie przed potencjalnym bledem o niskim prawdopodobienstwie */
	//	if (Running != nullptr && Running != InitProcess) {
	//		/* Aktualizacja danych o procesie wywlaszczonym */
	//		Running->PCB::ustawStan(4);
	//		//Scheduler.Running.schedulingInformations.setSchedulersLastQuantumAmountCounter(QuantumAmountCounter);
	//		if (Running->PCB::getUsedQuantumAmount() >= 1) {
	//			Scheduler::usun(Running);
	//			Scheduler::addtoreadyList(Running);
	//		}
	//	}
	//	/* Ustawienie procesu wywlaszczajacego na aktualnie wykonywany */
	//	Running = Scheduler::FindReadyThread();
	//	/* Zmiana stanu na running */
	//	Running->PCB::ustawStan(4);
	//}
	//else {
	//	
	//}
	/* Zabezpieczenie przed przypadkiem kiedy proces zostal ustawiony na stan Waiting lub Terminated
	* (Oddzielna instrukcja warunkowa aby zapobiec bledom sprawdzania stanu procesu obiektu null*/
	//if (Running != InitProcess && Running != nullptr &&  Running->PCB::getStan() != 4) {					//ZABEZPIECZENIE W GO
	//	
	//	/* Usuniecie z listy procesow gotowych */
	//	Scheduler::usun(Running);
	//	/* Znalezienie procesu o najwyzszym priorytecie do wykonania*/
	//	Running = FindReadyThread();
	//	/* Zmiana stanu na running */
	//	Running->PCB::ustawStan(4);

	//}