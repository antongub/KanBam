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
int aufgabenNummer = 1;


// STRUKTUREN
struct Aufgabe {
	string titel;
	string beschreibung;
	string bearbeiter;
	int nummer;
	string spalte;
};

struct Board {
	Aufgabe alleAufgaben[AUFGABENMAXIMUM];
	const string alleSpalten[4] = { "Backlog", "ToDo", "Doing", "Done" };
	const int spaltenBreite = 15;

	void aufgabeHinzufuegen(string titel, string beschreibung, string bearbeiter) {
		Aufgabe aufgabe;
		if (aufgabenNummer < 999) {
			aufgabe.titel = titel;
			aufgabe.beschreibung = beschreibung;
			aufgabe.bearbeiter = bearbeiter;
			aufgabe.spalte = "Backlog";
			aufgabe.nummer = aufgabenNummer;
			KanBamBoard.alleAufgaben[aufgabenNummer] = aufgabe;
			aufgabenNummer++;
		}
	}

	/*
	 * Fragt den Nutzer welche der drei Felder vom Aufgaben Objekt er ändern will
	 * (titel, beschreibung, bearbeiter)
	 * Und überschreibt die Werte in die Aufgabe
	 * 
	 * @param int aufgabenNummer
	 * Nummer der Aufgabe aus dem Array im KanBam struct angelegtem "alleAufgaben[]"
	 * @return void
	 */
	void aufgabeBearbeiten(int aufgabenNummer) {
		Aufgabe aufgabe = KanBamBoard.alleAufgaben[aufgabenNummer];
		string titel, beschreibung, bearbeiter;
		cin.ignore();
		cout << "Was willst du bearbeiten?" << endl;
		cout << "Gebe nichts ein, wenn du das Feld unbearbeitet lassen willst." << endl;
		cout << "titel";
		getline(cin, titel);
		cout << "beschreibung";
		getline(cin, beschreibung);
		cout << "bearbeiter";
		getline(cin, bearbeiter);

		if (titel != "") aufgabe.titel = titel;
		if (beschreibung != "") aufgabe.beschreibung = beschreibung;
		if (bearbeiter != "") aufgabe.bearbeiter = bearbeiter;
		KanBamBoard.alleAufgaben[aufgabenNummer] = aufgabe;
	}

	/** 
	 * Überschreibt Objekt Aufgabe mit einem leeren neu angelegtem Objekt des Types Aufgabe
	 * 
	 * @param int aufgabenNummer
	 * Nummer der Aufgabe aus dem Array im KanBam struct angelegtem "alleAufgaben[]"
	 * @return void
	 */
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

	void aufgabeVerschieben(int aufgabenNummer, string spalte) {
		KanBamBoard.alleAufgaben[aufgabenNummer].spalte = spalte;
	}

}KanBamBoard;


// METHODEN
void gebeAlleAufgabenAusSpalteAus(string spalte) {
	cout << spalte << endl;
	for (int i = 0; i < AUFGABENMAXIMUM; i++) {
		if (KanBamBoard.alleAufgaben[i].spalte == spalte) {
			cout << KanBamBoard.alleAufgaben[i].nummer << ", ";
			cout << KanBamBoard.alleAufgaben[i].titel << ", ";
			cout << KanBamBoard.alleAufgaben[i].beschreibung << ", ";
			cout << KanBamBoard.alleAufgaben[i].bearbeiter << ", ";
			cout << KanBamBoard.alleAufgaben[i].spalte << endl;
		}
	}
}


// MAIN
int main() {
	// DEKLARIEREN
	string eingabeName;
	string eingabeBeschreibung;
	string eingabeBearbeiter;
	int benutzerEingabe;
	int eingabeAufgabenNummer;

	//Dauerschleife
	while (true) {
		//INITIALISIEREN
		eingabeName = "";
		eingabeBeschreibung = "";
		eingabeBearbeiter = "";
		eingabeAufgabenNummer = 0;
		benutzerEingabe = 0;

		// AUSGABE
		gebeAlleAufgabenAusSpalteAus("Backlog");
		gebeAlleAufgabenAusSpalteAus("ToDo");
		gebeAlleAufgabenAusSpalteAus("Doing");
		gebeAlleAufgabenAusSpalteAus("Done");

		// Benutzerschnittstelle
		  // Ausgabe: Benutzereingaben
		cout << "1. Aufgabe erstellen" << endl;
		cout << "2. Aufgabe bearbeiten" << endl;
		cout << "3. Aufgabe loeschen" << endl;
		cout << "4. Aufgabe verschieben" << endl;
		cout << "5. Aufgabe lesen" << endl;

		//Eingabe: Benutzereingaben
		cin >> benutzerEingabe;
		switch (benutzerEingabe) {
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
				cout << "Welche Aufgabe?" << endl;
				cin >> eingabeAufgabenNummer;
				KanBamBoard.aufgabeBearbeiten(eingabeAufgabenNummer);
				break;
			}
			case 3: {
				cout << "3" << endl;
				cout << "Welche Aufgabe?" << endl;
				cin >> eingabeAufgabenNummer;
				KanBamBoard.aufgabeLöschen(eingabeAufgabenNummer);
				break;
			}
			case 4: {
				cout << "4" << endl;
				KanBamBoard.aufgabeVerschieben(1, "ToDo");
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

