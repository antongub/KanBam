/**
* KanBam
*
* A KanBan Board which is only executable in the Command Line Interface
*
* @version 0.0.1 vom 30.04.2018
* @author Anton Gubenko && Justin Zien
*/

//INCLUDE VERZEICHNISSE
#include "stdafx.h"
#include "conio.h"
#include "iostream"
#include "string"

// DEFINES ANLEGEN
#define AUFGABENMAXIMUM 999
#define SPALTENANZAHL 4

// USING VERZEICHNISSE
using namespace std;


// GLOBALE VARIABLEN INITIALISIEREN
int aufgabenAnzahl = 0;


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
	const string alleSpalten[SPALTENANZAHL] = { "Backlog", "ToDo", "Doing", "Done" };
	const int spaltenBreite = 15;

	/*
	 * Holt die Werte aus dem @param und erstellt damit ein neues Objekt "Aufgabe".
	 * </br>
	 * Dieses Objekt wird dann automatisch mit der Zahl 'aufgabenAnzahl'+1 gesetzt und die Spalte ist standardmäßig zu 'Backlog' gesetzt.
	 * </br>
	 * Das Objekt wird automatisch in den alleAufgaben[] Array vom KanBam struct hinzugefügt.
	 * 
	 * @param string titel, string beschreibung, string bearbeiter
	 * @return void
	 */
	void aufgabeHinzufuegen(string titel, string beschreibung, string bearbeiter) {
		Aufgabe neueAufgabe;
		aufgabenAnzahl++;
		neueAufgabe.titel = titel;
		neueAufgabe.beschreibung = beschreibung;
		neueAufgabe.bearbeiter = bearbeiter;
		neueAufgabe.spalte = "Backlog";
		neueAufgabe.nummer = aufgabenAnzahl;
		alleAufgaben[aufgabenAnzahl] = neueAufgabe;
	}

	/*
	 * Fragt den Nutzer welche der drei Felder vom Aufgaben Objekt er ändern will
	 * (titel, beschreibung, bearbeiter)
	 * Und überschreibt die Werte in die Aufgabe
	 * 
	 * @param int aufgabenNummer
	 * Nummer der Aufgabe aus dem Array im KanBam struct angelegtem 'alleAufgaben[]'
	 * @return void
	 */
	void aufgabeBearbeiten(int aufgabenNummer, string titel, string beschreibung, string bearbeiter) {
			if (titel != "") alleAufgaben[aufgabenNummer].titel = titel;
			if (beschreibung != "") alleAufgaben[aufgabenNummer].beschreibung = beschreibung;
			if (bearbeiter != "") alleAufgaben[aufgabenNummer].bearbeiter = bearbeiter;
	}

	/** 
	  * Überschreibt Objekt Aufgabe mit einem leeren neu angelegtem Objekt des Types Aufgabe
	  * 
	  * @param int aufgabenNummer
	  * Nummer der Aufgabe aus dem Array im KanBam struct angelegtem 'alleAufgaben[]'
	  * @return void
	  */
	void aufgabeLöschen(int aufgabenNummer) {
		Aufgabe leereAufgabe;
		KanBamBoard.alleAufgaben[aufgabenNummer] = leereAufgabe;
	}

	/*
	 * Gibt folgende Werte aus einer Aufgabe mit der @param Nummer aus:
	 * Nummer, Titel, Beschreibung, Bearbeiter, Spalte.
	 * 
	 * @param int aufgabenNummer
	 * Nummer der Aufgabe aus dem Array im KanBam struct angelegtem 'alleAufgaben[]'
	 * @return void
	 */
	void aufgabeLesen(int aufgabenNummer) {
		Aufgabe aufgabe = KanBamBoard.alleAufgaben[aufgabenNummer];
		cout << "Nummer: " << aufgabe.nummer << endl;
		cout << "Titel: " << aufgabe.titel << endl;
		cout << "Beschreibung: " << aufgabe.beschreibung << endl;
		cout << "Bearbeiter: " << aufgabe.bearbeiter << endl;
		cout << "Spalte: " << aufgabe.spalte << endl;
	}

	/*
	 * Ändert den Wert, der in 'spalte' eines Aufgaben Objektes geschrieben ist.
	 * In was es umgeändert werden soll wird abgefragt.
	 * 
	 * @param int aufgabenNummer
	 * Nummer der Aufgabe aus dem Array im KanBam struct angelegtem 'alleAufgaben[]'
	 * @return void
	 */
	void aufgabeVerschieben(int aufgabenNummer, string spalte) {
		if (aufgabenNummer <= 0 || aufgabenNummer > aufgabenAnzahl) return;
		for (int i = 0; i < SPALTENANZAHL; i++) {
			if (spalte == KanBamBoard.alleSpalten[i]) {
				KanBamBoard.alleAufgaben[aufgabenNummer].spalte = spalte;
			}
		}
	}

}KanBamBoard;


// METHODEN
/*
 * Gibt folgende Eigenschaften aller Aufgaben Objekte aus, die den gleichen Wert in 'spalte' stehen haben, wie @param:
 * Nummer, Titel, Beschreibung, Bearbeier, Spalte.
 * 
 * @param string spalte
 * @return void
 */
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

void eingabe(int wert) {
	cin >> wert;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

void eingabe(string wert) {
	cin >> wert;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

// MAIN
int main() {
	// DEKLARIEREN
	string eingabeName;
	string eingabeBeschreibung;
	string eingabeBearbeiter;
	string eingabeSpalte;
	int benutzerEingabe;
	int eingabeAufgabenNummer;

	// DAUERSCHLEIFE
	while (true) {
		// INITIALISIEREN
		eingabeName = "";
		eingabeBeschreibung = "";
		eingabeBearbeiter = "";
		eingabeAufgabenNummer = 0;
		eingabeSpalte = "";
		benutzerEingabe = 0;

		// AUSGABE
		  // AUSGABE: Alle Spalten mit Inhalt
		gebeAlleAufgabenAusSpalteAus("Backlog");
		gebeAlleAufgabenAusSpalteAus("ToDo");
		gebeAlleAufgabenAusSpalteAus("Doing");
		gebeAlleAufgabenAusSpalteAus("Done");
		cout << endl;

		// BENUTZERSCHNITTSTELLE
		  // AUSGABE: Benutzereingaben
		cout << "1. Neue Aufgabe erstellen" << endl;
		cout << "2. Aufgabe bearbeiten" << endl;
		cout << "3. Aufgabe loeschen" << endl;
		cout << "4. Aufgabe verschieben" << endl;
		cout << "5. Aufgabe lesen" << endl;

		// EINGABE: Benutzereingaben
		cin >> benutzerEingabe;
		switch (benutzerEingabe) {
			// Aufgabe erstellen:
			case 1: {
				cin.ignore();
				cout << "Name" << endl;
				getline(cin, eingabeName);
				cout << "Beschreibung" << endl;
				getline(cin, eingabeBeschreibung);
				cout << "Bearbeiter" << endl;
				getline(cin, eingabeBearbeiter);
				KanBamBoard.aufgabeHinzufuegen(eingabeName, eingabeBeschreibung, eingabeBearbeiter);
				cout << endl << "Erfolgreich erstellt!";
				break;
			}
			// Aufgabe bearbeiten:
			case 2: {
				cout << "Welche Aufgabe?" << endl;
				cout << "(Gebe die Index Nummer dafuer an)" << endl;
				cin >> eingabeAufgabenNummer;
				if (eingabeAufgabenNummer > aufgabenAnzahl || cin.fail() || KanBamBoard.alleAufgaben[eingabeAufgabenNummer].nummer <= 0) {
					cout << endl << "Fehlerhafte Eingabe.";
					_getch();
					break;
				}
				cin.ignore();
				cout << "Was willst du bearbeiten?" << endl;
				cout << "Gebe nichts ein, wenn du das Feld unbearbeitet lassen willst." << endl;
				cout << "titel: ";
				getline(cin, eingabeName);
				cout << "beschreibung: ";
				getline(cin, eingabeBeschreibung);
				cout << "bearbeiter: ";
				getline(cin, eingabeBearbeiter);

				KanBamBoard.aufgabeBearbeiten(eingabeAufgabenNummer, eingabeName, eingabeBeschreibung, eingabeBearbeiter);
				cout << endl << "Erfolgreich bearbeitet!";
				break;
			}
			// Aufgabe löschen
			case 3: {
				cout << "Welche Aufgabe?" << endl;
				cin >> eingabeAufgabenNummer;
				if (eingabeAufgabenNummer > aufgabenAnzahl || cin.fail() || KanBamBoard.alleAufgaben[eingabeAufgabenNummer].nummer <= 0) {
					cout << endl << "Fehlerhafte Eingabe.";
					_getch();
					break;
				}else {
					KanBamBoard.aufgabeLöschen(eingabeAufgabenNummer);
					cout << endl << "Erfolgreich geloescht!";
					_getch();
					break;
				}
			}
			// Aufgabe verschieben
			case 4: {
				cout << "Welche Aufgabe?" << endl;
				cin >> eingabeAufgabenNummer;
				if (eingabeAufgabenNummer > aufgabenAnzahl || cin.fail() || KanBamBoard.alleAufgaben[eingabeAufgabenNummer].nummer <= 0) {
					cout << endl << "Fehlerhafte Eingabe.";
					_getch();
					break;
				}
				cout << "Wohin verschieben?" << endl;
				cin >> eingabeSpalte;


				for (int i = 0; i < SPALTENANZAHL; i++) {
					if (eingabeSpalte == KanBamBoard.alleSpalten[i]) {
						KanBamBoard.aufgabeVerschieben(eingabeAufgabenNummer, eingabeSpalte);
						cout << endl << "Erfolgreich verschoben!";
						_getch();
						break;
					}else if ( i == SPALTENANZAHL -1) {
						cout << endl << "Fehlerhafte Eingabe.";
						_getch();
						break;
					}
				}
				break;
			}
			// Aufgabe lesen
			case 5: {
				cout << "Welche Aufgabe?" << endl;
				int aufgabelesen;
				cin >> aufgabelesen;
				if (aufgabelesen <= AUFGABENMAXIMUM && KanBamBoard.alleAufgaben[aufgabelesen].nummer > 0) {
					KanBamBoard.aufgabeLesen(aufgabelesen);
				}else {
					cout << endl << "Fehlerhafte Eingabe.";
				}
				_getch();
				break;
			}
			// Default
			default: {
				if (!benutzerEingabe || benutzerEingabe!= 1 || benutzerEingabe != 2 || benutzerEingabe != 3 || benutzerEingabe != 4 || benutzerEingabe != 5) {
					cout << endl << "Fehlerhafte Eingabe.";
					_getch();
				}
				break;
			}
		}

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		system("CLS");
	}
	return 0;
}