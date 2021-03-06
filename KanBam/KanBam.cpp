//************************************
// KanBam
// Ein KanBan-Board, welches über die cmd ausgegeben wird.
//
// @version 1.0.0 vom 13.06.2018
// @author Anton Gubenko
//************************************


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

// USING VERZEICHNISSE
using namespace std;

// Funktionsprototypen
string gebeAufSpaltenbreiteAngepasstWieder(string wert);
int callback_aufgabenAnzahlHinzufügen(void *daten, int spaltenAnzahl, char **reihenInhalt, char **spaltenName);
int callback_boardlängeKallibrieren(void *daten, int spaltenAnzahl, char **reihenInhalt, char **spaltenName);
int callback_ausgabeInhalt(void *daten, int spaltenAnzahl, char **reihenInhalt, char **spaltenName);
int callback_ausgabeSpalteUndInhalt(void *daten, int spaltenAnzahl, char **reihenInhalt, char **spaltenName);


//************************************
// Methode:    gebeAufSpaltenbreiteAngepasstWieder
// Rückgabe:   string
// Parameter: string wert
//
//Ein String wird zurückgegeben der angepasst wird auf den define Wert 'SPALTENBREITE'.
//************************************
string gebeAufSpaltenbreiteAngepasstWieder(string wert) {
	string neuerWert(SPALTENBREITE, ' ');
	int wertlänge = wert.length();
	for (int spalte = 0; spalte < SPALTENBREITE; spalte++) {
		if (wert.length() <= SPALTENBREITE) {
			if (spalte < wertlänge) {
				neuerWert[spalte] = wert[spalte];
			}
			else if (spalte >= wertlänge) {
				neuerWert[spalte] = ' ';
			}
		}
		else {
			if (spalte < SPALTENBREITE - 3) {	// -3 für die Anzahl von "..."/Punkten ("testtes...")
				neuerWert[spalte] = wert[spalte];
			}
			else {
				neuerWert[spalte] = '.';
			}
		}
	}
	return neuerWert;
}

//************************************
// Methode:    callback_ausgabeSpalteUndInhalt
// Rückgabe:   int
// Parameter: void * daten - (Unbenutzt!) Besitzt Inhalt des 4ten sqlite_exec() Arguments
// Parameter: int spaltenAnzahl - Anzahl der Spalten die abgefragt werden aus der SQLITE Tabelle
// Parameter: char * * reihenInhalt - Inhalte der Reihen die abgefragt werden in der SQLITE Tabelle
// Parameter: char * * spaltenName - Alle Abgefragten Spalten Namen der SQLITE Tabelle
//
// Dient als Rückgabe-Funktions Parameter für das ausführen von SQLITE Befehle.
// Gibt bei einem SELECT sqlite statement Werte mit Spaltenname aus
// Ausgabe: "{spaltenName} : {reihenInhalt}"
//************************************
int callback_ausgabeSpalteUndInhalt(void *daten, int spaltenAnzahl, char **reihenInhalt, char **spaltenName) {
	for (int spalte = 0; spalte < spaltenAnzahl; spalte++) {
		if (strlen(reihenInhalt[spalte]) > 0)
			cout << spaltenName[spalte] << ": " << reihenInhalt[spalte] << endl;
	}
	return 0;
}

//************************************
// Methode:    callback_ausgabeInhalt
// Rückgabe:   int
// Parameter: void * SQLFunctionAufgerufen - Besitzt Inhalt des 4ten sqlite_exec() Arguments
// Parameter: int spaltenAnzahl - Anzahl der Spalten die abgefragt werden aus der SQLITE Tabelle
// Parameter: char * * reihenInhalt - Inhalte der Reihen die abgefragt werden in der SQLITE Tabelle
// Parameter: char * * spaltenName - (Unbenutzt!) Alle Abgefragten Spalten Namen der SQLITE Tabelle
//
// Dient als Rückgabe-Funktions Parameter für das ausführen von SQLITE Befehle.
// Gibt die Inhalte eines SQLITE Statements aus.
// Ausgabe: "#{reihenInhalt[0]} {reihenInhalt[1]} {reihenInhalt[2]} {..}"
//************************************
int callback_ausgabeInhalt(void *sqlFunktionAufgerufen, int spaltenAnzahl, char **reihenInhalt, char **spaltenName) {
	string ausgabeText = "#";
	for (int spalte = 0; spalte < spaltenAnzahl; spalte++) {
		ausgabeText += reihenInhalt[spalte];
		if (spalte != spaltenAnzahl - 1)
			ausgabeText += " ";
	}
	cout << gebeAufSpaltenbreiteAngepasstWieder(ausgabeText);
	(*(bool *)sqlFunktionAufgerufen) = true;
	return 0;
}

//************************************
// Methode:    callback_boardlängeKallibrieren
// Rückgabe:   int
// Parameter: void * länge - Besitzt Inhalt des 4ten sqlite_exec() Arguments
// Parameter: int spaltenAnzahl - Anzahl der Spalten die abgefragt werden aus der SQLITE Tabelle
// Parameter: char * * reihenInhalt - Inhalte der Reihen die abgefragt werden in der SQLITE Tabelle
// Parameter: char * * spaltenName - (Unbenutzt!) Alle Abgefragten Spalten Namen der SQLITE Tabelle
//
// Dient als Funktions Parameter für das ausführen von SQLITE Befehle.
// Setzt die global angelegte Variable "länge", sobald der gelieferte Wert größer ist, auf den gelieferten wert.
//************************************
int callback_boardlängeKallibrieren(void *länge, int spaltenAnzahl, char **reihenInhalt, char **spaltenName) {
	for (int spalte = 0; spalte < spaltenAnzahl; spalte++) {
		if (atoi(reihenInhalt[spalte]) > (*(int *)länge))
			(*(int *)länge) = atoi(reihenInhalt[spalte]);
	}
	return 0;
}

//************************************
// Methode:    callback_aufgabenAnzahlHinzufügen
// Rückgabe:   int
// Parameter: void * daten - (Unbenutzt!) Besitzt Inhalt des 4ten sqlite_exec() Arguments
// Parameter: int spaltenAnzahl - Unbenutzt in dem Fall. Standard: Anzahl der Spalten die abgefragt werden aus der SQLITE Tabelle
// Parameter: char * * reihenInhalt - Inhalte der Reihen die abgefragt werden in der SQLITE Tabelle
// Parameter: char * * spaltenName - (Unbenutzt!) Alle Abgefragten Spalten Namen der SQLITE Tabelle
//
// Dient als Funktions Parameter für das ausführen von SQLITE Befehle.
// Weist die Globale Variable aufgabenAnzahl zu dem ersten rückgabeParameter des reihenInhaltes zu.
//************************************
int callback_aufgabenAnzahlHinzufügen(void *aufgabenAnzahl, int spaltenAnzahl, char **reihenInhalt, char **spaltenName) {
	*((int*)aufgabenAnzahl) = atoi(reihenInhalt[0]);
	return 0;
}


//************************************
// Methode:    main
// Rückgabe:   int
//
// Einstiegspunkt der Anwendung
//************************************
int main() {
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
	bool sqlFunktionAufgerufen;

	// INITIALISIEREN: Boarddaten
	const string alleSpalten[SPALTENANZAHL] = { "Backlog", "ToDo", "Doing", "Done" };	// Alle verfügbaren Spalten // Zum ändern muss SPALTENZAHL ebenfalls angepasst werden
	const int boardbreite = (SPALTENBREITE * SPALTENANZAHL) + SPALTENANZAHL + 1;
	const int trennStrichAnzahl = SPALTENANZAHL + 1;
	int aufgabenAnzahl = 0;
	// DEKLARIEREN: Boarddaten
	int boardLesePosition[SPALTENANZAHL], trennStrichPosition[trennStrichAnzahl], boardlänge, höchsterWertAllerAufgaben;


	// DATENBANK VERBINDUNG //
		// DATENBANK: Datenbank öffnen
	datenbankStatus = sqlite3_open("board.db", &datenbank);
	// Bei Fehlschlag wird das Programm mit einer Fehlermeldung beendet:
	if (datenbankStatus) {
		cerr << "Kann Datenbank nicht öffnen" << endl << sqlite3_errmsg(datenbank);
		_getch();
		return 0;
	}
	// DATENBANK: Initiale Tabellen Erstellung
		// SQL Befehl: erstellen
	sqlBefehl = "CREATE TABLE IF NOT EXISTS `Board` ("  \
		"`id`	INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE," \
		"`titel`	TEXT," \
		"`beschreibung`	TEXT," \
		"`bearbeiter`	TEXT," \
		"`spalte`	TEXT DEFAULT 'Backlog');";
	// SQL Befehl: ausführen
	datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, 0, 0, 0);


	// Dauerschleife, solange der Benutzer bei abfrage nicht "9" eingibt
	while (benutzerEingabe != 9) {
		// ZUWEISUNGEN: WERTE AUF 0 SETZEN//
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

		// ZUWEISUNG: Boarddaten
		trennStrichPosition[0] = 0;
		höchsterWertAllerAufgaben = 0;
		// Jede zuletzGespeicherteNummer Arraystelle mit dem Wert 1 füllen
		for (int spalte = 0; spalte < SPALTENANZAHL; spalte++) {
			boardLesePosition[spalte] = 1;
		}
		// Jede trennStrichPositionen Arraystelle mit ausgerechneten Werten füllen
		// Fängt bei 1 an, da die erste Stelle des Arrays schon befüllt ist (mit 0)
		for (int spalte = 1; spalte <= SPALTENANZAHL; spalte++) {
			trennStrichPosition[spalte] = ((spalte)* SPALTENBREITE) + (spalte);
		}


		// Boardlänge ausrechnen
		// Höchstes Auftreten der Aufgaben aus allen Spalten herausfinden
		for (int spalte = 0; spalte < SPALTENANZAHL; spalte++) {
			// SQL Befehl: erstellen
			sqlStringstream.str("");
			sqlStringstream << "SELECT MAX (mycount) " \
				"FROM (SELECT spalte, COUNT(spalte) mycount " \
				"FROM Board WHERE spalte = '" << alleSpalten[spalte] << "');";
			sqlString = sqlStringstream.str();
			sqlBefehl = sqlString.c_str();
			// SQL Befehl: ausführen
			datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, callback_boardlängeKallibrieren, &höchsterWertAllerAufgaben, 0);
		}
		boardlänge = 4 + höchsterWertAllerAufgaben + BOARDLEERZEICHEN;		// Die Zahl "4" für die ersten 3 spalten (====, {spaltennamen}, |--|--|) und 1 für die letzte (=====)


		// Aufgabenanzahl herausfinden
		// SQL Befehl: erstellen
		sqlBefehl = "SELECT seq FROM sqlite_sequence;";
		// SQL Befehl: ausführen
		datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, callback_aufgabenAnzahlHinzufügen, &aufgabenAnzahl, 0);


		// AUSGABE: EINLEITUNG
		// Schreibt bis zur hälfte des Boardes (-5) leerzeichen hin, damit die kommende Ausgabe Mittig erscheint:
		// Variable position bekommt am Anfang den Wert der Mitte des Boardes zugewiesen
		for (int position = (((SPALTENBREITE * SPALTENANZAHL + SPALTENANZAHL + 1) / 2) - 5); position > 0; position--) {
			cout << " ";
		}
		cout << "KanBam-Board" << endl << endl;

		// AUSGABE: BOARD
		for (int reihe = 0; reihe < boardlänge; reihe++) {
			for (int spalte = 0; spalte < boardbreite; spalte++) {

				// Erste und letzte Reihe		 
				// =========  //
				if (reihe == 0 || reihe == boardlänge - 1) {
					cout << "=";
				}

				// Schleife für die Trennstrichpositionen
				for (int aktuelleSpalte = 0; aktuelleSpalte < trennStrichAnzahl; aktuelleSpalte++) {

					// Zweite Reihe, aka. Spalten auflistung 
					// |Backlog   |ToDo      |..  //
					if (reihe == 1) {
						// Spalten-Trennstriche:
						if (spalte == trennStrichPosition[aktuelleSpalte]) {
							cout << "|";
						}
						// Spaltennamen:
						else if (spalte == trennStrichPosition[aktuelleSpalte] + 1) {
							cout << gebeAufSpaltenbreiteAngepasstWieder(alleSpalten[aktuelleSpalte]);
						}
					}

					// Dritte Reihe, aka. Trennreihe 
					//  |----------|----------|..  //
					if (reihe == 2) {
						// Spalten-Trennstriche:
						if (spalte == trennStrichPosition[aktuelleSpalte]) {
							cout << "|";
							aktuelleSpalte = trennStrichAnzahl;
						}
						// Zeichenausgabe:
						else if (aktuelleSpalte == trennStrichAnzahl - 1) {
							cout << "-";
						}
					}

					// Reihe mit der auflistung der Aufgaben // 
					//  |#id aufgabe|#id aufgabe|..  //
					if (reihe >= 3 && reihe < boardlänge - (BOARDLEERZEICHEN + 1)) {
						// Spalten-Trennstriche:
						if (spalte == trennStrichPosition[aktuelleSpalte]) {
							cout << "|";
						}
						// Aufgabenlistung:
						else if (spalte == trennStrichPosition[aktuelleSpalte] + 1) {
							if (aktuelleSpalte < SPALTENANZAHL) {
								for (int aufgabenID = boardLesePosition[aktuelleSpalte]; aufgabenID <= aufgabenAnzahl + 1; aufgabenID++) {
									// SQL Befehl: erstellen
									sqlStringstream.str("");
									sqlStringstream << "SELECT id,titel FROM Board " \
										"WHERE (spalte='" << alleSpalten[aktuelleSpalte] << "') " \
										"AND (id= " << aufgabenID << " );";
									sqlString = sqlStringstream.str();
									sqlBefehl = sqlString.c_str();
									// SQL Befehl: ausführen
									sqlFunktionAufgerufen = false;
									datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, callback_ausgabeInhalt, &sqlFunktionAufgerufen, 0);
									if (sqlFunktionAufgerufen == false && aufgabenID >= aufgabenAnzahl) {
										cout << gebeAufSpaltenbreiteAngepasstWieder(" ");
										aufgabenID = aufgabenAnzahl + 1;
									}
									else if (sqlFunktionAufgerufen == true) {
										boardLesePosition[aktuelleSpalte] = aufgabenID + 1;
										aufgabenID = aufgabenAnzahl + 1;
									}
								}
							}
						}
					}

					// Reihe mit leeren Zeilen, definiert in BOARDLEERZEICHEN variable
					// |      |      |.. //
					if (reihe >= ((boardlänge - BOARDLEERZEICHEN) - 1) && reihe != boardlänge - 1) {
						// Spalten-Trennstriche:
						if (spalte == trennStrichPosition[aktuelleSpalte]) {
							cout << "|";
						}
						// Leerzeichen-füller:
						else if (spalte == trennStrichPosition[aktuelleSpalte] + 1) {
							cout << gebeAufSpaltenbreiteAngepasstWieder(" ");
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
				// Benutzereingaben
				cin.ignore();
				// Dauerschleife, bis benutzer einen eingabeTitel eingibt
				do {
					cout << "Titel" << endl;
					getline(cin, eingabeTitel);
					if (eingabeTitel == "") {
						cout << "Titel ist Pflichtfeld." << endl;
					}
				} while (eingabeTitel == "");
				cout << "Beschreibung" << endl;
				getline(cin, eingabeBeschreibung);
				cout << "Bearbeiter" << endl;
				getline(cin, eingabeBearbeiter);

				// SQL Befehl: erstellen
				sqlStringstream << "INSERT INTO `Board` (titel,beschreibung,bearbeiter) " \
					<< "VALUES ( '" << eingabeTitel << "', '" \
					<< eingabeBeschreibung << "','" \
					<< eingabeBearbeiter << "');";
				sqlString = sqlStringstream.str();
				sqlBefehl = sqlString.c_str();
				// SQL Befehl: ausführen
				datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, 0, 0, 0);

				break;
			}
				// Aufgabe bearbeiten:
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

				switch (benutzerEingabe) {
					// Aufgabe Bearbeiten: Titel
					case 1: {
						//Benutzereingabe
						// Dauerschleife, bis benutzer einen eingabeTitel eingibt
						do {
							cout << "Titel: " << endl;
							getline(cin, eingabeTitel);
							if (eingabeTitel == "") {
								cout << "Titel darf nicht leer sein." << endl;
							}
						} while (eingabeTitel == "");

						// SQL Befehl: erstellen
						sqlStringstream << "UPDATE `Board` set titel = '" << eingabeTitel \
							<< "' where ID = " << eingabeID << ";";
						sqlString = sqlStringstream.str();
						sqlBefehl = sqlString.c_str();

						break;
					}
							// Aufgabe Bearbeiten: Beschreibung
					case 2: {
						//Benutzereingabe
						cout << "beschreibung: ";
						getline(cin, eingabeBeschreibung);

						// SQL Befehl: erstellen
						sqlStringstream << "UPDATE `Board` set beschreibung = '" << eingabeBeschreibung \
							<< "' where ID = " << eingabeID << ";";
						sqlString = sqlStringstream.str();
						sqlBefehl = sqlString.c_str();

						break;
					}
							// Aufgabe Bearbeiten: Bearbeiter
					case 3: {
						//Benutzereingabe
						cout << "bearbeiter: ";
						getline(cin, eingabeBearbeiter);

						// SQL Befehl: erstellen
						sqlStringstream << "UPDATE `Board` set bearbeiter = '" << eingabeBearbeiter \
							<< "' where ID = " << eingabeID << ";";
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
				datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, 0, 0, 0);

				break;
			}
				// Aufgabe löschen:
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
				datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, 0, 0, 0);

				_getch();
				break;
			}
				// Aufgabe verschieben:
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
				sqlStringstream << "UPDATE `Board` set spalte = '" << eingabeSpalte \
					<< "' where ID = " << eingabeID << ";";
				sqlString = sqlStringstream.str();
				sqlBefehl = sqlString.c_str();
				// SQL Befehl: ausführen
				datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, 0, 0, 0);

				_getch();
				break;
			}
				// Aufgabe lesen:
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
				cout << "Aufgaben Informationen:" << endl << endl;

				// SQL Befehl erstellen
				sqlStringstream.str("");
				sqlStringstream << "SELECT * from `Board` where ID = " << eingabeID << ";";
				sqlString = sqlStringstream.str();
				sqlBefehl = sqlString.c_str();
				// SQL Befehl ausführen
				datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, callback_ausgabeSpalteUndInhalt, 0, 0);

				_getch();
				break;
			}
				// Default:
			default: {
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