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


// STRUKTUREN
struct Aufgabe {
	string titel;
	string beschreibung;
	string bearbeiter;
	int id;
	string spalte;
};

struct Board {
	const string alleSpalten[SPALTENANZAHL] = { "Backlog", "ToDo", "Doing", "Done" };


}KanBamBoard;


// METHODEN
void gebeAlleAufgabenAusSpalteAus(string spalte) {

}

Aufgabe aufgabeErstellen(string titel, string beschreibung, string bearbeiter) {
	Aufgabe neueAufgabe;
	neueAufgabe.titel = titel;
	neueAufgabe.beschreibung = beschreibung;
	neueAufgabe.bearbeiter = bearbeiter;
	neueAufgabe.spalte = "Backlog";
	return neueAufgabe;
}

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
				cin.ignore();
				cout << "Was willst du bearbeiten?" << endl;
				cout << "Gebe nichts ein, wenn du das Feld unbearbeitet lassen willst." << endl;
				cout << "titel: ";
				getline(cin, eingabeTitel);
				cout << "beschreibung: ";
				getline(cin, eingabeBeschreibung);
				cout << "bearbeiter: ";
				getline(cin, eingabeBearbeiter);

				// SQL Befehl erstellen
				sqlStringstream << "UPDATE `Board` set titel = '" << eingabeTitel << "', beschreibung = '" << eingabeBeschreibung << "', bearbeiter = '" << eingabeBearbeiter << "' where ID = " << eingabeID << ";";
				sqlString = sqlStringstream.str();
				sqlBefehl = sqlString.c_str();
				cout << sqlBefehl;
				// SQL Befehl ausführen
				datenbankStatus = sqlite3_exec(datenbank, sqlBefehl, callback, 0, &fehlerNachricht);
				abfrageDatenbankStatus(datenbankStatus, fehlerNachricht);

				break;
			}
			case 3: {	// Aufgabe löschen
						// Benutzereingabe
				cout << "Welche Aufgabe?" << endl;
				cin >> eingabeID;

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
				cout << "Wohin verschieben?" << endl;
				cin >> eingabeSpalte;

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

		_getch();
		system("CLS");
	}
	// Datenbank schließen
	sqlite3_close(datenbank);

	return 0;
}

