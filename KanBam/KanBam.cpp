// KanBam.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "conio.h"
#include "iostream"
#include "string"

using namespace std;

int aufgabenNummer = 0;

struct Aufgabe {
	string titel;
	string beschreibung;
	string bearbeiter;
	int nummer;
	string spalte;
};


struct Board {
	Aufgabe alleAufgaben[999];
	const string alleSpalten[4];
	const int spaltenBreite = 15;


	void aufgabeHinzufuegen(Aufgabe neueAufgabe) {

	}

	void aufgabeBearbeiten(Aufgabe aufgabe) {

	}

	void aufgabeLöschen(Aufgabe aufgabe) {

	}

	void aufgabeLesen(Aufgabe aufgabe) {

	}

	void aufgabeVerschieben(Aufgabe aufgabe, string spalte) {

	}

}KanBamBoard;

void gebeAlleAufgabenAusSpalteAus(Aufgabe alleAufgaben[], string spalte) {

}

Aufgabe erstelleAufgabe(string titel, string beschreibung, string bearbeiter, string spalte) {
	Aufgabe aufgabe;
	if (aufgabenNummer < 999) {
		aufgabe.titel = titel;
		aufgabe.beschreibung = beschreibung;
		aufgabe.bearbeiter = bearbeiter;
		aufgabe.spalte = spalte;
		aufgabenNummer++;
		aufgabe.nummer = aufgabenNummer;
	}
	return aufgabe;
}

int main()
{
	_getch();

	return 0;
}

