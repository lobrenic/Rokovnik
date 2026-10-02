#pragma once
#include <ctime>
#include <string>
#include <vector>

struct Note {
	int id=-1;
	std::tm date{};
	std::string text;
};

class Notes {
private:
	std::vector<Note> notes;
	std::string filename;

	void load();
	void save() const;
	int generateId() const;
public:
	Notes(const std::string& filename);
	const std::vector<Note>& getAllNotes() const;
	std::vector<Note> getAllNotesForDate(const std::tm& date) const;
	void addNote(const Note& note);
	bool removeNote(int id);
	void editNote(int id,const std::tm& newDate,const std::string& newText);
};