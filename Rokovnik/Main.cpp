#include <iostream>
#include "notes.h"
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <sstream>
#include <stdexcept>
using namespace std;

const string filename="beleske.txt";

void listNotes(const vector<Note>& notes);
bool parseDate(const string& s, tm& out);
int readInt();
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
		cout << "5. Izmeni belesku\n";
		cout << "0. Izlaz\n";
		cout << "====================\n";
		cout << "Vas izbor: ";
		try {
			choice = readInt();
		}
		catch (const exception&) {
			choice = -1;   
		}
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
			if (notes.empty()) {
				cout << "\nNema beleski za dati kriterijum, pritisnite enter da se vratite u meni: ";
				cin.ignore();
				cin.get();
				break;
			}	
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
			if (critNotes.empty()) {
				cout << "\nNema beleski za dati kriterijum, pritisnite enter da se vratite u meni: ";
				cin.ignore();
				cin.get();
				break;
			}
			listNotes(critNotes);
			cout << "Press enter: ";
			cin.ignore();
			cin.get();
			break;
		}
		case 4: {
			system("cls");
			vector<Note> notes = n.getAllNotes();
			if (notes.empty()) {
				cout << "\nNema beleski za dati kriterijum, pritisnite enter da se vratite u meni: ";
				cin.ignore();
				cin.get();
				break;
			}
			listNotes(notes);
			string idStr;
			cout << "\nUnesite id beleske koju zelite da izbrisete: ";
			int id;
			try {
				id = readInt();
			}
			catch (const exception&) {
				cout << "\nNiste uneli broj, pritisnite enter da se vratite u meni: ";
				cin.ignore();
				cin.get();
				break;
			}
			if (id == 0) break;
			bool uspeh = n.removeNote(id);
			if (uspeh) 
				cout << "\nUspesno je uklonjena beleska, pritisnite enter da se vratite u meni: ";
			else
				cout << "\nDoslo je do greske, pritisnite enter da se vratite u meni: ";
			cin.ignore();
			cin.get();
			break;
		}
		case 5: {
			system("cls");
			listNotes(n.getAllNotes());
			cout << "\nUnesite id da bi izabrali belesku za izmenu(ili 0 za povratak): ";
			string idStr;
			int id;
			try {
				id = readInt();
			}
			catch (const exception&) {
				cout << "\nNiste uneli broj, pritisnite enter da se vratite u meni: ";
				cin.ignore();
				cin.get();
				break;
			}
			if (id == 0) break;
			
			string newText;
			string newDateStr;
			tm newDate{};
			cout << "\nUnesite novi datum u formatu dd.mm.YYYY: ";
			cin >> newDateStr;
			if (!parseDate(newDateStr, newDate)) {
				cout << "\nNeispravan datum, pritisnite enter: ";
				cin.ignore();
				cin.get();
				break;
			}
			cout << "\nUnesite novi tekst: ";
			cin.ignore();
			
			getline(cin,newText);
			
			
			try {
				n.editNote(id, newDate, newText);
				cout << "\nBeleska uspesno izmenjena, pritisnite enter: ";
			}
			catch (const exception& e) {
				cout << "\nGreska, " << e.what() << ", pritisnite enter: ";
			}
			
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
	if (ss.fail())
		return false;
	tm copy = out;
	mktime(&copy);
	return copy.tm_mday == out.tm_mday
		&& copy.tm_year == out.tm_year
		&& copy.tm_mon == out.tm_mon;
	
}

int readInt() {
	string s;
	cin >> s;
	size_t pos;
	int value = stoi(s, &pos);
	if (pos != s.size())
		throw invalid_argument("Unos nije ceo broj");  
	return value;
}