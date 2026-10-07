#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "notes.h"
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cmath>
using namespace std;

const string filename="beleske.txt";

void listNotes(const vector<Note>& notes);
bool parseDate(const string& s, tm& out);
int readInt();
vector<Note> sortedByDate(const vector<Note>& notes);
int daysUntil(const tm& date);
void formatDaysUntil(string& str, const tm& date);
int main() {
	Notes n(filename);
	int choice;
	
	do {
		system("cls");
		cout << "\n===== ROKOVNIK =====\n";
		cout << "1. Dodaj belesku\n";
		cout << "2. Prikazi predstojece beleske\n";
		cout << "3. Prikazi beleske po datumu\n";
		cout << "4. Obrisi belesku po id\n";
		cout << "5. Izmeni belesku\n";
		cout << "6. Prikazi sve ispite\n";
		cout << "0. Izlaz\n";
		cout << "====================\n";
		cout << "Vas izbor: ";
		try {
			choice = readInt();
		}
		catch (const exception&) {
			choice = -1;   
		}
		switch (choice)
		{
		case 1: {
			Note newNote;
			string dateStr;
			system("cls");
			cin.ignore();
			cout << "\nUnesite datum u formatu dd.mm.YYYY: ";
			cin >> dateStr;
			tm date{};
			if (!parseDate(dateStr, date)) {
				cout << "\nNeispravan datum, pritisnite enter: ";
				cin.ignore();
				cin.get();
				break;
			}
			if (n.isPast(date)) {
				cout << "\nDatum je u proslosti, pritisnite enter: ";
				cin.ignore();
				cin.get();
				break;
			}
			cout << "\nDa li je ispit? (d/n): ";
			char isExam;
			cin >> isExam;
			if (isExam != 'd' && isExam != 'n') {
				cout << "\nNeispravan unos, pritisnite enter za povratak na meni: ";
				cin.ignore();
				cin.get();
				break;
			}
			if (isExam == 'd')
				newNote.type = NoteType::Exam;
			cin.ignore();
			cout << "\nUnesite belesku: ";
			newNote.date = date;
			getline(cin, newNote.text);
			n.addNote(newNote);
			cout << "\nBeleska uspesno dodata, pritisnite enter: ";
			cin.get();
			break;
		}
		case 2: {
			system("cls");
			vector<Note> notes = n.getUpcomingNotes();
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
			if (n.isPast(date)) {
				cout << "\nDatum je u proslosti, pritisnite enter: ";
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
			vector<Note> notes = n.getAllNotes();
			if (notes.empty()) {
				cout << "\nNema beleski";
				cout << "\nPress enter: ";
				cin.ignore();
				cin.get();
				break;
			}
			listNotes(notes);
			cout << "\nUnesite id da bi izabrali belesku za izmenu(ili 0 za povratak): ";
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
			if (n.isPast(newDate)) {
				cout << "\nDatum je u proslosti, pritisnite enter: ";
				cin.ignore();
				cin.get();
				break;
			}
			cout << "\nDa li je ispit? (d/n): ";
			char isExam;
			NoteType type=NoteType::Note;
			cin >> isExam;
			if (isExam != 'd' && isExam != 'n') {
				cout << "\nNeispravan unos, pritisnite enter za povratak na meni: ";
				cin.ignore();
				cin.get();
				break;
			}
			if (isExam == 'd')type = NoteType::Exam;
			
			cout << "\nUnesite novi tekst: ";
			cin.ignore();
			getline(cin,newText);
			
			try {
				n.editNote(id, newDate, type,newText);
				cout << "\nBeleska uspesno izmenjena, pritisnite enter: ";
			}
			catch (const exception& e) {
				cout << "\nGreska, " << e.what() << ", pritisnite enter: ";
			}
			
			cin.get();
			break;
		}
		case 6: {
			system("cls");
			vector<Note> exams = n.getAllExams();
			if (exams.empty()) {
				cout << "\nNema predstojecih ispita";
				cout << "\nPress enter: ";
				cin.ignore();
				cin.get();
				break;
			}
			listNotes(exams);
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
int daysUntil(const tm& date) {
	tm target = date;
	target.tm_hour = 12;
	target.tm_min = 0;
	target.tm_sec = 0;
	target.tm_isdst = -1;

	time_t now = time(nullptr);
	tm today = *localtime(&now);
	today.tm_hour = 12;
	today.tm_min = 0;
	today.tm_sec = 0;
	today.tm_isdst = -1;

	double diff = difftime(mktime(&target), mktime(&today));
	return (int)round(diff / (60 * 60 * 24));
}
void formatDaysUntil(string& str,const tm& date) {
	int days = daysUntil(date);
	int years, mons, daysM;
	if (days < 0) {
		str = "Passed";
		return;
	}
	switch (days) {
	case 0:
		str = "Today";
		break;
	case 1:
		str = "Tomorrow";
		break;
	default: {
		years = days / 365;
		mons = (days % 365) / 30;
		daysM = days % 365 % 30; 
		str = "Time until: ";
		string parts;
		if (years) 
			parts += to_string(years) + (years == 1 ? " year" : " years");
		if (mons) {
			if (!parts.empty()) parts += ", ";
			parts += to_string(mons) + (mons == 1 ? " month" : " months");
		}
		if (daysM) {
			if (!parts.empty()) parts += ", ";
			parts += to_string(daysM) + (daysM == 1 ? " day" : " days");
		}
		str += parts;
		break;
	}
	}
}


void listNotes(const vector<Note>& notes) {
	cout << "\n===== Lista beleski =====\n";
	vector<Note> copy = sortedByDate(notes);
	string until;
	for (const Note& note : copy){
		formatDaysUntil(until, note.date);
		cout << '[' << note.id << "] " <<
			note.text << "\t" <<
			put_time(&note.date, "%d.%m.%Y") << "\t" <<
			until << "\t" <<
			"[" << typeToChar(note.type) << "]" << endl;
	}
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
vector<Note> sortedByDate(const vector<Note>& notes) {
	vector<Note> copy = notes;
	stable_sort(copy.begin(), copy.end(), [](const Note& a, const Note& b) {
		if (a.date.tm_year != b.date.tm_year)
			return a.date.tm_year < b.date.tm_year;
		if (a.date.tm_mon != b.date.tm_mon)
			return a.date.tm_mon < b.date.tm_mon;
		return a.date.tm_mday < b.date.tm_mday;
		});
	return copy;
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