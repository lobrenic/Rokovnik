#include <iostream>
#include "notes.h"
#include <string>
#include <ctime>
#include <cstdlib>
using namespace std;

string filename="beleske.txt";
int main() {
	Notes n(filename);
	int choice;
	
	do {
		system("cls");
		cout << "\n===== ROKOVNIK =====\n";
		cout << "1. Dodaj belesku\n";
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
			system("cls");
			cout << "Unesite belesku: ";
			cin.ignore();
			getline(cin, newNote.text);
			n.addNote(newNote);
			break;
		}

		default:
			break;
		}
	} while (choice!=0);
	
}