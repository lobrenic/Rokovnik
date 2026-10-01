#include <iostream>
#include "notes.h"
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <sstream>
using namespace std;

string filename="beleske.txt";

void listNotes(const vector<Note>& notes);
bool parseDate(const string& s, tm& out);
int main() {
	Notes n(filename);
	int choice;
	
	do {
		system("cls");
		cout << "\n===== ROKOVNIK =====\n";
		cout << "1. Dodaj belesku\n";
		cout << "2. Prikazi sve beleske\n";
		cout << "3. Prikazi beleske po datumu\n";
		cout << "4. Obrisi belesku po id\n";
		cout << "0. Izlaz\n";
		cout << "====================\n";
		cout << "Vas izbor: ";
		cin >> choice;
		if (choice == 0) {
			return 0;
		}
		switch (choice)
		{
		case 1: {
			Note newNote;
			newNote.id = -1;
			string dateStr;
			system("cls");
			cout << "\nUnesite datum u formatu dd.mm.YYYY: ";
			cin >> dateStr;
			
			tm date{};
			if (!parseDate(dateStr, date)) {
				cout << "\nNeispravan datum, pritisnite enter: ";
				cin.ignore();
				cin.get();
				break;
			}
			cout << "\nUnesite belesku: ";
			cin.ignore();
			newNote.date = date;
			getline(cin, newNote.text);
			n.addNote(newNote);
			cout << "\nBeleska uspesno dodata, pritisnite enter: ";
			cin.get();
			break;
		}
		case 2: {
			system("cls");
			vector<Note> notes = n.getAllNotes();
			listNotes(notes);
			cout << "Press enter: ";
			cin.ignore();
			cin.get();
			break;
		}
		case 3: {
			system("cls");
			string critDate;
			cout << "\nUnesite datum u formatu dd.mm.YYYY: ";
			cin >> critDate;
			
			tm date{};
			if (!parseDate(critDate, date)) {
				cout << "\nNeispravan datum, pritisnite enter: ";
				cin.ignore();
				cin.get();
				break;
			}
			vector<Note> critNotes = n.getAllNotesForDate(date);
			listNotes(critNotes);
			cout << "Press enter: ";
			cin.ignore();
			cin.get();
			break;
		}
		case 4: {
			system("cls");
			vector<Note> notes = n.getAllNotes();
			listNotes(notes);
			string idStr;
			cout << "\nUnesite id beleske koju zelite da izbrisete: ";
			int id=-1;
			cin >> idStr;
			try {
				id = stoi(idStr);
			}
			catch (...) {
				cout << "\nNiste uneli broj, pritisnite enter da se vratite u meni: ";
				cin.ignore();
				cin.get();
				break;
			}
			bool uspeh = n.removeNote(id);
			if (uspeh) 
				cout << "\nUspesno je uklonjena beleska, pritisnite enter da se vratite u meni: ";
			else
				cout << "Doslo je do greske, pritisnite enter da se vratite u meni: ";
			cin.ignore();
			cin.get();
			break;
		}
		default:
			break;
		}
	} while (choice!=0);
	
}

void listNotes(const vector<Note>& notes) {
	cout << "\n===== Lista beleski =====\n";
	for (const Note& note : notes)
		cout << '[' << note.id << "] " << note.text << "\t" << put_time(&note.date, "%d.%m.%Y") << endl;
	cout << "\n=========================\n";
}

bool parseDate(const string& s, tm& out) {
	istringstream ss(s);
	ss >> get_time(&out, "%d.%m.%Y");
	return !ss.fail();
}