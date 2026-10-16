#include "ui.h"
#include <iostream>
#include <iomanip>
#include "input.h"
#include "datetime.h"

void UI::printMenu() {
	std::cout << "\n===== ROKOVNIK =====\n"
		<< "1. Dodaj belesku\n"
		<< "2. Prikazi predstojece beleske\n"
		<< "3. Prikazi beleske po datumu\n"
		<< "4. Obrisi belesku po id\n"
		<< "5. Izmeni belesku\n"
		<< "6. Prikazi sve ispite\n"
		<< "7. Pretrazi beleske po tekstu\n"
		<< "0. Izlaz\n"
		<< "====================\n"
		<< "Vas izbor: ";
}
void UI::listNotes(const std::vector<Note>& list) {
	std::cout << "\n===== Lista beleski =====\n";
	std::string until;
	for (const Note& note : list) {
		formatDaysUntil(until, note.date);
		std::cout << '[' << note.id << "] \t"
			<< std::put_time(&note.date, "%d.%m.%Y") << "\t"
			<< until << "\t" << note.text << '\n';
		if (note.exam) {
			const ExamInfo& e = *note.exam;
			std::cout << "      [ISPIT] " << e.subject;
			if(!e.room.empty())
				std::cout << ", sala " << e.room;
			std::cout << ", " << std::setfill('0')
				<< std::setw(2) << e.hour << ":"
				<< std::setw(2) << e.minute << std::setfill(' ')
				<< ", " << e.durationMin << " min" << '\n';
		}
	}
	std::cout << "\n=========================\n";
}

void UI::addNoteOption() {
	clearScreen();
	tm date{};
	if (!readDate("Unesite datum (dd.mm.YYYY) ili 0 za povratak: ", date))
		return;
	if (notes.isPast(date)) {
		pause("Datum je u proslosti, pritisnite enter: ");
		return;
	}
	Note note;
	note.date=date;
	if (readYesNo("Da li je ispit? (d/n): ")) {
		ExamInfo info;
		readExamInfo(info);
		note.exam = info;
	}
	std::cout << "Unesite belesku: ";
	note.text = readLine();
	notes.addNote(note);
	pause("Beleska uspesno dodata, pritisnite enter: ");
}

void UI::showUpcomingOption() {
	clearScreen();
	std::vector<Note> list = notes.getUpcomingNotes();
	if (list.empty()) {
		pause("Nema predstojecih beleski, pritisnite enter za povratak: ");
		return;
	}
	listNotes(list);
	pause("Pritisnite enter za povratak: ");

}

void UI::showByDateOption() {
	clearScreen();
	std::tm date{};
	if (!readDate("Unesite datum (dd.mm.YYYY) ili 0 za povratak: ", date))
		return;
	std::vector<Note> list = notes.getAllNotesForDate(date);
	if (list.empty()) {
		pause("Nema beleski za taj datum, pritisnite enter za povratak: ");
		return;
	}
	listNotes(list);
	pause("Pritisnite enter za povratak: ");
}
void UI::deleteOption() {
	clearScreen();
	std::vector<Note> list = notes.getAllNotes();
	if (list.empty()) {
		pause("Nema beleski, pritisnite enter za povratak: ");
		return;
	}
	listNotes(list);
	std::cout << "\nUnesite id beleske za brisanje (ili 0 za povratak): ";
	int id = readInt();
	if (id == 0)return;
	if (notes.removeNote(id))
		pause("Beleska je uklonjena, pritisnite enter: ");
	else
		pause("Beleska sa tim id-jem ne postoji, pritisnite enter: ");
}

void UI::editOption() {
	clearScreen();
	std::vector<Note> list = notes.getAllNotes();
	if (list.empty()) {
		pause("Nema beleski, pritisnite enter za povratak: ");
		return;
	}
	listNotes(list);
	std::cout << "\nUnesite id beleske za izmenu (ili 0 za povratak): ";
	int id = readInt();
	if (id == 0)return;
	std::tm newDate{};
	if (!readDate("Novi datum (dd.mm.YYYY) ili 0 za povratak: ", newDate))
		return;
	if (notes.isPast(newDate)) {
		pause("Datum je u proslosti, pritisnite enter: ");
		return;
	} 
	std::optional<ExamInfo> newExam;
	if (readYesNo("Da li je ispit? (d/n): ")) {
		ExamInfo info;
		readExamInfo(info);
		newExam = info;
	}
	std::cout << "Novi tekst: ";
	std::string newText = readLine();
	notes.editNote(id, newDate, newExam, newText);
	pause("Beleska je izmenjena, pritisnite enter: ");
}

void UI::showExamsOption() {
	clearScreen();
	std::vector<Note> list = notes.getAllExams();
	if (list.empty()) {
		pause("Nema predstojecih ispita, pritisnite enter za povratak: ");
		return;
	}
	listNotes(list);
	pause("Pritisnite enter za povratak: ");
}

void UI::searchOption() {
	clearScreen();
	std::cout << "Unesite tekst za pretragu (ili samo Enter za povratak): ";
	std::string query = readLine();
	if (query.empty())return;
	std::vector<Note> list = notes.searchByText(query);
	if (list.empty()) {
		pause("Nema beleski sa tim tekstom, pritisnite enter za povratak: ");
		return;
	}
	listNotes(list);
	pause("Pritisnite enter za povratak: ");
}

UI::UI(Notes& notes) :notes(notes){}
void UI::run() {
	int choice = -1;
	do {
		if (!std::cin)break;
		clearScreen();
		printMenu();
		try {
			choice = readInt();
		}
		catch (const std::exception&) {
			choice = -1;
		}
		try {
			switch (choice) {
			case 1: addNoteOption(); break;
			case 2: showUpcomingOption(); break;
			case 3: showByDateOption(); break;
			case 4: deleteOption(); break;
			case 5: editOption(); break;
			case 6: showExamsOption(); break;
			case 7: searchOption(); break;
			case 0: break;
			default: pause("Nepoznata opcija, pritisnite enter: ");
			}
		}
		catch (const std::exception& e) {
			pause(std::string("Greska: ") + e.what() + ", pritisnite enter: ");
		}
	} while (choice != 0);
	
}