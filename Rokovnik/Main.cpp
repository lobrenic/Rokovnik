#include <iostream>
#include "notes.h"
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iomanip>
using namespace std;

string filename="beleske.txt";
int main() {
	Notes n(filename);
	int choice;
	
	do {
		system("cls");
		cout << "\n===== ROKOVNIK =====\n";
		cout << "1. Dodaj belesku\n";
		cout << "2. Prikazi sve beleske\n";
		cout << "3. Prikazi beleske po datumu\n";
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
			int day = stoi(dateStr.substr(0, 2));
			int month = stoi(dateStr.substr(3, 2));
			int year = stoi(dateStr.substr(6, 4));
			newNote.date.tm_year = year - 1900;
			newNote.date.tm_mon = month - 1;
			newNote.date.tm_mday = day;
			cout << "\nUnesite belesku: ";
			cin.ignore();
			getline(cin, newNote.text);
			n.addNote(newNote);
			cout << "\nBeleska uspesno dodata, pritisnite enter: ";
			cin.get();
			break;
		}
		case 2: {
			system("cls");
			vector<Note> notes = n.getAllNotes();
			cout << "\n===== Lista beleski =====\n";
			for (Note note : notes)
				cout << '[' << note.id << "] " << note.text << "\t" << put_time(&note.date, "%d.%m.%Y") << endl;
			cout << "\n=========================\n";
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
			int day = stoi(critDate.substr(0, 2));
			int month = stoi(critDate.substr(3, 2));
			int year = stoi(critDate.substr(6, 4));
			tm date{};
			date.tm_year = year - 1900;
			date.tm_mon = month - 1;
			date.tm_mday = day;
			vector<Note> critNotes = n.getAllNotesForDate(date);
			cout << "\n===== Lista beleski =====\n";
			for (Note note : critNotes)
				cout << '[' << note.id << "] " << note.text << "\t" << put_time(&note.date, "%d.%m.%Y") << endl;
			cout << "\n=========================\n";
			cout << "Press enter: ";
			cin.ignore();
			cin.get();
			break;
		}
		default:
			break;
		}
	} while (choice!=0);
	
}