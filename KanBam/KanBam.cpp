//INCLUDE VERZEICHNISSE
#include "stdafx.h"
#include "conio.h"
#include "iostream"
#include "string"
#include "sstream"
#include "sqlite3.h"

// DEFINES ANLEGEN
#define SPALTENANZAHL 4			// Anzahl der Spalten  // Standard 4  // (Backlog, ToDo, Doing, Done)
#define SPALTENBREITE 15		// Breite einer Spalte  // Standard 15
#define BOARDLEERZEICHEN 2		// bei der Anzeige des Boards: Leerzeichen nach der Auflistung der Aufgaben  // Standard 2
#define AUFGABENMAX 99+1		// Maximum Anzahl+1 der Aufgaben, die existieren dürfen // NICHT BEI BESTEHENDER EXISTENZ DER DATENBANK ÄNDERN

// USING VERZEICHNISSE
using namespace std;

// Funktionsprototypen
string gebeAufSpaltenbreiteAngepasstWieder(string wert);
int sqlite_aufgabenAnzahlHinzufügen(void *unbenutzt, int spaltenAnzahl, char **reihenInhalt, char **spaltenName);
int sqlite_boardlängeKallibrieren(void *unbenutzt, int spaltenAnzahl, char **reihenInhalt, char **spaltenName);
int sqlite_ausgabeInhalt(void *unbenutzt, int spaltenAnzahl, char **reihenInhalt, char **spaltenName);
int sqlite_ausgabeSpalteUndInhalt(void *unbenutzt, int spaltenAnzahl, char **reihenInhalt, char **spaltenName);

//GLOBALE VARIABLEN
int länge = 0;					// Hilft bei der Berechnung der Board länge (/y achse) // besitzt letzendlich den Wert der höchsten Aufgaben aller Spalten
int aufgabenAnzahl = 0;			// Nötig um das Maximum der zu erstellenden Aufgaben herauszufinden


//************************************
// Method:    gebeAufSpaltenbreiteAngepasstWieder
// Returns:   std::string
// Parameter: string wert
//
//Ein String wird zurückgegeben der angepasst wird auf den define Wert 'SPALTENBREITE'.
//************************************
string gebeAufSpaltenbreiteAngepasstWieder(string wert) {
	string neuerWert(SPALTENBREITE, ' ');
	int wertlänge = wert.length();
	for (int inkrement = 0; inkrement < SPALTENBREITE; inkrement++) {
		if (wert.length() <= SPALTENBREITE) {
			if (inkrement < wertlänge) {
				neuerWert[inkrement] = wert[inkrement];
			}
			else if (inkrement >= wertlänge) {
				neuerWert[inkrement] = ' ';
			}
		}
		else {
			if (inkrement < SPALTENBREITE - 3) {	// -3 für die Anzahl von "..."/Punkten ("testtes...")
				neuerWert[inkrement] = wert[inkrement];
			}
			else {
				neuerWert[inkrement] = '.';
			}
		}
	}
	return neuerWert;
}

//************************************
// Method:    sqlite_ausgabeSpalteUndInhalt
// Returns:   int
// Parameter: void * unbenutzt - Unbenutzt in dem Fall, siehe sqlite3_exec dokumentation
// Parameter: int spaltenAnzahl - Anzahl der Spalten die abgefragt werden aus der SQLITE Tabelle
// Parameter: char * * reihenInhalt - Inhalte der Reihen die abgefragt werden in der SQLITE Tabelle
// Parameter: char * * spaltenName - Alle Abgefragten Spalten Namen der SQLITE Tabelle
//
// Dient als Rückgabe-Funktions Parameter für das ausführen von SQLITE Befehle.
// Gibt bei einem SELECT sqlite statement Werte mit Spaltenname aus
// Ausgabe: "{spaltenName} : {reihenInhalt}"
//************************************
int sqlite_ausgabeSpalteUndInhalt(void *unbenutzt, int spaltenAnzahl, char **reihenInhalt, char **spaltenName) {
	for (int inkrement = 0; inkrement < spaltenAnzahl; inkrement++) {
		if (strlen(reihenInhalt[inkrement]) > 0)
			cout << spaltenName[inkrement] << ": " << reihenInhalt[inkrement] << endl;
	}
	return 0;
}

//************************************
// Method:    sqlite_ausgabeInhalt
// Returns:   int
// Parameter: void * unbenutzt - Unbenutzt in dem Fall, siehe sqlite3_exec dokumentation
// Parameter: int spaltenAnzahl - Anzahl der Spalten die abgefragt werden aus der SQLITE Tabelle
// Parameter: char * * reihenInhalt - Inhalte der Reihen die abgefragt werden in der SQLITE Tabelle
// Parameter: char * * spaltenName - Unbenutzt in dem Fall. Standard: Alle Abgefragten Spalten Namen der SQLITE Tabelle
//
// Dient als Rückgabe-Funktions Parameter für das ausführen von SQLITE Befehle.
// Gibt die Inhalte eines SQLITE Statements aus.
// Ausgabe: "#{reihenInhalt[0]} {reihenInhalt[1]} {reihenInhalt[2]} {..}"
//************************************
int sqlite_ausgabeInhalt(void *unbenutzt, int spaltenAnzahl, char **reihenInhalt, char **spaltenName) {
	string ausgabeText = "#";
	for (int inkrement = 0; inkrement < spaltenAnzahl; inkrement++) {
		ausgabeText += reihenInhalt[inkrement];
		if (inkrement != spaltenAnzahl - 1)
			ausgabeText += " ";
	}
	cout << gebeAufSpaltenbreiteAngepasstWieder(ausgabeText);
	return 0;
}

//************************************
// Method:    sqlite_boardlängeKallibrieren
// Returns:   int
// Parameter: void * unbenutzt - Unbenutzt in dem Fall, siehe sqlite3_exec dokumentation
// Parameter: int spaltenAnzahl - Anzahl der Spalten die abgefragt werden aus der SQLITE Tabelle
// Parameter: char * * reihenInhalt - Inhalte der Reihen die abgefragt werden in der SQLITE Tabelle
// Parameter: char * * spaltenName - Unbenutzt in dem Fall. Standard: Alle Abgefragten Spalten Namen der SQLITE Tabelle
//
// Dient als Funktions Parameter für das ausführen von SQLITE Befehle.
// Setzt die global angelegte Variable "länge", sobald der gelieferte Wert größer ist, auf den gelieferten wert.
//************************************
int sqlite_boardlängeKallibrieren(void *unbenutzt, int spaltenAnzahl, char **reihenInhalt, char **spaltenName) {
	for (int inkrement = 0; inkrement < spaltenAnzahl; inkrement++) {
		if (atoi(reihenInhalt[inkrement]) > länge)
			länge = atoi(reihenInhalt[inkrement]);
	}
	return 0;
}

//************************************
// Method:    sqlite_aufgabenAnzahlHinzufügen
// Returns:   int
// Parameter: void * unbenutzt - Unbenutzt in dem Fall, siehe sqlite3_exec dokumentation
// Parameter: int spaltenAnzahl - Unbenutzt in dem Fall. Standard: Anzahl der Spalten die abgefragt werden aus der SQLITE Tabelle
// Parameter: char * * reihenInhalt - Inhalte der Reihen die abgefragt werden in der SQLITE Tabelle
// Parameter: char * * spaltenName - Unbenutzt in dem Fall. Standard: Alle Abgefragten Spalten Namen der SQLITE Tabelle
//
// Dient als Funktions Parameter für das ausführen von SQLITE Befehle.
// Weist die Globale Variable aufgabenAnzahl zu dem ersten rückgabeParameter des reihenInhaltes zu.
//************************************
int sqlite_aufgabenAnzahlHinzufügen(void *unbenutzt, int spaltenAnzahl, char **reihenInhalt, char **spaltenName) {
	aufgabenAnzahl = atoi(reihenInhalt[0]);
	return 0;
}

//************************************
// Method:    main
// Returns:   int
//
// Einstiegspunkt der Anwendung
//************************************
int main() {
	// DEKLARIEREN & INITIALISIEREN//
	// DEKLARIEREN: Benutzereingaben
	int eingabeID;
	string eingabeTitel, eingabeBeschreibung, eingabeBearbeiter, eingabeSpalte;
	// INITIALISIEREN: Benutzereingaben
	int benutzerEingabe = 0;
	// DEKLARIEREN: Datenbankdaten
	sqlite3 *datenbank;
	int datenbankStatus;
	const char *sqlBefehl;
	stringstream sqlStringstream;
	string sqlString;
	// DEKLARIEREN: Boarddaten
	int stat;

	// DATENBANK VERBINDUNG //
	// DATENBANK: Datenbank öffnen
	datenbankStatus = sqlite3_open("board.db", &datenbank);
	// Bei Fehlschlag wir das Programm mit einer Fehlermeldung beendet
	if (datenbankStatus) {
		cerr << "Kann Datenbank nicht öffnen" << endl << sqlite3_errmsg(datenbank);
		_getch();
		return 0;
	}
	// DATENBANK: Initiale Tabellen Erstellung
	// SQL Befehl: erstellen
	sqlBefehl = "CREATE TABLE IF NOT EXISTS `Board` ("  \
		"`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE," \
		"`titel`	TEXT," \
		"`beschreibung`	TEXT," \
		"`bearbeiter`	TEXT," \
		"`spalte`	TEXT NOT NULL DEFAULT 'Backlog');";
	// SQL Befehl: ausführen
	datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, sqlite_ausgabeSpalteUndInhalt, 0, 0);


	// Dauerschleife, solange der Benutzer bei abfrage nicht "9" eingibt
	while (benutzerEingabe != 9) {
		// ZUWEISUNG & INITIALISIEREN //
		// ZUWEISUNG: Benutzereingaben
		benutzerEingabe = 0;
		eingabeID = 0;
		eingabeTitel = "";
		eingabeBeschreibung = "";
		eingabeBearbeiter = "";
		eingabeSpalte = "";
		// ZUWEISUNG: Datenbankdaten
		sqlBefehl = "";
		sqlStringstream.str("");
		sqlString = "";
		// DEKLARIEREN & INITIALISIEREN & ZUWEISUNG: Boarddaten
		const string alleSpalten[SPALTENANZAHL] = { "Backlog", "ToDo", "Doing", "Done" };
		const int boardbreite = (SPALTENBREITE * SPALTENANZAHL) + SPALTENANZAHL + 1;
		const int spaltenStartPositionenArrayLänge = SPALTENANZAHL + 1;
		int aufgabenID = 1;
		int lastsavedNumber[SPALTENANZAHL];
		int spaltenStartPositionen[spaltenStartPositionenArrayLänge];
		spaltenStartPositionen[0] = 0;
		for (int inkrement = 0; inkrement < SPALTENANZAHL; inkrement++) { // Jede lastsavedNumber Arraystelle mit dem Wert 1 füllen
			lastsavedNumber[inkrement] = 1;
		}
		for (int inkrement = 1; inkrement <= SPALTENANZAHL; inkrement++) { // Jede spaltenStartPositionen Arraystelle mit ausgerechneten Werten füllen
			spaltenStartPositionen[inkrement] = ((inkrement)* SPALTENBREITE) + (inkrement);
		}
		//ZUWEISUNG: GLobale Variable
		länge = 0;


		// Boardlänge ausrechnen
		// höchstes Auftreten der Aufgaben aus allen Spalten herausfinden
		for (int inkrement = 0; inkrement < SPALTENANZAHL; inkrement++) {
			// SQL Befehl: erstellen
			sqlStringstream.str("");
			sqlStringstream << "SELECT MAX (mycount) FROM (SELECT spalte, COUNT(spalte) mycount FROM Board WHERE spalte = '" << alleSpalten[inkrement] << "');";
			sqlString = sqlStringstream.str();
			sqlBefehl = sqlString.c_str();
			// SQL Befehl: ausführen
			datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, sqlite_boardlängeKallibrieren, 0, 0);
		}
		int größteAufgabenAnzahlAusSpalten = länge;
		const int boardlänge = 4 + größteAufgabenAnzahlAusSpalten + BOARDLEERZEICHEN;		// "4" für die ersten 3 spalten (====, {spalten}, |--|--|) und 1 für die letzte (=====)


		// Aufgabenanzahl herausfinden
		// SQL Befehl: erstellen
		sqlStringstream.str("");
		sqlStringstream << "SELECT seq FROM sqlite_sequence;";
		sqlString = sqlStringstream.str();
		sqlBefehl = sqlString.c_str();
		// SQL Befehl: ausführen
		datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, sqlite_aufgabenAnzahlHinzufügen, 0, 0);


		// AUSGABE //
		// AUSGABE: EINLEITUNG
		for (int inkrement = (((SPALTENBREITE * SPALTENANZAHL + SPALTENANZAHL + 1) / 2) - 5); inkrement > 0; inkrement--) {
			cout << " ";
		}
		cout << "KanBam-Board" << endl << endl;

		// AUSGABE: BOARD
		for (int reihe = 0; reihe < boardlänge; reihe++) {
			for (int spalte = 0; spalte < boardbreite; spalte++) {
				// Erste und letzte Reihe // //  ============================================  //
				if (reihe == 0 || reihe == boardlänge - 1) {
					cout << "=";
				}

				// Zweite Reihe, aka. Spalten auflistung // // |Backlog   |ToDo      |Doing      |Done      |  //
				if (reihe == 1) {
					for (int inkrement = 0; inkrement < spaltenStartPositionenArrayLänge; inkrement++) {
						// Spalten-Trennstriche:
						if (spalte == spaltenStartPositionen[inkrement]) {
							cout << "|";
						}
						// Spaltenname:
						else if (spalte == spaltenStartPositionen[inkrement] + 1) {
							cout << gebeAufSpaltenbreiteAngepasstWieder(alleSpalten[inkrement]);
						}
					}
				}

				// Dritte Reihe, aka. Trennreihe // //  |----------|----------|----------|----------|  //
				if (reihe == 2) {
					for (int inkrement = 0; inkrement < spaltenStartPositionenArrayLänge; inkrement++) {
						// Spalten-Trennstriche:
						if (spalte == spaltenStartPositionen[inkrement]) {
							cout << "|";
							inkrement = spaltenStartPositionenArrayLänge;
						}
						// Zeichenausgabe:
						else if (inkrement == spaltenStartPositionenArrayLänge - 1) {
							cout << "-";
						}
					}
				}

				// Reihe mit der auflistung der Aufgaben // //  |{id} {aufgabe}|#2 karate|#4 hochzei...|#0 test| //
				if (reihe >= 3 && reihe != boardlänge - 1) {
					for (int inkrement = 0; inkrement < spaltenStartPositionenArrayLänge; inkrement++) {
						// Spalten-Trennstriche:
						if (spalte == spaltenStartPositionen[inkrement]) {
							cout << "|";
						}
						// Aufgabenlistung:
						else if (spalte == spaltenStartPositionen[inkrement] + 1) {
							if (inkrement < SPALTENANZAHL) {
								stat = SQLITE_DONE;
								for (int aufgabenID = lastsavedNumber[inkrement]; stat == SQLITE_DONE && aufgabenID <= AUFGABENMAX; aufgabenID++) {
									// SQL Befehl: erstellen
									sqlStringstream.str("");
									sqlStringstream << "SELECT id,titel FROM Board WHERE (spalte='" << alleSpalten[inkrement] << "') AND (id= " << aufgabenID << " );";
									sqlString = sqlStringstream.str();
									sqlBefehl = sqlString.c_str();
									// SQL Befehl: ausführen
									datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, sqlite_ausgabeInhalt, 0, 0);

									sqlite3_stmt *stmt2;
									sqlite3_prepare(datenbank, sqlBefehl, -1, &stmt2, NULL);
									stat = sqlite3_step(stmt2);
									lastsavedNumber[inkrement] = aufgabenID + 1;

									if (aufgabenID == AUFGABENMAX && stat == SQLITE_DONE) {
										cout << gebeAufSpaltenbreiteAngepasstWieder(" ");
										lastsavedNumber[inkrement] = AUFGABENMAX;
									}

								}
							}
						}
					}
				}
			}
			cout << endl;
		}

		// BENUTZERSCHNITTSTELLE //
		// AUSGABE: Benutzereingaben
		cout << "1. Aufgabe erstellen" << endl;
		cout << "2. Aufgabe bearbeiten" << endl;
		cout << "3. Aufgabe loeschen" << endl;
		cout << "4. Aufgabe verschieben" << endl;
		cout << "5. Aufgabe lesen" << endl;
		cout << "9. Programm beenden" << endl;

		// EINGABE //
		// EINGABE: Benutzereingaben
		cin >> benutzerEingabe;
		switch (benutzerEingabe) {
			// Aufgabe erstellen:
		case 1: {
			if (aufgabenAnzahl >= AUFGABENMAX - 1) {
				cout << "Aufgaben Maximum erreicht" << endl;
				_getch();
				break;
			}
			// Benutzereingaben
			cin.ignore();
			// Dauerschleife, bis benutzer einen eingabeTitel eingibt
			do {
				cout << "Name" << endl;
				getline(cin, eingabeTitel);
				if (eingabeTitel == "") {
					cout << "Name ist Pflichtfeld." << endl;
				}
			} while (eingabeTitel == "");
			cout << "Beschreibung" << endl;
			getline(cin, eingabeBeschreibung);
			cout << "Bearbeiter" << endl;
			getline(cin, eingabeBearbeiter);

			// SQL Befehl: erstellen
			sqlStringstream << "INSERT INTO `Board` (titel,beschreibung,bearbeiter) " << "VALUES ( '" << eingabeTitel << "', '" << eingabeBeschreibung << "','" << eingabeBearbeiter << "');";
			sqlString = sqlStringstream.str();
			sqlBefehl = sqlString.c_str();
			// SQL Befehl: ausführen
			datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, sqlite_ausgabeSpalteUndInhalt, 0, 0);

			break;
		}
				// Aufgabe bearbeiten
		case 2: {
			// Benutzereingaben
			cout << "Welche Aufgabe?" << endl;
			cout << "(Gebe die Aufgaben-Nummer ein)" << endl;
			cin >> eingabeID;
			if (cin.fail() || eingabeID > aufgabenAnzahl) {
				cout << "Falsche Eingabe.";
				_getch();
				break;
			}

			cin.ignore();
			cout << "Was willst du bearbeiten?" << endl;
			cout << "1. Titel" << endl;
			cout << "2. Beschreibung" << endl;
			cout << "3. Bearbeiter" << endl;
			cin >> benutzerEingabe;
			if (cin.fail()) {
				cout << "Falsche Eingabe." << endl;
				_getch();
				break;
			}
			cin.ignore();

			// SQL Befehl: erstellen
			switch (benutzerEingabe) {
			case 1: {
				//Benutzereingabe
				// Dauerschleife, bis benutzer einen eingabeTitel eingibt
				do {
					cout << "Name: " << endl;
					getline(cin, eingabeTitel);
					if (eingabeTitel == "") {
						cout << "Name darf nicht leer sein." << endl;
					}
				} while (eingabeTitel == "");

				// SQL Befehl: erstellen
				sqlStringstream << "UPDATE `Board` set titel = '" << eingabeTitel << "' where ID = " << eingabeID << ";";
				sqlString = sqlStringstream.str();
				sqlBefehl = sqlString.c_str();

				break;
			}
			case 2: {
				//Benutzereingabe
				cout << "beschreibung: ";
				getline(cin, eingabeBeschreibung);

				// SQL Befehl: erstellen
				sqlStringstream << "UPDATE `Board` set beschreibung = '" << eingabeBeschreibung << "' where ID = " << eingabeID << ";";
				sqlString = sqlStringstream.str();
				sqlBefehl = sqlString.c_str();

				break;
			}
			case 3: {
				//Benutzereingabe
				cout << "bearbeiter: ";
				getline(cin, eingabeBearbeiter);

				// SQL Befehl: erstellen
				sqlStringstream << "UPDATE `Board` set bearbeiter = '" << eingabeBearbeiter << "' where ID = " << eingabeID << ";";
				sqlString = sqlStringstream.str();
				sqlBefehl = sqlString.c_str();

				break;
			}
			default: {
				cout << "Falsche Eingabe" << endl;
				break;
			}
			}

			// SQL Befehl: ausführen
			datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, sqlite_ausgabeSpalteUndInhalt, 0, 0);

			break;
		}
				// Aufgabe löschen
		case 3: {
			// Benutzereingabe
			cout << "Welche Aufgabe?" << endl;
			cout << "(Gebe die Aufgaben-Nummer ein)" << endl;
			cin >> eingabeID;
			if (cin.fail() || eingabeID > aufgabenAnzahl) {
				cout << "Falsche Eingabe." << endl;
				_getch();
				break;
			}

			// SQL Befehl: erstellen
			sqlStringstream << "DELETE from `Board` where ID = " << eingabeID << ";";
			sqlString = sqlStringstream.str();
			sqlBefehl = sqlString.c_str();
			// SQL Befehl: ausführen
			datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, sqlite_ausgabeSpalteUndInhalt, 0, 0);

			_getch();
			break;
		}
				// Aufgabe verschieben
		case 4: {
			// Benutzereingaben
			cout << "Welche Aufgabe?" << endl;
			cout << "(Gebe die Aufgaben-Nummer ein)" << endl;
			cin >> eingabeID;
			if (cin.fail() || eingabeID > aufgabenAnzahl) {
				cout << "Falsche Eingabe." << endl;
				_getch();
				break;
			}
			cout << "Wohin verschieben?" << endl;
			cout << "(Gebe den Namen der Spalte dafuer ein.)" << endl;
			cin >> eingabeSpalte;

			// Überprüft ob Eingabe gleich ist mit einem der Inhalte aus dem alleSpalten Array
			bool prüfwert = false;
			for (int spaltenName = 0; spaltenName < SPALTENANZAHL; spaltenName++) {
				if (eingabeSpalte == (alleSpalten[spaltenName])) {
					prüfwert = true;
				}
			}

			if (prüfwert == false || cin.fail()) {
				cout << "Falsche Eingabe." << endl;
				_getch();
				break;
			}


			// SQL Befehl: erstellen
			sqlStringstream << "UPDATE `Board` set spalte = '" << eingabeSpalte << "' where ID = " << eingabeID << ";";
			sqlString = sqlStringstream.str();
			sqlBefehl = sqlString.c_str();
			// SQL Befehl: ausführen
			datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, sqlite_ausgabeSpalteUndInhalt, 0, 0);

			_getch();
			break;
		}
				// Aufgabe lesen
		case 5: {
			// Benutzereingabe
			cout << "Welche Aufgabe?" << endl;
			cout << "(Gebe die Aufgaben-Nummer ein)" << endl;
			cin >> eingabeID;
			if (cin.fail() || eingabeID > aufgabenAnzahl) {
				cout << "Falsche Eingabe." << endl;
				_getch();
				break;
			}

			system("CLS");
			// SQL Befehl erstellen
			sqlStringstream << "SELECT * from `Board` where ID = " << eingabeID << ";";
			sqlString = sqlStringstream.str();
			sqlBefehl = sqlString.c_str();
			// SQL Befehl ausführen
			datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, sqlite_ausgabeSpalteUndInhalt, 0, 0);

			_getch();
			break;
		}
		default: {  // Default
			if (benutzerEingabe != 9) {
				cout << "Falsche Eingabe." << endl;
				_getch();
			}
			break;
		}
		}

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		system("CLS");
	}

	// DATENBANK: Datenbank schließen
	sqlite3_close(datenbank);

	return 0;
}

