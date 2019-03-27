public void Go() throws Exception {

	/*Zmienna pomocnicza zliczajaca ilosc wykonanych instrukcji w danym kwancie czasu */
	byte InstructionsExecuted = 0;


	boolean WasExpriopriatedDuringQuantum = false;
	/* Jesli cokolwiek w czasie "przerwy" zostalo dodane to i tak planista to zauwazy dzieki metodzie FindReadyThread
	* takze flaga musi byc wyzerowana*/
	this.IsExpropriated = false;

	/* Sprawdzenie czy juz nie czas odpalic BalanceSetManager do zaglodzonych watkow */
	if (this.QuantumAmountCounter%QuantumAmountToRunBalanceSetManager == 0) {
		this.BalanceSetManager();
	}

	/* Znalezienie procesu o najwyzszym priorytecie do wykonania*/
	Scheduler.Running = this.FindReadyThread();

	System.out.println("The chosen one: " + Scheduler.Running.getPID());
	/* Zmiana stanu na running */
	Scheduler.Running.setStan(Process.processState.Running);

	/* Petla wykonujaca rozkazy odpowiadajaca jednemu kwantowi czasu procesora */
	while (InstructionsExecuted<Scheduler.InstructionsPerQuantum) {

		/* Zabezpieczenie przed przypadkiem kiedy proces zostal ustawiony na stan Waiting lub Terminated
		* (Oddzielna instrukcja warunkowa aby zapobiec bledom sprawdzania stanu procesu obiektu null*/
		if (Scheduler.Running != this.InitProcess && Scheduler.Running != null &&  Scheduler.Running.getState() != Process.processState.Running) {
			System.out.println(1);
			/* Usuniecie z listy procesow gotowych */
			this.RemoveFromReadyList(Scheduler.Running);
			/* Znalezienie procesu o najwyzszym priorytecie do wykonania*/
			Scheduler.Running = this.FindReadyThread();
			/* Zmiana stanu na running */
			Scheduler.Running.setStan(Process.processState.Running);

		}

		if (Scheduler.Running != null && Scheduler.Running != this.InitProcess) {
			/* Zapisanie informacji o stanie licznika procesora w momencie ostatniego przydzialu procesora procesowi(postarzanie/odmladzanie) */
			Scheduler.Running.schedulingInformations.setSchedulersLastQuantumAmountCounter(QuantumAmountCounter);


			/* Sprawdzenie czy watek ktory otrzymal kwant czasu zostal wywlaszczony */
			if (this.IsExpropriated == true) {
				System.out.println("wywlaszczenie");
				WasExpriopriatedDuringQuantum = true;
				/* Zabezpieczenie przed potencjalnym bledem o niskim prawdopodobienstwie */
				if (Scheduler.Running != null && Scheduler.Running != this.InitProcess) {
					/* Aktualizacja danych o procesie wywlaszczonym */
					Scheduler.Running.setStan(Process.processState.Ready);
					//Scheduler.Running.schedulingInformations.setSchedulersLastQuantumAmountCounter(QuantumAmountCounter);
					if (Scheduler.Running.schedulingInformations.getUsedQuantumAmount() >= 1) {
						this.RemoveFromReadyList(Scheduler.Running);
						this.AddToReadyList(Scheduler.Running);
					}
				}
				/* Ustawienie procesu wywlaszczajacego na aktualnie wykonywany */
				Scheduler.Running = this.FindReadyThread();
				/* Zmiana stanu na running */
				Scheduler.Running.setStan(Process.processState.Running);
			}
			else {
				/* Czy w innym wypadku cos trzeba robic? Zapewne nie */
			}


			/*
			* ======================================
			* SEKCJA WYKONYWANIA ROZKAZU
			* ======================================
			*/

			/* POBRANIE OSTATNICH ZAPISANYCH STANOW REJESTRU Z BLOKU KONTROLNEGO PROCESU */
			System.out.println("Aktualnie wykonywany proces: " + Scheduler.Running.getPID());

			_InterpreterModule.getRegister(Scheduler.Running.getR1(), Scheduler.Running.getR2(), Scheduler.Running.getProgramCounter());

			System.out.println("Wyjscie z interpretera1");
			/* WYKONANIE ROZKAZU W INTERPETERZE*/
			_InterpreterModule.getCommand(Scheduler.Running.getProcessTab().getCommand(Scheduler.Running.getProgramCounter()));
			System.out.println("Wyjscie z interpretera2");

			/* Ustawienie stanow rejestrow po wykonaniu rozkazu  */
			Scheduler.Running.setR1(_InterpreterModule.setRegisterA());
			Scheduler.Running.setR2(_InterpreterModule.setRegisterB());
			Scheduler.Running.setProgramCounter(_InterpreterModule.setCommandCounter());


			/* ZAPISANIE INFORMACJI DO BLOKU KONTROLNEGO PROCESU
			* JESLI PROCES WYKONA OSTATNI ROZKAZ NIE POWINIEN BYC OD RAZU USUNIETY TYLKO DOSTAC STATUS TERMINATED(ABY MOZNA BYLO ODCZYTAC Z NIEGO DANE I GO RECZNIE SKASOWAC)
			* I POWINIEN MIEC AKTUALNE DANE W REJESTRZE PO OSTATNIM ROZKAZIE*/

			/*
			* ======================================
			* SEKCJA WYKONYWANIA ROZKAZU - KONIEC
			* ======================================
			*/


		}
		else {
			System.out.println(3);
			/* Proba znalezienia procesu(na wypadek jakby proces dotychczas wykonywany sie zakonczyl i watek ustawiony bylby na null) */
			Scheduler.Running = this.FindReadyThread();
			if (Scheduler.Running != null && Scheduler.Running != this.InitProcess) {
				/* Obnizenie licznika wykonywanych instrukcji, ktory zostanie podbity na koniec petli */
				InstructionsExecuted--;
				/* Zmiana stanu na running */
				Scheduler.Running.setStan(Process.processState.Running);
				/* Ustawienie flagi "niepelnego kwantu" */
				WasExpriopriatedDuringQuantum = true;
			}
			else {
				/* Petla jest przerywana, aby nie marnowac czasu procesora */
				break;
			}
		}

		/* Podbicie licznika wykonanych instrukcji */
		InstructionsExecuted++;
		System.out.println(InstructionsExecuted);
	}

	/*Sprawdzenie czy nie jest ustawiony watek postojowy, zabezpieczenie przed bledem */
	if (Scheduler.Running != null && Scheduler.Running != this.InitProcess) {
		System.out.println(31);
		/* Po uplynieciu kwantu czasu ustawiamy stan procesu ktory wykonal ostatni rozkaz na stan Ready pod warunkiem ze wczesniej jego stan nie zostal zmieniony(np. na Waiting) */
		if (Scheduler.Running.getState() == Process.processState.Running) {
			System.out.println(32);
			Scheduler.Running.setStan(Process.processState.Ready);
		}
		else {
			System.out.println(33);
			/* Jezeli proces po wykorzystaniu swojego kwantu czasu zmienil stan (np. na Waiting lub Terminated) powinien zostac usuniety z kolejki procesowy gotowych*/
			this.RemoveFromReadyList(Scheduler.Running);
		}

		/* Sprawdzenie czy w trakcie kwantu procesora nastapilo wywlaszczenie, bo jesli bylo to zaden z procesow uczestniczacych nie wyczerpal pelnego kwantu czasu i przysluguje mu nadal pelny */
		if (WasExpriopriatedDuringQuantum == false) {
			System.out.println(4);
			/* Po wykorzystaniu kwantu przez proces aktualizacja informacji w jego procesie kontrolnym */
			Scheduler.Running.schedulingInformations.setSchedulersLastQuantumAmountCounter(QuantumAmountCounter);
			Scheduler.Running.schedulingInformations.setUsedQuantumAmount((byte)(Scheduler.Running.schedulingInformations.getUsedQuantumAmount() + 1));

			/* Jesli proces wykorzystal ilosc jednostek kwantow czasu nalezy powziac odpowiednie kroki*/
			if (Scheduler.Running.schedulingInformations.getUsedQuantumAmount() == Scheduler.Running.schedulingInformations.getGivenQuantumAmount()) {
				System.out.println(5);
				/* Usuniecie procesu z poczatku kolejki*/
				this.RemoveFromReadyList(Scheduler.Running);

				/* Wyzerowanie ilosci uzytych jednostek kwantow czasu*/
				Scheduler.Running.schedulingInformations.setUsedQuantumAmount((byte)0);
				/* Jesli tymczasowo podwyzszono lub zmniejszono ilosc danych jednostek kwantow czasu dla danego procesu, po ich wykorzystaniu nalezy wrocic do wartosci domyslnych*/
				if (Scheduler.Running.schedulingInformations.getGivenQuantumAmount() != Scheduler.Running.schedulingInformations.getDefaultGivenQuantumAmount()) {
					Scheduler.Running.schedulingInformations.setGivenQuantumAmount(Scheduler.Running.schedulingInformations.getDefaultGivenQuantumAmount());
				}
				/* Jesli tymczasowo podwyzszono priorytet procesu i proces wykorzystal swoj kwant czasu powinien wrocic do priorytetu bazowego*/
				if (Scheduler.Running.schedulingInformations.getPriorityNumber() != Scheduler.Running.schedulingInformations.getDefaultPriorityNumber()) {
					Scheduler.Running.schedulingInformations.setPriorityNumber(Scheduler.Running.schedulingInformations.getDefaultPriorityNumber());
				}

				/* Zebezpieczenie przed bledem dodania do kolejki procesu o innym stanie niz Ready */
				if (Scheduler.Running.getState() == Process.processState.Ready) {
					/* Dodanie procesu na koniec kolejki o odpowiednim priorytecie */
					this.AddToReadyList(Scheduler.Running);
				}
			}

		}
	}

	/* Po wykonaniu kwantu czasu uruchomienie watku postojowego */
	Scheduler.Running = this.InitProcess;
	/* Zwiekszenie wartosci licznika wykonanych kwantow przez procesor */
	QuantumAmountCounter++;
}
/*
* FUNKCJA GO - KONIEC
*/