#pragma once
#include "notes.h"
#include <vector>
class UI {
private:
	Notes& notes;

	static void printMenu();
	static void listNotes(const std::vector<Note>& list);

	void addNoteOption();
	void showUpcomingOption();
	void showByDateOption();
	void deleteOption();
	void editOption();
	void showExamsOption();
	void searchOption();
public:
	explicit UI(Notes& notes);
	void run();
};