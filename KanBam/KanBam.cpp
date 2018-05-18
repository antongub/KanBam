//INCLUDE VERZEICHNISSE
#include "stdafx.h"
#include "conio.h"
#include "iostream"
#include "string"
#include "sstream"
#include "sqlite3.h"
#include "stdlib.h"

// DEFINES ANLEGEN
#define SPALTENANZAHL 4
#define SPALTENBREITE 15
#define BOARDLEERZEICHEN 2

// USING VERZEICHNISSE
using namespace std;

//GLOBALE VARIABLEN
int länge = 0;


int callback(void *data, int argc, char **argv, char **azColName) {
	for (int i = 0; i<argc; i++) {
		if(!strlen(argv[i]) == 0)
		cout << azColName[i] << ": " << argv[i] << endl;
	}
	return 0;
}

string gebeAufSpaltenbreiteAngepasstWieder(const string wert) {
	string newWert(SPALTENBREITE, ' ');
	int wertlänge = wert.length();
	if (wert.length() <= SPALTENBREITE) {
		for (int i = 0; i < SPALTENBREITE; i++) {
			if (i < wertlänge) {
				newWert[i] = wert[i];
			}else if (i >= wertlänge) {
				newWert[i] = ' ';
			}
		}
	}else {
		for (int i = 0; i < SPALTENBREITE; i++) {
			if (i < SPALTENBREITE - 3) {	// -3 für die Anzahl von "..."/Punkten ("testtes...")
				newWert[i] = wert[i];
			}else {
				newWert[i] = '.';
			}
		}
	}
	return newWert;
}

int ausgabe(void *data, int argc, char **argv, char **azColName) {
	string ausgabeText = "#";
	for (int i = 0; i < argc; i++) {
		ausgabeText += argv[i];
			if (i != argc - 1)
				ausgabeText += " ";
	}
	cout << gebeAufSpaltenbreiteAngepasstWieder(ausgabeText);
	return 0;
}


int boardlängeKallibrieren(void *data, int argc, char **argv, char **azColName) {
	for (int i = 0; i < argc; i++) {
		if (atoi(argv[i]) > länge)
			länge = atoi(argv[i]);
	}
	return 0;
}


// MAIN
int main(int argc, char** argv) {
	// DEKLARIEREN
	// Benutzereingaben
	int benutzerEingabe;
	int eingabeID;
	string eingabeTitel;
	string eingabeBeschreibung;
	string eingabeBearbeiter;
	string eingabeSpalte;
	// Datenbankdaten
	sqlite3 *datenbank;
	char *fehlerNachricht;
	int datenbankStatus;
	const char *sqlBefehl;
	stringstream sqlStringstream;
	string sqlString;
	// Boarddaten
	int stat;

	// DATENBANK VERBINDUNG
	// Datenbank öffnen
	datenbankStatus = sqlite3_open("board.db", &datenbank);
	if (datenbankStatus) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(datenbank));
		return 0;
	}
	// Standard Tabellen Initialisierung
	// SQLITE ignoriert, bei bestehender Existenz der Tabelle, das erstellen
	sqlBefehl = "CREATE TABLE `Board` ("  \
		"`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE," \
		"`titel`	TEXT," \
		"`beschreibung`	TEXT," \
		"`bearbeiter`	TEXT," \
		"`spalte`	TEXT NOT NULL DEFAULT 'Backlog');";
	// SQL Befehl ausführen
	datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, callback, 0, &fehlerNachricht);


	// DAUERSCHLEIFE
	while (true) {
		// INITIALISIEREN
		// Benutzereingaben
		benutzerEingabe = 0;
		eingabeID = 0;
		eingabeTitel = "";
		eingabeBeschreibung = "";
		eingabeBearbeiter = "";
		eingabeSpalte = "";
		// Datenbankdaten
		fehlerNachricht = 0;
		sqlBefehl = "";
		sqlStringstream.str("");
		sqlString = "";
		// Boarddaten
		const string alleSpalten[SPALTENANZAHL] = { "Backlog", "ToDo", "Doing", "Done" };
		const int boardbreite = (SPALTENBREITE * SPALTENANZAHL) + SPALTENANZAHL + 1;
		const int spaltenStartPositionenArrayLänge = SPALTENANZAHL + 1;
		int aufgabenID = 1;
		const int aufgabenMax = 99;
		//Deklarierung mit Initialisierung
		int lastsavedNumber[SPALTENANZAHL];
		for (int i = 0; i < SPALTENANZAHL; i++) {
			lastsavedNumber[i] = 1;
		}
		int spaltenStartPositionen[spaltenStartPositionenArrayLänge];
		spaltenStartPositionen[0] = 0;
		for (int i = 1; i <= SPALTENANZAHL; i++) {
			spaltenStartPositionen[i] = ((i)* SPALTENBREITE) + (i);
		}
		//GLobale Variable zuweisen
		länge = 0;


		// Figure out the Board length
		// need to get the highest occurence of tasks
		for (int i = 0; i < SPALTENANZAHL; i++) {
			// SQL Befehl erstellen
			sqlStringstream.str("");
			sqlStringstream << "SELECT MAX (mycount) FROM (SELECT spalte, COUNT(spalte) mycount FROM Board WHERE spalte = '" << alleSpalten[i] << "');";
			sqlString = sqlStringstream.str();
			sqlBefehl = sqlString.c_str();
			// SQL Befehl ausführen
			datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, boardlängeKallibrieren, 0, &fehlerNachricht);
		}
		int größteAufgabenAnzahlAusSpalten = länge;
		const int boardlänge = 4 + größteAufgabenAnzahlAusSpalten + BOARDLEERZEICHEN;		// "4" für die ersten 3 spalten (====, {spalten}, |--|--|) und für die letzte (=====)


		// AUSGABE: EINLEITUNG
			for (int i = (((SPALTENBREITE * SPALTENANZAHL + SPALTENANZAHL + 1) / 2) - 5); i > 0; i--) {
				cout << " ";
			}
		cout << "KanBam-Board" << endl << endl;

		// AUSGABE: BOARD
		for (int reihe = 0; reihe < boardlänge; reihe++) {
			//  ============================================  //
			if (reihe == 0 || reihe == boardlänge-1) {
				for (int i = 0; i < boardbreite; i++) {
					cout << "=";
				}
				cout << endl;
			}

			// |Backlog   |ToDo      |Doing      |Done      |  //
			if (reihe == 1) {
				for (int spalte = 0; spalte < boardbreite; spalte++) {
					//  |           |          |          |
					for (int k = 0; k < spaltenStartPositionenArrayLänge; k++) {
						if (spalte == spaltenStartPositionen[k]) {
							cout << "|";
						}
						else if (spalte == spaltenStartPositionen[k] + 1) {
							cout << gebeAufSpaltenbreiteAngepasstWieder(alleSpalten[k]);
						}
					}
				}
				cout << endl;
			}

			//  |----------|----------|----------|----------|  //
			if (reihe == 2) {
				for (int i = 0; i < boardbreite; i++) {
					for (int k = 0; k < spaltenStartPositionenArrayLänge; k++) {
						if (i == spaltenStartPositionen[k]) {
							cout << "|";
							k = spaltenStartPositionenArrayLänge;
						}
						else if (k == spaltenStartPositionenArrayLänge - 1) {	// zeichenausgabe
							cout << "-";
						}
					}
				}
				cout << endl;
			}

			//  |{id} {aufgabe}|#2 karate|#4 hochzei...|#0 test| //
			if (reihe >= 3 && reihe != boardlänge -1) {
				for (int spalte = 0; spalte < boardbreite; spalte++) {
					for (int k = 0; k < spaltenStartPositionenArrayLänge; k++) {
						//  |           |          |          |
						if (spalte == spaltenStartPositionen[k]) {
							cout << "|";
						}
						//  Karate      Hochzeit    test
						else if (spalte == spaltenStartPositionen[k] + 1) {
							if (k < SPALTENANZAHL) {
								stat = SQLITE_DONE;
								for(int aufgabenID = lastsavedNumber[k]; stat == SQLITE_DONE && aufgabenID <= aufgabenMax; aufgabenID++){
									// SQL Befehl erstellen
									sqlStringstream.str("");
									sqlStringstream << "SELECT id,titel FROM Board WHERE (spalte='" << alleSpalten[k] << "') AND (id= " << aufgabenID << " );";
									sqlString = sqlStringstream.str();
									sqlBefehl = sqlString.c_str();
									// SQL Befehl ausführen
									datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, ausgabe, 0, &fehlerNachricht);

									sqlite3_stmt *stmt2;
									sqlite3_prepare(datenbank, sqlBefehl, -1, &stmt2, NULL);//preparing the statement
									stat = sqlite3_step(stmt2);
									lastsavedNumber[k] = aufgabenID+1;

									if (aufgabenID == aufgabenMax && stat == SQLITE_DONE) {
										cout << gebeAufSpaltenbreiteAngepasstWieder(" ");
										lastsavedNumber[k] = aufgabenMax;
									}

								}
							}
						}
					}
				}
				cout << endl;
			}
		}


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
			case 1: {  // Aufgabe erstellen:
				// Benutzereingaben
				cin.ignore();
				cout << "Name" << endl;
				getline(cin, eingabeTitel);
				//if (eingabeTitel == "") eingabeTitel = "";
				cout << "Beschreibung" << endl;
				getline(cin, eingabeBeschreibung);
				cout << "Bearbeiter" << endl;
				getline(cin, eingabeBearbeiter);

				// SQL Befehl erstellen
				sqlStringstream << "INSERT INTO `Board` (titel,beschreibung,bearbeiter) " <<  "VALUES ( '" << eingabeTitel << "', '" << eingabeBeschreibung << "','" << eingabeBearbeiter << "');";
				sqlString = sqlStringstream.str();
				sqlBefehl = sqlString.c_str();
				// SQL Befehl ausführen
				datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, callback, 0, &fehlerNachricht);

				break;
			}
			case 2: {  // Aufgabe bearbeiten
				// Benutzereingaben
				cout << "Welche Aufgabe?" << endl;
				cin >> eingabeID;
				if (cin.fail()) {
					break;
				}
				cin.ignore();
					cout << "Was willst du bearbeiten?" << endl;
					cout << "1. Titel" << endl;
					cout << "2. Beschreibung" << endl;
					cout << "3. Bearbeiter" << endl;
					cin >> benutzerEingabe;
					if (cin.fail()) {
						break;
					}
					cin.ignore();
					switch (benutzerEingabe) {
						case 1: {
							//Benutzereingabe
							cout << "titel: ";
							getline(cin, eingabeTitel);

							// SQL Befehl erstellen
							sqlStringstream << "UPDATE `Board` set titel = '" << eingabeTitel << "' where ID = " << eingabeID << ";";
							sqlString = sqlStringstream.str();
							sqlBefehl = sqlString.c_str();

							break;
						}
						case 2: {
							//Benutzereingabe
							cout << "beschreibung: ";
							getline(cin, eingabeBeschreibung);

							// SQL Befehl erstellen
							sqlStringstream << "UPDATE `Board` set beschreibung = '" << eingabeBeschreibung << "' where ID = " << eingabeID << ";";
							sqlString = sqlStringstream.str();
							sqlBefehl = sqlString.c_str();

							break;
						}
						case 3: {
							//Benutzereingabe
							cout << "bearbeiter: ";
							getline(cin, eingabeBearbeiter);

							// SQL Befehl erstellen
							sqlStringstream << "UPDATE `Board` set bearbeiter = '" << eingabeBearbeiter << "' where ID = " << eingabeID << ";";
							sqlString = sqlStringstream.str();
							sqlBefehl = sqlString.c_str();

							break;
						}
						default: {
							break;
						}
					}

					// SQL Befehl ausführen
					datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, callback, 0, &fehlerNachricht);

				break;
			}
			case 3: {  // Aufgabe löschen
				// Benutzereingabe
				cout << "Welche Aufgabe?" << endl;
				cin >> eingabeID;
				if (cin.fail()) {
					break;
				}

				// SQL Befehl erstellen
				sqlStringstream << "DELETE from `Board` where ID = " << eingabeID << ";";
				sqlString = sqlStringstream.str();
				sqlBefehl = sqlString.c_str();
				// SQL Befehl ausführen
				datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, callback, 0, &fehlerNachricht);

				break;
			}
			case 4: {  // Aufgabe verschieben
				// Benutzereingaben
				cout << "Welche Aufgabe?" << endl;
				cin >> eingabeID;
				if (cin.fail()) {
					break;
				}
				cout << "Wohin verschieben?" << endl;
				cin >> eingabeSpalte;

				if ((!eingabeSpalte._Equal("Backlog") && !eingabeSpalte._Equal("ToDo") && !eingabeSpalte._Equal("Doing") && !eingabeSpalte._Equal("Done")) || cin.fail() )
					break;

				// SQL Befehl erstellen
				sqlStringstream << "UPDATE `Board` set spalte = '" << eingabeSpalte << "' where ID = " << eingabeID << ";";
				sqlString = sqlStringstream.str();
				sqlBefehl = sqlString.c_str();
				// SQL Befehl ausführen
				datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, callback, 0, &fehlerNachricht);

				break;
			}
			case 5: {  // Aufgabe lesen
				// Benutzereingabe
				cout << "Welche Aufgabe?" << endl;
				cin >> eingabeID;
				if (cin.fail()) {
					break;
				}

				system("CLS");
				// SQL Befehl erstellen
				sqlStringstream << "SELECT * from `Board` where ID = " << eingabeID << ";";
				sqlString = sqlStringstream.str();
				sqlBefehl = sqlString.c_str();
				// SQL Befehl ausführen
				datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, callback, 0, &fehlerNachricht);

				break;
			}
			default: {  // Default
				break;
			}
		}

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		_getch();
		system("CLS");
	}
	// Datenbank schließen
	sqlite3_close(datenbank);

	return 0;
}

