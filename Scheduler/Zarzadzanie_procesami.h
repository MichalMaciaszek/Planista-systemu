#pragma once

#include<iostream>
#include<vector>
#include<fstream>
#include<list>
#include<string>

//#include "memory.h"

using namespace std;


class PCB
{
public:
	int PID;
	int PPID;
	PCB* ojciec = NULL;

	string NazwaProcesu;
	int stanProcesu;  //1-nowy, 2-czekaj¹cy, 3-gotowy, 4-aktywny, 5-zakoñczony, 6-zombie
	int LicznikRozkazow;
	int Priorytet;
	int PriorytetBazowy;
	int Size;

public:
	list<PCB*> dzieci;
	static PCB *idle;
	void sprawdzOjca();
	int toGo;
	int baza;
	int limit;
	

	bool czyczeka;
	int DefaultGivenQuantumAmount;
	int lastQuantumCounter;
	int UsedQuantumAmount;
	int getlastQuantumCounter();
	int getUsedQuantumAmount();
	int getDefaultGivenQuantumAmount();
	int GivenQuantumAmount;
	void setGivenQuantumAmount(int givenQuantumAmount);
	void setSchedulersLastQuantumAmountCounter(int LastQuantumCounter);
	void setUsedQuantumAmount(int usedQuantumAmount);
	bool czyczekaa();
	void ustawczyczeka(bool a);

	vector<vector<int>> tablicaStronic;
	vector<int> workingSet;
	PCB();
	PCB(string nazwa, int size, string nazwaPliku);
	~PCB();

	PCB::PCB(string nazwa, int size);
	PCB(string nazwa);
	void wyswietlInformacje();
	PCB* addPro(string name, int siz, string plik);
	void ustawLicznik(int n);
	void ustawStan(int s);
	int getStan();
	int getSize();
	void ustawPriorytet(int p);
	int getPriorytet();
	int getPriorytetBazowy();
	string getNazwa();
	int getID();
	int getPPID();
	PCB* getOjciec();
	void ustawOjciec(PCB* o);
	void dodajDziecko(PCB* d);
	void wyswietlListeDzieci();
	void dodajLicznik(int licz);
	int getLicznik();
	int getBaza();
	void ustawBaza(int i);
	int getLimit();
	void ustawLimit(int i);
	void plik(string p1);
	int getGivenQuantumAmount();


};

extern list<PCB*>ListaProcesow;
extern list<PCB*>ListaGotowych;



