// KanBam.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "conio.h"
#include "iostream"
#include "string"

using namespace std;


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

int main()
{
	_getch();

	return 0;
}

