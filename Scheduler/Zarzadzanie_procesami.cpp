
#include<iostream>
#include<vector>
#include<fstream>
#include<list>
#include<string>
#include<iterator>
#include "Zarzadzanie_procesami.h"


using namespace std;

//extern memoryManagement mem;
list<PCB*>ListaProcesow;
list<PCB*>ListaGotowych;

int x = 1;
int z = 7;
int g = 0;
PCB* Init = new PCB("init", 0, "");

PCB::PCB() {};

PCB::PCB(string nazwa, int size, string nazwaPliku)
{

	PID = x;
	x++;
	NazwaProcesu = nazwa;
	stanProcesu = 1;
	LicznikRozkazow = 0;
	if (z == 0) { z = 7; }
	PriorytetBazowy = z;
	Priorytet = PriorytetBazowy;
	lastQuantumCounter = 0;
	DefaultGivenQuantumAmount = 2;
	GivenQuantumAmount = DefaultGivenQuantumAmount;
	czyczeka = false;
	z--;
	
	//Size = size;
	ListaProcesow.push_back(this);
	plik(nazwaPliku);
}

PCB::PCB(string nazwa, int priorytet )
{

	PID = x;
	x++;
	NazwaProcesu = nazwa;
	stanProcesu = 1;
	LicznikRozkazow = 0;
	//if (g == 0) { g = 4; }
	PriorytetBazowy = priorytet;
	Priorytet = PriorytetBazowy;
	lastQuantumCounter = 0;
	DefaultGivenQuantumAmount = 2;
	GivenQuantumAmount = DefaultGivenQuantumAmount;
	czyczeka = false;
	//g--;

//	Size = size;
	ListaProcesow.push_back(this);
}



PCB::PCB(string nazwa)
{
	PID = x;
	x++;
	NazwaProcesu = nazwa;
	stanProcesu = 1;
	LicznikRozkazow = 0;
	PriorytetBazowy = 0;
	lastQuantumCounter = 0;
	DefaultGivenQuantumAmount = 2;
	GivenQuantumAmount = DefaultGivenQuantumAmount;
	Priorytet = PriorytetBazowy;
	czyczeka = false;
	ojciec = this;
	ListaProcesow.push_back(this);
}

void PCB::wyswietlInformacje()
{
	cout << "ID procesu: " << this->PID << endl;
	cout << "Nazwa procesu: " << this->NazwaProcesu << endl;
	cout << "Stan procesu: " << this->stanProcesu << endl;
	cout << "Priorytet bazowy: " << this->PriorytetBazowy << endl;
	cout << "Priorytet: " << this->Priorytet << endl;
	cout << "Lastquantum kiedy byl wykonany " << this->lastQuantumCounter << endl;
	cout << "czy czeka " << this->czyczeka << endl;
	cout << "ile kwantow ma jeszcze " << this->GivenQuantumAmount << endl;
}

PCB* PCB::addPro(string name, int siz, string plik)
{
	PCB* z = new PCB(name, siz, plik);
	z->ustawOjciec(this);
	z->PPID = (*ojciec).getID();
	this->dodajDziecko(z);
	return z;
}

int PCB::getSize()
{
	return Size;
}

void PCB::ustawLicznik(int n)
{
	LicznikRozkazow = n;
}

void PCB::ustawStan(int s)
{
	stanProcesu = s;
}

int PCB::getStan()
{
	return stanProcesu;
}

int PCB::getlastQuantumCounter() {
	return lastQuantumCounter;
}

int PCB::getUsedQuantumAmount()
{
	return UsedQuantumAmount;
}

int PCB::getDefaultGivenQuantumAmount()
{
	return DefaultGivenQuantumAmount;
}

void PCB::ustawPriorytet(int p)
{
	Priorytet = p;
}

int PCB::getPriorytet()
{
	return Priorytet;
}

int PCB::getPriorytetBazowy()
{
	return PriorytetBazowy;
}

string PCB::getNazwa()
{
	return NazwaProcesu;
}

void PCB::setGivenQuantumAmount(int givenQuantumAmount) {
	GivenQuantumAmount = givenQuantumAmount;
}

void PCB::setSchedulersLastQuantumAmountCounter(int LastQuantumCounter)
{
	lastQuantumCounter = LastQuantumCounter;

}

void PCB::setUsedQuantumAmount(int usedQuantumAmount)
{
	UsedQuantumAmount= usedQuantumAmount;
}

bool PCB::czyczekaa()
{
	return czyczeka;
}

void PCB::ustawczyczeka(bool a)
{
	 czyczeka = a;
	
}

int PCB::getID()
{
	return PID;
}

int PCB::getPPID()
{
	return PPID;
}

PCB* PCB::getOjciec()
{
	return ojciec;
}

void PCB::ustawOjciec(PCB* o)
{
	ojciec = o;
}

void PCB::dodajDziecko(PCB* d)
{
	dzieci.push_back(d);
}

void PCB::wyswietlListeDzieci()
{
	for (auto it : dzieci)
	{
		cout << it->getNazwa() << endl;
	}
}


void PCB::dodajLicznik(int licz)
{
	LicznikRozkazow += licz;
}

int PCB::getLicznik()
{
	return LicznikRozkazow;
}

int PCB::getBaza()
{
	return baza;
}

void PCB::ustawBaza(int i)
{
	baza = i;
}

int PCB::getLimit()
{
	return limit;
}

void PCB::ustawLimit(int i)
{
	baza = i;
}



void PCB::plik(string p1)
{
	//mem.writeToPF(this, p1);
	stanProcesu = 1;
	ListaGotowych.push_back(this);
}



int PCB::getGivenQuantumAmount()
{
	return GivenQuantumAmount;
}

void PCB::sprawdzOjca()
{
	for (auto it : ListaProcesow)
	{
		if (it == this->getOjciec())
			cout << it->getNazwa() << endl;
	}
}




PCB::~PCB()
{
	if (this->getStan() == 3)
	{
		ListaGotowych.remove(this);
	}
	for (auto it : dzieci)
	{
		if (it->getOjciec() == this)
		{
			for (auto ite : ListaProcesow)
			{
				if (ite->getID() == 1)
				{
					(it)->ustawOjciec(ite);
					ite->dzieci.push_back(it);
				}
			}
		}
	}
	ListaProcesow.remove(this);
	getOjciec()->dzieci.remove(this);
}
