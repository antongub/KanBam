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
#include "sqlite3.h"

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
		Aufgabe aufgabe;
		aufgabenAnzahl++;
		if (aufgabenAnzahl < 999) {
			aufgabe.titel = titel;
			aufgabe.beschreibung = beschreibung;
			aufgabe.bearbeiter = bearbeiter;
			aufgabe.spalte = "Backlog";
			aufgabe.nummer = aufgabenAnzahl;
			KanBamBoard.alleAufgaben[aufgabenAnzahl] = aufgabe;
		}
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
	void aufgabeVerschieben(int aufgabenNummer) {
		if(aufgabenNummer <= 0 || aufgabenNummer > aufgabenNummer) return;
		string spalte;
		cout << "Wohin verschieben?" << endl;
		cin >> spalte;
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
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

// MAIN
int main() {
	// DEKLARIEREN
	string eingabeName;
	string eingabeBeschreibung;
	string eingabeBearbeiter;
	int benutzerEingabe;
	int eingabeAufgabenNummer;



	// DATENBANK VERBINDUNG
	sqlite3 *datenbank;
	char datenbankDateiName[] = "board.db";
	int datenbankHandle;
	datenbankHandle = sqlite3_open(datenbankDateiName, &datenbank);

		// Überprüfe auf Verfügbarkeit
	if (datenbankHandle) {
		cout << stderr << "Can't open database: " << endl << sqlite3_errmsg(datenbank);
		return(0);
	}else {
		cout << stderr << " Opened database successfully." << endl;
	}

		// Tabelle erstellen
	char *errorMessage;
	const char *sqlStatement;
	sqlStatement = "CREATE TABLE `Board` ( \
		`id`	INTEGER NOT NULL DEFAULT 1 PRIMARY KEY AUTOINCREMENT UNIQUE, \
		`titel`	TEXT, \
		`beschreibung`	TEXT, \
		`bearbeiter`	TEXT);";
	datenbankHandle = sqlite3_exec(datenbank, sqlStatement, callback, 0, &errorMessage);
	if (datenbankHandle != SQLITE_OK) sqlite3_free(errorMessage);

	sqlite3_close(datenbank);


	// DAUERSCHLEIFE
	while (true) {
		// INITIALISIEREN
		eingabeName = "";
		eingabeBeschreibung = "";
		eingabeBearbeiter = "";
		eingabeAufgabenNummer = 0;
		benutzerEingabe = 0;

		// AUSGABE
		  // AUSGABE: Alle Spalten mit Inhalt
		gebeAlleAufgabenAusSpalteAus("Backlog");
		gebeAlleAufgabenAusSpalteAus("ToDo");
		gebeAlleAufgabenAusSpalteAus("Doing");
		gebeAlleAufgabenAusSpalteAus("Done");

		// BENUTZERSCHNITTSTELLE
		  // AUSGABE: Benutzereingaben
		cout << "1. Aufgabe erstellen" << endl;
		cout << "2. Aufgabe bearbeiten" << endl;
		cout << "3. Aufgabe loeschen" << endl;
		cout << "4. Aufgabe verschieben" << endl;
		cout << "5. Aufgabe lesen" << endl;

		// EINGABE: Benutzereingaben
		cin >> benutzerEingabe;
		switch (benutzerEingabe) {
			case 1: {	// Aufgabe erstellen:
				cin.ignore();
				cout << "1" << endl;
				cout << "Name" << endl;
				getline(cin, eingabeName);
				cout << "Beschreibung" << endl;
				getline(cin, eingabeBeschreibung);
				cout << "Bearbeiter" << endl;
				getline(cin, eingabeBearbeiter);
				KanBamBoard.aufgabeHinzufuegen(eingabeName, eingabeBeschreibung, eingabeBearbeiter);
				break;
			}
			case 2: {	// Aufgabe bearbeiten
				cout << "2" << endl;
				cout << "Welche Aufgabe?" << endl;
				cin >> eingabeAufgabenNummer;
				KanBamBoard.aufgabeBearbeiten(eingabeAufgabenNummer);
				break;
			}
			case 3: {	// Aufgabe löschen
				cout << "3" << endl;
				cout << "Welche Aufgabe?" << endl;
				cin >> eingabeAufgabenNummer;
				KanBamBoard.aufgabeLöschen(eingabeAufgabenNummer);
				break;
			}
			case 4: {	// Aufgabe verschieben
				cout << "4" << endl;
				cout << "Welche Aufgabe?" << endl;
				cin >> eingabeAufgabenNummer;
				KanBamBoard.aufgabeVerschieben(eingabeAufgabenNummer);
				break;
			}
			case 5: {	// Aufgabe lesen
				cout << "5" << endl;
				cout << "Welche Aufgabe?" << endl;
				int aufgabelesen;
				cin >> aufgabelesen;
				KanBamBoard.aufgabeLesen(aufgabelesen);
				break;
			}
			default: {	// Default
				cout << "Falsche Eingabe" << endl;
				break;
			}
		}

		_getch();
		system("CLS");
	}
	return 0;
}

