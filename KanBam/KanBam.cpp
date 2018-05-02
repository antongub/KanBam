/**
* KanBam
*
* A KanBan Board which is only executable in the Command Line Interface
*
* @version 0.0.1 vom 30.04.2018
* @author Anton Gubenko && Justin Zien
*/

//INCLUDE DIRECTORIES
#include "stdafx.h"
#include "conio.h"
#include "iostream"
#include "string"

// DEFINE ANLEGEN
#define AUFGABENMAXIMUM 999

// USING DIRECTORIES
using namespace std;


// DEKLARIEREN & INITIALISIEREN
enum Spalte {Backlog, ToDo, Doing, Done};
int aufgabenNummer = 1;


// STRUKTUREN
struct Aufgabe {
	string titel;
	string beschreibung;
	string bearbeiter;
	int nummer;
	Spalte spalte;
};

struct Board {
	Aufgabe alleAufgaben[AUFGABENMAXIMUM];
	const Spalte alleSpalten[4] = {Backlog, ToDo, Doing, Done};
	const int spaltenBreite = 15;

	void aufgabeHinzufuegen(string titel, string beschreibung, string bearbeiter) {
		Aufgabe aufgabe;
		if (aufgabenNummer < 999) {
			aufgabe.titel = titel;
			aufgabe.beschreibung = beschreibung;
			aufgabe.bearbeiter = bearbeiter;
			aufgabe.spalte = Backlog;
			aufgabe.nummer = aufgabenNummer;
			KanBamBoard.alleAufgaben[aufgabenNummer] = aufgabe;
			aufgabenNummer++;
		}
	}

	void aufgabeBearbeiten(int aufgabenNummer) {
		Aufgabe aufgabe = KanBamBoard.alleAufgaben[aufgabenNummer];
		string titel, beschreibung, bearbeiter;
		cout << "Was willst du bearbeiten?" << endl;
		cout << "titel";
		cin >> titel;
		cout << "beschreibung";
		cin >> beschreibung;
		cout << "bearbeiter";
		cin >> bearbeiter;

		aufgabe.titel = titel;
		aufgabe.beschreibung = beschreibung;
		aufgabe.bearbeiter = bearbeiter;
		KanBamBoard.alleAufgaben[aufgabenNummer] = aufgabe;
	}

	void aufgabeLöschen(int aufgabenNummer) {
		Aufgabe leereAufgabe;
		KanBamBoard.alleAufgaben[aufgabenNummer] = leereAufgabe;
	}

	void aufgabeLesen(int aufgabenNummer) {
		Aufgabe aufgabe = KanBamBoard.alleAufgaben[aufgabenNummer];
		cout << "Nummer: " << aufgabe.nummer << endl;
		cout << "Titel: " << aufgabe.titel << endl;
		cout << "Beschreibung: " << aufgabe.beschreibung << endl;
		cout << "Bearbeiter: " << aufgabe.bearbeiter << endl;
		cout << "Spalte: " << aufgabe.spalte << endl;
	}

	void aufgabeVerschieben(int aufgabenNummer, Spalte spalte) {
		KanBamBoard.alleAufgaben[aufgabenNummer].spalte = spalte;
	}

}KanBamBoard;


// METHODEN
void gebeAlleAufgabenAusSpalteAus(Spalte spalte) {
	cout << spalte << endl;
	for (int i = 0; i < AUFGABENMAXIMUM; i++) {
		if (KanBamBoard.alleAufgaben[i].spalte == spalte) {
			cout << KanBamBoard.alleAufgaben[i].nummer << endl;
			cout << KanBamBoard.alleAufgaben[i].titel << endl;
			cout << KanBamBoard.alleAufgaben[i].beschreibung << endl;
			cout << KanBamBoard.alleAufgaben[i].bearbeiter << endl;
			cout << KanBamBoard.alleAufgaben[i].spalte << endl;
		}
	}
}


// MAIN
int main() {
	// DEKLARIEREN
	string eingabeName = "";
	string eingabeBeschreibung = "";
	string eingabeBearbeiter = "";
	int userInput = 0;

	//Dauerschleife
	while (true) {
		//INITIALISIEREN
		eingabeName = "";
		eingabeBeschreibung = "";
		eingabeBearbeiter = "";
		userInput = 0;

		// AUSGABE
		/*
		gebeAlleAufgabenAusSpalteAus(Backlog);
		gebeAlleAufgabenAusSpalteAus(ToDo);
		gebeAlleAufgabenAusSpalteAus(Doing);
		gebeAlleAufgabenAusSpalteAus(Done);
		*/

		// Benutzerschnittstelle
		  // Ausgabe: Benutzereingaben
		cout << "1. Aufgabe erstellen" << endl;
		cout << "2. Aufgabe bearbeiten" << endl;
		cout << "3. Aufgabe loeschen" << endl;
		cout << "4. Aufgabe verschieben" << endl;
		cout << "5. Aufgabe lesen" << endl;

		//Eingabe: Benutzereingaben
		cin >> userInput;
		switch (userInput) {
			case 1: {
				cout << "1" << endl;
				cout << "Name" << endl;
				cin.ignore();
				getline(cin, eingabeName);
				cout << "Beschreibung" << endl;
				getline(cin, eingabeBeschreibung);
				cout << "Bearbeiter" << endl;
				getline(cin, eingabeBearbeiter);
				KanBamBoard.aufgabeHinzufuegen(eingabeName, eingabeBeschreibung, eingabeBearbeiter);
				break;
			}
			case 2: {
				cout << "2" << endl;
				KanBamBoard.aufgabeBearbeiten(1);
				break;
			}
			case 3: {
				cout << "3" << endl;
				KanBamBoard.aufgabeLöschen(1);
				break;
			}
			case 4: {
				cout << "4" << endl;
				KanBamBoard.aufgabeVerschieben(1, ToDo);
				break;
			}
			case 5: {
				cout << "5" << endl;
				cout << "Welche Aufgabe?" << endl;
				int aufgabelesen;
				cin >> aufgabelesen;
				KanBamBoard.aufgabeLesen(aufgabelesen);
				break;
			}
			default: {
				cout << "Falsche Eingabe" << endl;
				break;
			}
		}

		_getch();
		system("CLS");
	}
	return 0;
}

