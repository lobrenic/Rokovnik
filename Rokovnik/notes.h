#pragma once
#include <ctime>
#include <string>
#include <vector>
#include "noteType.h"

struct Note {
	int id=-1;
	std::tm date{};
	std::string text;
	NoteType type = NoteType::Note;
};

class Notes {
private:
	std::vector<Note> notes;
	std::string filename;

	void load();
	void save() const;
	int generateId() const;
	void toLower(std::string& s)const;
public:
	Notes(const std::string& filename);
	const std::vector<Note>& getAllNotes() const;
	std::vector<Note> getAllNotesForDate(const std::tm& date) const;
	void addNote(const Note& note);
	bool removeNote(int id);
	void editNote(int id,const std::tm& newDate,NoteType newType,const std::string& newText);
	std::vector<Note> getUpcomingNotes() const;
	bool isPast(const std::tm& date) const;
	std::vector<Note> getAllExams() const;
	void searchByText(const std::string& query, std::vector<Note>& list) const;
};