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