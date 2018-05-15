//INCLUDE VERZEICHNISSE
#include "stdafx.h"
#include "conio.h"
#include "iostream"
#include "string"
#include "sstream"
#include "sqlite3.h"

// DEFINES ANLEGEN
#define SPALTENANZAHL 4

// USING VERZEICHNISSE
using namespace std;


void abfrageDatenbankStatus(int datenbankStatus, char *fehlerNachricht) {
	if (datenbankStatus != SQLITE_OK) {
		//fprintf(stderr, "SQL error: %s\n", zErrMsg);
		cout << stderr << "SQL fehler: " << fehlerNachricht << endl;
		sqlite3_free(fehlerNachricht);
	}
	else {
		//fprintf(stdout, "Erfolgreich.");
		cout << stdout << "Erfolgreich." << endl;
	}
}

static int callback(void *data, int argc, char **argv, char **azColName) {
	int i;
	fprintf(stderr, "%s: ", (const char*)data);

	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}


string gebeAufWertAngepasstWieder(string wert, int max) {
	string newWert;

	if (wert.length < max) {
		for (int i = 0; i < max; i++) {
			if (i < wert.length) {
				newWert[i] = wert[i];
			}else if (i <= wert.length) {
				newWert[i] = ' ';
			}
		}
	}
	else {
		for (int i = 0; i < max; i++) {
			if (i < max - 3) {
				newWert[i] = wert[i];
			}
			else {
				newWert[i] = '.';
			}
		}
	}
	return newWert;
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


	// DATENBANK VERBINDUNG
	// Datenbank öffnen
	datenbankStatus = sqlite3_open("board.db", &datenbank);
	if (datenbankStatus) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(datenbank));
		return 0;
	}
	else {
		fprintf(stdout, "Opened database successfully\n");
	}
	// Standard Tabellen Initialisierung
	sqlBefehl = "CREATE TABLE `Board` ("  \
		"`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE," \
		"`titel`	TEXT," \
		"`beschreibung`	TEXT," \
		"`bearbeiter`	TEXT," \
		"`spalte`	TEXT NOT NULL DEFAULT 'Backlog');";
	// SQL Befehl ausführen
	datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, callback, 0, &fehlerNachricht);
	abfrageDatenbankStatus(datenbankStatus, fehlerNachricht);


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


		// AUSGABE
		// BOARD

		const int spaltenAnzahl = 4;
		const char *alleSpalten[spaltenAnzahl] = { "Backlog", "ToDo", "Doing", "Done" };
		const int spaltenbreite = 10;		//Änderbar
		const int boardlänge = 30;		// Änderbar
		const int boardbreite = (spaltenbreite * spaltenAnzahl) + spaltenAnzahl + 1;

		const int abstandZeichenPositionengröße = spaltenAnzahl + 1;
		int abstandZeichenPositionen[abstandZeichenPositionengröße];
		abstandZeichenPositionen[0] = 0;
		for (int i = 1; i <= spaltenAnzahl; i++) {
			abstandZeichenPositionen[i] = ((i) * spaltenbreite) + (i);
		}



		for (int reihe = 0; reihe < boardlänge; reihe++) {
			//  ============================================  //
			if (reihe == 0) {
				for (int i = 0; i < boardbreite; i++) {
					cout << "=";
				}
				cout << endl;
			}


			// |Backlog   |ToDo      |Doing      |Done      |  //
			if (reihe == 1) {
				for (int i = 0; i < boardbreite; i ++) {
					for (int k = 0; k < abstandZeichenPositionengröße; k++) {
						if (i == abstandZeichenPositionen[k]) {
							cout << "|";
							k= abstandZeichenPositionengröße;
						}else if (k == abstandZeichenPositionengröße - 1) {	// zeichenausgabe
							cout << "-";
						}
					}
				}
				cout << endl;
			}


			//  |----------|----------|----------|----------|  //
			if (reihe == 2) {
				for (int i = 0; i < boardbreite; i++) {
					for (int k = 0; k < abstandZeichenPositionengröße; k++) {
						if (i == abstandZeichenPositionen[k]) {
							cout << "|";
							k = abstandZeichenPositionengröße;
						}
						else if (k == abstandZeichenPositionengröße - 1) {	// zeichenausgabe
							cout << "-";
						}
					}
				}
				cout << endl;
			}


			// |Backlog   |ToDo      |Doing      |Done      |  //
			if (reihe == 3) {
				for (int spalte = 0; spalte < boardbreite; spalte++) {
					//  |           |          |          |
					for (int k = 0; k < abstandZeichenPositionengröße; k++) {
						if (spalte == abstandZeichenPositionen[k]) {
							cout << "|";
						}
						else if (spalte == abstandZeichenPositionen[k] + 1) {
							cout << gebeAufWertAngepasstWieder(alleSpalten[k], spaltenbreite);
						}
					}
				}
				cout << endl;
			}
		}







		/*
		// SQL Befehl erstellen
		sqlStringstream << "SELECT * from `Board`;";
		sqlString = sqlStringstream.str();
		sqlBefehl = sqlString.c_str();
		// SQL Befehl ausführen
		datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, callback, 0, &fehlerNachricht);
		abfrageDatenbankStatus(datenbankStatus, fehlerNachricht);
		*/










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
				abfrageDatenbankStatus(datenbankStatus, fehlerNachricht);

				break;
			}
			case 2: {	// Aufgabe bearbeiten
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
					abfrageDatenbankStatus(datenbankStatus, fehlerNachricht);
				break;
			}
			case 3: {	// Aufgabe löschen
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
				abfrageDatenbankStatus(datenbankStatus, fehlerNachricht);

				break;
			}
			case 4: {	// Aufgabe verschieben
						// Benutzereingaben
				cout << "Welche Aufgabe?" << endl;
				cin >> eingabeID;
				if (cin.fail()) {
					break;
				}
				cout << "Wohin verschieben?" << endl;
				cin >> eingabeSpalte;

				if ((!eingabeSpalte._Equal("Backlog") && !eingabeSpalte._Equal("ToDO") && !eingabeSpalte._Equal("Doing") && !eingabeSpalte._Equal("Done")) || cin.fail() )
					break;

				// SQL Befehl erstellen
				sqlStringstream << "UPDATE `Board` set spalte = '" << eingabeSpalte << "' where ID = " << eingabeID << ";";
				sqlString = sqlStringstream.str();
				sqlBefehl = sqlString.c_str();
				// SQL Befehl ausführen
				datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, callback, 0, &fehlerNachricht);
				abfrageDatenbankStatus(datenbankStatus, fehlerNachricht);

				break;
			}
			case 5: {	// Aufgabe lesen
						// Benutzereingabe
				cout << "Welche Aufgabe?" << endl;
				cin >> eingabeID;
				if (cin.fail()) {
					break;
				}

				// SQL Befehl erstellen
				sqlStringstream << "SELECT * from `Board` where ID = " << eingabeID << ";";
				sqlString = sqlStringstream.str();
				sqlBefehl = sqlString.c_str();
				// SQL Befehl ausführen
				datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, callback, 0, &fehlerNachricht);
				abfrageDatenbankStatus(datenbankStatus, fehlerNachricht);

				break;
			}
			default: {	// Default

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

