#include "scheduler.h"
#include<iostream>
#include <vector>
#include "Zarzadzanie_procesami.h"
using namespace std;
	
extern PCB* czekajacy;





int main() {

	PCB* Init = new PCB("init");
	Scheduler::Scheduler(Init);

	PCB* process = new PCB("nazwa", 1);
	PCB* process2 = new PCB("nazwa2", 2);
	PCB* process3 = new PCB("nazwa3", 3);
	PCB* process4 = new PCB("nazwa4", 2);

	PCB* process5= new PCB("nazwa5", 6);
	PCB* processnowy2 = new PCB();
	PCB* processnowy3 = new PCB();
	//PCB* process6 = new PCB("nazwa6", 2);
	//PCB* process7= new PCB("nazwa7", 1);
	//PCB* process8 = new PCB("nazwa8", 2);	
	//PCB* process9 = new PCB("nazwa9", 1);
	//PCB* process10 = new PCB("nazwa10", 2);
	
	Scheduler::ReadyThread(process);
	Scheduler::ReadyThread(process2);
	Scheduler::ReadyThread(process3);

	/*Scheduler::ReadyThread(process4);
	Scheduler::ReadyThread(process5);
	Scheduler::ReadyThread(process6); 
	Scheduler::ReadyThread(process7);
	Scheduler::ReadyThread(process8);
	Scheduler::ReadyThread(process9);
	Scheduler::ReadyThread(process10);*/

	
	Running = Scheduler::FindReadyThread();

	char a;
	for (;;) {

		cout << "Jaka operacja?" << endl<<endl;
		cout << "g - jeden takt" << endl;
		cout << "1 - sto taktow" << endl;
		cout << "2 - Usun running" << endl;
		cout << "3 - Wyswietl info o running" << endl;
		cout << "4 - Ready Semaphora (dla procesu 4, o priorytecie 2)" << endl;
		cout << "5 - Info o 3ch procesach" << endl;
		cout << "6 - Jakie procesy na liscie" << endl;
		cout << "7 - Ready proces o priorytecie statycznym (wysokim!)" << endl;
		cout << "f - Nastepny rozkaz wywola oczekiwanie tego procesu" << endl;
		cout << "n - proces doczeka sie na to, na co czekal, podczas nastepnego taktu" << endl;
		cout << "z-v - dodawanie procesow" << endl;

		std::cin >> a;


		switch (a) {
		case 'g': {
			cout << "Pojedyncze go" << endl;
			Scheduler::Go();
			break;

		}

			case '1':
		{
			
			cout << "Wiele Go" << endl;
			for (int i = 0; i < 100; i++) {
				Scheduler::Go();
			}
			break;
		}
		case '2':
		{
			cout << "Usun running"<<endl;
			cout << "czyli " << Running->PCB::getNazwa()<<endl;
			Scheduler::usun(Running);
			Running = Scheduler::FindReadyThread();
			break;
		}


		case '3':
		{
			cout << "Obecne Running: " << endl;
			Running->PCB::wyswietlInformacje();
			break;
		}
		
		case '4': {
			Scheduler::ReadyThread(process4, true);
			break;
		}

		case '5':
		{
			process->PCB::wyswietlInformacje();
			process2->PCB::wyswietlInformacje();
			process3->PCB::wyswietlInformacje();
			break;
		}
		case '6':
		{
			Scheduler::Wyswietlkolejke();
			break;
		}

		case '7':
		{				Scheduler::ReadyThread(process5);
			break;
		}

				case 'f': {
			Running->ustawczyczeka(true);
			break;
		}
			case 'n': {

				Scheduler::pomocustawlock(true);
				break;

		}
					

		case 'z':
		{
			cout << "Dodaj proces,jaka nazwa, jaki priorytet?" << endl;
			string a;
			cin >> a;
			int b;
			cin >> b;
			PCB* processnowy = new PCB(a, b);
			Scheduler::ReadyThread(processnowy);
			Running = Scheduler::FindReadyThread();
			break;
		}
		case 'x':
		{
			cout << "Dodaj proces,jaka nazwa, jaki priorytet?" << endl;
			string a;
			cin >> a;
			int b;
			cin >> b;
			processnowy2 = new PCB(a, b);
			Scheduler::ReadyThread(processnowy2);
			Running = Scheduler::FindReadyThread();
			break;
		}
		case 'w':
		{
			processnowy2->PCB::wyswietlInformacje();
			break;
		
		}
		case 'e':
		{
			processnowy3->PCB::wyswietlInformacje();
			break;

		}

		case 'c':
		{
			cout << "Dodaj proces,jaka nazwa, jaki priorytet?" << endl;
			string a;
			cin >> a;
			int b;
			cin >> b;
		processnowy3 = new PCB(a, b);
			Scheduler::ReadyThread(processnowy3);
			Running = Scheduler::FindReadyThread();
			break;
		}
		case 'v':
		{
			cout << "Dodaj proces,jaka nazwa, jaki priorytet?" << endl;
			string a;
			cin >> a;
			int b;
			cin >> b;
			PCB* processnowy4 = new PCB(a, b);
			Scheduler::ReadyThread(processnowy4);
			Running = Scheduler::FindReadyThread();
			break;
		}




		}


	}

	return 0;
}